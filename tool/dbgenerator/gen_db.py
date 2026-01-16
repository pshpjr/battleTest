"""DB 코드 생성 도구 메인"""

import json
import argparse
from pathlib import Path
from .metadata import MetadataExtractor
from .code_generator import CodeGenerator


def save_migration_files(extractor: MetadataExtractor, config: dict):
    """마이그레이션 파일 저장"""
    migrations_dir = Path('db/migrations')
    migrations_dir.mkdir(parents=True, exist_ok=True)

    # 모든 테이블 조회 후 제외 목록 필터링
    all_tables = extractor.get_all_tables()
    excluded_tables = set(config.get('excluded_tables', []))
    tables = [t for t in all_tables if t not in excluded_tables]

    # 테이블 CREATE 쿼리 저장
    for table_name in tables:
        create_query = extractor.extract_table_create(table_name)
        file_path = migrations_dir / f"{table_name}.sql"

        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(create_query)

        print(f"✓ Saved: {file_path}")

    # 모든 SP 조회 후 제외 목록 필터링
    all_sps = extractor.get_all_stored_procedures()
    excluded_sps = set(config.get('excluded_stored_procedures', []))
    sps = [sp for sp in all_sps if sp not in excluded_sps]

    # SP CREATE 쿼리 저장
    for sp_name in sps:
        create_query = extractor.extract_sp_create(sp_name)
        file_path = migrations_dir / f"{sp_name}.sql"

        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(create_query)

        print(f"✓ Saved: {file_path}")


def run_generator(config_path: str):
    """전체 프로세스 실행"""
    # 설정 로드
    with open(config_path, 'r') as f:
        config = json.load(f)

    db_config = config['database']
    output_config = config['output']

    # 템플릿 디렉토리 (gen_db.py 기준 상대 경로)
    script_dir = Path(__file__).parent
    template_dir = script_dir / 'cppcodetemplate'

    print("Connecting to database...")

    with MetadataExtractor(db_config) as extractor:
        print("\n=== Extracting migration files ===")
        save_migration_files(extractor, config)

        print("\n=== Generating C++ code ===")
        generator = CodeGenerator(
            template_dir=str(template_dir),
            output_dir=output_config['directory'],
            namespace=output_config['namespace']
        )

        # 모든 SP 조회 후 제외 목록 필터링
        all_sps = extractor.get_all_stored_procedures()
        excluded_sps = set(config.get('excluded_stored_procedures', []))
        sp_names = [sp for sp in all_sps if sp not in excluded_sps]

        generator.generate_all(extractor, sp_names)

    print("\n✅ All done!")


def main():
    """CLI 진입점"""
    parser = argparse.ArgumentParser(description='DB Query Generator')
    parser.add_argument('--config', default='config.json', help='Config file path')
    args = parser.parse_args()

    run_generator(args.config)


if __name__ == '__main__':
    main()
