import configparser
import re
from pathlib import Path

# Config + target files
INI_PATH = "ntint.cfg"
TARGET_FILES = ["src/GraphicsManager.cpp", "src/DynamicRend.cpp"]

# Load INI
cfg = configparser.ConfigParser()
cfg.read(INI_PATH)
render = cfg["RENDER"]

# Values from INI (with defaults)
refresh_rate = render.get("RefreshRate", "60")
buffer_count = render.get("BufferCount", "2")

# Regex patterns for replacements
patterns = {
    r"(scd\.BufferCount\s*=\s*)\d+;": rf"\g<1>{buffer_count};",
    r"(scd\.BufferDesc\.RefreshRate\.Numerator\s*=\s*)\d+;": rf"\g<1>{refresh_rate};",
    r"(scd\.BufferDesc\.RefreshRate\.Denominator\s*=\s*)\d+;": r"\g<1>1;",
    r"(scd\.BufferDesc\.Format\s*=\s*)DXGI_FORMAT_[A-Z0-9_]+;": r"\g<1>DXGI_FORMAT_R8G8B8A8_UNORM;",
    r"(scd\.BufferUsage\s*=\s*)DXGI_USAGE_[A-Z0-9_]+;": r"\g<1>DXGI_USAGE_RENDER_TARGET_OUTPUT;",
    r"(scd\.SampleDesc\.Count\s*=\s*)\d+;": r"\g<1>1;",
    r"(scd\.SampleDesc\.Quality\s*=\s*)\d+;": r"\g<1>0;",
    r"(scd\.Windowed\s*=\s*)(TRUE|FALSE);": r"\g<1>TRUE;",
    r"(scd\.SwapEffect\s*=\s*)DXGI_SWAP_EFFECT_[A-Z0-9_]+;": r"\g<1>DXGI_SWAP_EFFECT_FLIP_DISCARD;",
    r"(scd\.Flags\s*=\s*)[A-Z0-9_|]+;": r"\g<1>DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;"
}

# Patch each file
for file_path in TARGET_FILES:
    path = Path(file_path)
    if not path.exists():
        print(f"Skipping {file_path} (not found)")
        continue

    code = path.read_text(encoding="utf-8")
    for pat, repl in patterns.items():
        code = re.sub(pat, repl, code)

    path.write_text(code, encoding="utf-8")
    print(f"Patched {file_path} with RefreshRate={refresh_rate} BufferCount={buffer_count}")