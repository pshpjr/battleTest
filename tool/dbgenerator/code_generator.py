"""C++ 코드 생성"""

from pathlib import Path
from jinja2 import Environment, FileSystemLoader
from .metadata import SPMetadata, MetadataExtractor


class CodeGenerator:
    """C++ 코드 생성"""

    def __init__(self, template_dir: str, output_dir: str, namespace: str):
        """
        Args:
            template_dir: 템플릿 디렉토리 경로
            output_dir: 출력 디렉토리 경로
            namespace: C++ 네임스페이스
        """
        self.template_dir = Path(template_dir)
        self.output_dir = Path(output_dir)
        self.namespace = namespace

        # Jinja2 환경 설정
        self.env = Environment(loader=FileSystemLoader(str(self.template_dir)))

    def generate_sp_header(self, sp_metadata: SPMetadata) -> str:
        """SP 헤더 파일 생성"""
        template = self.env.get_template('sp_header.j2')

        code = template.render(
            sp=sp_metadata,
            namespace=self.namespace
        )

        return code

    def save_sp_header(self, sp_metadata: SPMetadata):
        """SP 헤더 파일 저장"""
        code = self.generate_sp_header(sp_metadata)

        # 출력 디렉토리 생성
        self.output_dir.mkdir(parents=True, exist_ok=True)

        # 파일 저장
        file_path = self.output_dir / f"{sp_metadata.name}.h"
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(code)

        return file_path

    def generate_all(self, extractor: MetadataExtractor, sp_names: list) -> list:
        """모든 SP 헤더 파일 생성

        Args:
            extractor: MetadataExtractor 인스턴스 (연결된 상태)
            sp_names: SP 이름 목록

        Returns:
            생성된 파일 경로 목록
        """
        generated_files = []

        for sp_name in sp_names:
            metadata = extractor.extract_sp_metadata(sp_name)
            file_path = self.save_sp_header(metadata)
            generated_files.append(file_path)
            print(f"✓ Generated: {file_path}")

        return generated_files
