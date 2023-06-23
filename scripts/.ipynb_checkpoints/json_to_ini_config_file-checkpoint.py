from collections import OrderedDict
from typing import Any, Tuple, List
from pathlib import Path
import configparser
import json
import sys


base_config_file = '../simulations/omnetpp.ini'

def _sort_func(key_val: Tuple[str, Any]) -> int:
    if isinstance(key_val[1], (list, tuple)):
        return 2
    elif isinstance(key_val[1], dict):
        return 1

    return 0

def reorder_nested_dict(data: Any) -> Any:
    if isinstance(data, (list, tuple)):
        return [reorder_nested_dict(d) for d in data]
    elif isinstance(data, dict):
        out = []

        for key, val in sorted(data.items(), key=_sort_func):
            out.append((key, reorder_nested_dict(val)))

        return OrderedDict(out)

    return data

def json_to_ini_config(config: Any, base_name: str, is_top_object: bool=True, out: List[Any]=[]) -> Any:
    if isinstance(config, dict):
        if not is_top_object:
            out.append('')

        for key, value in reorder_nested_dict(config).items():
            out = json_to_ini_config(value, f'{base_name}.{key}', False)
    elif isinstance(config, (list, tuple)):
        for i, value in enumerate(config):
            out = json_to_ini_config(value, f'{base_name}[{i}]', False)
    else:
        if isinstance(config, str):
            config = f'"{config}"'

        out.append(f'{base_name} = {config}')

    if is_top_object:
        return '\n'.join(out)

    return out

if len(sys.argv) == 3:
    _, json_file, ini_file = sys.argv

    config_name = Path(ini_file).stem

    with open(json_file, 'r') as f:
        config = json.load(f)

    base_config = configparser.ConfigParser()
    base_config.read(
        Path(__file__) \
            .parent \
            .joinpath(base_config_file) \
            .resolve()
    )
    
    base_name = base_config['General']['network'].split('.')[-1]

    ini_file_content = json_to_ini_config(config, base_name)

    with open(ini_file, 'w') as f:
        f.write(f'[Config {config_name}]\n')
        f.write(ini_file_content)
else:
    raise ValueError('You must pass a path to the JSON file and a path for the new INI file')
