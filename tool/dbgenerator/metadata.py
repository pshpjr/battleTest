"""DB 메타데이터 추출"""

import mysql.connector
from dataclasses import dataclass
from typing import List
from .type_mapper import TypeMapper


@dataclass
class ColumnInfo:
    """컬럼 정보"""
    name: str
    cpp_type: str
    nullable: bool


@dataclass
class ParamInfo:
    """파라미터 정보"""
    name: str
    cpp_type: str
    mode: str  # IN, OUT, INOUT


@dataclass
class SPMetadata:
    """Stored Procedure 메타데이터"""
    name: str
    input_params: List[ParamInfo]
    output_columns: List[ColumnInfo]


class MetadataExtractor:
    """DB 메타데이터 추출"""

    def __init__(self, db_config: dict):
        """
        Args:
            db_config: DB 연결 설정 (host, port, user, password, database)
        """
        self.db_config = db_config
        self.conn = None
        self.cursor = None

    def connect(self):
        """DB 연결"""
        self.conn = mysql.connector.connect(
            host=self.db_config['host'],
            port=self.db_config['port'],
            user=self.db_config['user'],
            password=self.db_config['password'],
            database=self.db_config['database']
        )
        self.cursor = self.conn.cursor()

    def disconnect(self):
        """DB 연결 종료"""
        if self.cursor:
            self.cursor.close()
        if self.conn:
            self.conn.close()

    def __enter__(self):
        """컨텍스트 매니저 진입"""
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        """컨텍스트 매니저 종료"""
        self.disconnect()

    def extract_table_create(self, table_name: str) -> str:
        """테이블 CREATE 쿼리 추출"""
        self.cursor.execute(f"SHOW CREATE TABLE {table_name}")
        result = self.cursor.fetchone()
        return result[1]

    def extract_sp_create(self, sp_name: str) -> str:
        """SP CREATE 쿼리 추출"""
        self.cursor.execute(f"SHOW CREATE PROCEDURE {sp_name}")
        result = self.cursor.fetchone()
        return result[2]

    def get_sp_parameters(self, sp_name: str) -> List[ParamInfo]:
        """SP 입력 파라미터 정보 추출"""
        query = """
        SELECT PARAMETER_NAME, DATA_TYPE, PARAMETER_MODE
        FROM INFORMATION_SCHEMA.PARAMETERS
        WHERE SPECIFIC_NAME = %s
        AND PARAMETER_MODE = 'IN'
        ORDER BY ORDINAL_POSITION
        """
        self.cursor.execute(query, (sp_name,))

        params = []
        for row in self.cursor.fetchall():
            param_name = row[0]
            data_type = row[1]
            param_mode = row[2]
            cpp_type = TypeMapper.from_sql_type(data_type)

            params.append(ParamInfo(
                name=param_name,
                cpp_type=cpp_type,
                mode=param_mode
            ))

        return params

    def get_sp_output_schema(self, sp_name: str, params: List[ParamInfo]) -> List[ColumnInfo]:
        """SP 출력 스키마 추출 (빈 파라미터로 호출)"""
        # 트랜잭션 시작
        self.conn.start_transaction()

        try:
            # 파라미터 기본값 생성
            param_values = [
                TypeMapper.get_param_default_value(param.cpp_type)
                for param in params
            ]

            # SP 호출
            placeholders = ', '.join(['%s'] * len(param_values))
            call_query = f"CALL {sp_name}({placeholders})"
            self.cursor.execute(call_query, param_values)

            # 결과 메타데이터 추출
            columns = []
            if self.cursor.description:
                for col in self.cursor.description:
                    col_name = col[0]
                    type_code = col[1]
                    null_ok = col[6] if len(col) > 6 else True

                    cpp_type = TypeMapper.from_mysql_type_code(type_code)

                    columns.append(ColumnInfo(
                        name=col_name,
                        cpp_type=cpp_type,
                        nullable=null_ok
                    ))

            # 결과 소비
            self.cursor.fetchall()

            return columns

        finally:
            # 롤백
            self.conn.rollback()

    def extract_sp_metadata(self, sp_name: str) -> SPMetadata:
        """SP 전체 메타데이터 추출"""
        params = self.get_sp_parameters(sp_name)
        columns = self.get_sp_output_schema(sp_name, params)

        return SPMetadata(
            name=sp_name,
            input_params=params,
            output_columns=columns
        )

    def get_all_stored_procedures(self) -> List[str]:
        """데이터베이스의 모든 SP 목록 조회"""
        query = """
        SELECT ROUTINE_NAME
        FROM INFORMATION_SCHEMA.ROUTINES
        WHERE ROUTINE_SCHEMA = %s
        AND ROUTINE_TYPE = 'PROCEDURE'
        ORDER BY ROUTINE_NAME
        """
        self.cursor.execute(query, (self.db_config['database'],))
        return [row[0] for row in self.cursor.fetchall()]

    def get_all_tables(self) -> List[str]:
        """데이터베이스의 모든 테이블 목록 조회"""
        query = """
        SELECT TABLE_NAME
        FROM INFORMATION_SCHEMA.TABLES
        WHERE TABLE_SCHEMA = %s
        AND TABLE_TYPE = 'BASE TABLE'
        ORDER BY TABLE_NAME
        """
        self.cursor.execute(query, (self.db_config['database'],))
        return [row[0] for row in self.cursor.fetchall()]
