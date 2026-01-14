"""MySQL 타입을 C++ 타입으로 매핑"""


class TypeMapper:
    """MySQL 타입을 C++ 타입으로 매핑"""

    MYSQL_TYPE_MAP = {
        0: 'double',      # DECIMAL
        1: 'int8_t',      # TINY
        2: 'int16_t',     # SHORT
        3: 'int32_t',     # LONG
        4: 'float',       # FLOAT
        5: 'double',      # DOUBLE
        7: 'std::string', # TIMESTAMP
        8: 'int64_t',     # LONGLONG
        9: 'int32_t',     # INT24
        10: 'std::string', # DATE
        11: 'std::string', # TIME
        12: 'std::string', # DATETIME
        13: 'int16_t',     # YEAR
        15: 'std::string', # VARCHAR
        16: 'int8_t',      # BIT
        245: 'std::string', # JSON
        246: 'double',     # NEWDECIMAL
        247: 'std::string', # ENUM
        248: 'std::string', # SET
        249: 'std::string', # TINY_BLOB
        250: 'std::string', # MEDIUM_BLOB
        251: 'std::string', # LONG_BLOB
        252: 'std::string', # BLOB
        253: 'std::string', # VAR_STRING
        254: 'std::string', # STRING
    }

    SQL_TYPE_MAP = {
        'tinyint': 'int8_t',
        'smallint': 'int16_t',
        'mediumint': 'int32_t',
        'int': 'int32_t',
        'bigint': 'int64_t',
        'float': 'float',
        'double': 'double',
        'decimal': 'double',
        'varchar': 'std::string',
        'char': 'std::string',
        'text': 'std::string',
        'tinytext': 'std::string',
        'mediumtext': 'std::string',
        'longtext': 'std::string',
        'datetime': 'std::string',
        'timestamp': 'std::string',
        'date': 'std::string',
        'time': 'std::string',
        'blob': 'std::vector<uint8_t>',
        'tinyblob': 'std::vector<uint8_t>',
        'mediumblob': 'std::vector<uint8_t>',
        'longblob': 'std::vector<uint8_t>',
        'json': 'std::string',
    }

    DEFAULT_VALUES = {
        'tinyint': 0,
        'smallint': 0,
        'mediumint': 0,
        'int': 0,
        'bigint': 0,
        'float': 0.0,
        'double': 0.0,
        'decimal': 0.0,
        'varchar': "''",
        'char': "''",
        'text': "''",
        'tinytext': "''",
        'mediumtext': "''",
        'longtext': "''",
        'datetime': "'1970-01-01 00:00:00'",
        'timestamp': "'1970-01-01 00:00:00'",
        'date': "'1970-01-01'",
        'time': "'00:00:00'",
        'blob': "''",
        'json': "'{}'",
    }

    @classmethod
    def from_mysql_type_code(cls, type_code: int) -> str:
        """MySQL 타입 코드에서 C++ 타입 반환"""
        return cls.MYSQL_TYPE_MAP.get(type_code, 'std::string')

    @classmethod
    def from_sql_type(cls, sql_type: str) -> str:
        """SQL 타입 문자열에서 C++ 타입 반환"""
        sql_type_lower = sql_type.lower().split('(')[0]
        return cls.SQL_TYPE_MAP.get(sql_type_lower, 'std::string')

    @classmethod
    def get_default_value(cls, sql_type: str):
        """SQL 타입의 기본값 반환"""
        sql_type_lower = sql_type.lower().split('(')[0]
        return cls.DEFAULT_VALUES.get(sql_type_lower, "''")

    @classmethod
    def get_param_default_value(cls, cpp_type: str):
        """C++ 타입으로부터 파라미터 기본값 추정"""
        if 'int' in cpp_type:
            return 0
        elif 'float' in cpp_type or 'double' in cpp_type:
            return 0.0
        elif 'string' in cpp_type:
            return ''
        elif 'vector' in cpp_type:
            return b''
        else:
            return None
