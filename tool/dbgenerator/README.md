# DB 코드 생성 도구

MySQL 데이터베이스에서 Stored Procedure 메타데이터를 추출하여 C++ 헤더 파일을 자동 생성하는 도구입니다.

## 설치

```bash
cd tool/dbgenerator
pip install -r requirements.txt
```

## 사용법

```bash
# 기본 설정 파일 사용
python -m tool.dbgenerator.gen_db

# 커스텀 설정 파일 사용
python -m tool.dbgenerator.gen_db --config my_config.json
```

## 설정 파일 (config.json)

```json
{
  "database": {
    "host": "localhost",
    "port": 3306,
    "user": "dev",
    "password": "password",
    "database": "game_db"
  },
  "excluded_tables": ["internal_log", "temp_cache"],
  "excluded_stored_procedures": ["sp_internal_helper", "sp_admin_only"],
  "output": {
    "directory": "generated/db",
    "namespace": "DB"
  }
}
```

**참고:**
- 데이터베이스의 모든 테이블과 SP를 자동으로 가져옵니다
- `excluded_tables`, `excluded_stored_procedures`에 제외할 항목만 나열하세요
- 새로운 테이블/SP가 추가되면 자동으로 포함됩니다

## 디렉토리 구조

```
tool/dbgenerator/
├── __init__.py           # 패키지 초기화
├── type_mapper.py        # MySQL ↔ C++ 타입 변환
├── metadata.py           # DB 메타데이터 추출
├── code_generator.py     # C++ 코드 생성
├── gen_db.py             # 메인 진입점
├── config.json           # 설정 파일
├── requirements.txt      # 의존성
└── cppcodetemplate/
    └── sp_header.j2      # Jinja2 템플릿
```

## 생성 결과

- `db/migrations/` - 테이블 및 SP의 CREATE 쿼리 저장
- `{output.directory}/` - 생성된 C++ 헤더 파일
