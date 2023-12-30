import re
import sys

HEX_COLOR_RE = re.compile(r"#([A-Fa-f0-9]{3,8})")


def hexstr2int(hex_substring: str) -> int:
    return int(hex_substring, 16)


def hex_to_rgba(hex_color: str) -> str:
    if len(hex_color) in (3, 4):
        hex_color = "".join(c * 2 for c in hex_color)

    rgb = [hexstr2int(hex_color[i : i + 2]) for i in range(0, 6, 2)]
    alpha = hexstr2int(hex_color[6:8]) / 255 if len(hex_color) == 8 else None

    if alpha is not None:
        return "rgba({} {} {} / {:.5f})".format(*rgb, alpha)
    else:
        return "rgb({} {} {})".format(*rgb)


def convert_hex_colors_in_text(text: str) -> str:
    def replace_match(match: re.Match[str]) -> str:
        return hex_to_rgba(match.group(1))

    return HEX_COLOR_RE.sub(replace_match, text)


if __name__ == "__main__":
    input_css = sys.stdin.read()
    sys.stdout.write(convert_hex_colors_in_text(input_css))
