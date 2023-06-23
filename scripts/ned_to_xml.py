from pathlib import Path
import subprocess


def render_ned_to_xml(ned_file: str | Path, xml_file: str | Path) -> None:
    ned_file = ned_file if isinstance(ned_file, str) else str(ned_file)
    xml_file = xml_file if isinstance(xml_file, str) else str(xml_file)

    subprocess.run([
        pwsh_path, './ned_to_xml.ps1',
        '-NedFile', ned_file,
        '-XmlFile', xml_file
    ])
