from dataclasses import dataclass
from fractions import Fraction
import os

UCD_DIR = "ucd16"
DB_PATH = "src/xc_unicode_db.c"

# NOTE: Keep in sync with xc.h
MASK_UPPERCASE = 0x01
MASK_LOWERCASE = 0x02
MASK_CASED = 0x04
MASK_ALPHABETIC = 0x08
MASK_DECIMAL = 0x10
MASK_DIGIT = 0x20
MASK_NUMERIC = 0x40
MASK_XID_START = 0x80
MASK_XID_CONTINUE = 0x100
MASK_PRINTABLE = 0x200


class UnicodeDBWriter:
    def __init__(self, path, include_guard_name, mode="w", encoding="ascii"):
        self.file = open(path, mode=mode, encoding=encoding)
        self.guard = include_guard_name.upper() if include_guard_name else None
        if self.guard:
            self.file.write(f"#ifndef {self.guard}\n#define {self.guard}\n")

    def writeln(self, line):
        self.file.write(line)
        self.file.write("\n")

    def write(self, text):
        self.file.write(text)

    def write_struct(self, name, *fields):
        self.file.write(f"typedef struct {name} {{\n")
        for field in fields:
            self.file.write(f"    {field};\n")
        self.file.write(f"}} {name};\n\n")

    def write_array1d(self, type_, name, data, format="", max_line_len=80):
        assert len(data) != 0, "data cannot be empty"

        self.file.write(f"{type_} {name}[{len(data)}] = {{\n")
        line = "   "
        for num in data:
            num_str = f"{num:{format}}"
            if len(line) + len(num_str) + 2 > max_line_len:
                self.file.write(line + "\n")
                line = f"    {num_str},"
            else:
                line += " " + num_str + ","
        if line != "   ":
            self.file.write(line + "\n")
        self.write("};\n\n")

    def write_array2d(self, type_, name, data, format="", max_line_len=80):
        assert len(data) != 0, "data cannot be empty"

        self.file.write(f"{type_} {name}[{len(data)}][{len(data[0])}] = {{\n    ")
        for sub_array in data:
            self.write("{\n")
            line = "       "
            for num in sub_array:
                num_str = f"{num:{format}}"
                if len(line) + len(num_str) + 2 > max_line_len:
                    self.file.write(line + "\n")
                    line = f"        {num_str},"
                else:
                    line += " " + num_str + ","
            if line != "       ":
                self.file.write(line + "\n")
            self.write("    },")
        self.write("\n};\n\n")

    def write_code(self, code):
        lines = code.replace("\r\n", "\n").replace("\r", "\n").split("\n")
        while lines[0] == "":
            lines = lines[1:]
        if not lines:
            return
        ignore_indent = len(lines[0]) - len(lines[0].lstrip())
        for line in lines:
            self.file.write(line[ignore_indent:])
            self.file.write("\n")

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if self.guard:
            self.file.write(f"\n#endif // !{self.guard}\n")
        self.file.close()


@dataclass
class SpecialCasingEntry:
    code: int
    lower: list[int]
    upper: list[int]
    title: list[int]


@dataclass
class UnicodeDataEntry:
    code: int
    name: str | None
    general_category: str | None
    numeric_type: str | None
    numeric_value: int | float | None
    simple_lowercase_mapping: int
    simple_uppercase_mapping: int
    simple_titlecase_mapping: int


def data_lines(f, final_semicolon: bool):
    content = f.read().split("\n")
    for line in content:
        if not line or line.lstrip().startswith("#"):
            continue
        values = list(map(str.strip, line.split("#")[0].split(";")))
        yield values[:-1] if final_semicolon else values


def code_point(value: str | int) -> int:
    if type(value) is str:
        value = int(value.strip(), 16)
    if value > 0x10FFFF or value < 0:
        raise ValueError(f"{value:04X} is not a valid codepoint")
    return value


def make_unicode_data(file) -> dict[int, UnicodeDataEntry]:
    chars = {}
    for (
        code, name, general_category,
        _, _, _,
        num6, num7, num8, *_,
        upper, lower, title
    ) in data_lines(file, False):
        code = code_point(code)
        if "<" in name or not name:
            name = None
        upper = code_point(upper) if upper else code
        lower = code_point(lower) if lower else code
        title = code_point(title) if title else upper
        numeric_type = None
        numeric_value = None
        if num6:
            numeric_type = "Decimal"
            numeric_value = int(num6)
        elif num7:
            numeric_type = "Digit"
            numeric_value = int(num7)
        elif num8:
            numeric_type = "Numeric"
            numeric_value = float(Fraction(num8))
        chars[code] = UnicodeDataEntry(
            code=code,
            name=name,
            general_category=general_category,
            numeric_type=numeric_type,
            numeric_value=numeric_value,
            simple_lowercase_mapping=lower,
            simple_uppercase_mapping=upper,
            simple_titlecase_mapping=title
        )
    return chars


def make_core_properties(file):
    chars = {}

    for rng, *prop in data_lines(file, False):
        if ".." in rng:
            start, end = rng.split("..")
            rng = range(code_point(start), code_point(end) + 1)
        else:
            rng = code_point(rng)
            rng = range(rng, rng + 1)

        for ch in rng:
            match prop:
                case ["Lowercase"]:    prop_value = MASK_LOWERCASE
                case ["Uppercase"]:    prop_value = MASK_UPPERCASE
                case ["Cased"]:        prop_value = MASK_CASED
                case ["Alphabetic"]:   prop_value = MASK_ALPHABETIC
                case ["XID_Start"]:    prop_value = MASK_XID_START
                case ["XID_Continue"]: prop_value = MASK_XID_CONTINUE
                case _: continue
            chars[ch] = chars.get(ch, 0) | prop_value
    return chars


def make_special_casing(file) -> dict[int, SpecialCasingEntry]:
    chars = {}
    for code, lower, upper, title, *conditions in data_lines(file, True):
        if len(conditions) != 0:
            continue
        code = code_point(code)
        chars[code] = SpecialCasingEntry(
            code=code,
            lower=[code_point(cp) for cp in lower.split()],
            upper=[code_point(cp) for cp in upper.split()],
            title=[code_point(cp) for cp in title.split()]
        )
    return chars


def make_character_props(data, core_properties, special_casing, special_casing_list):
    code = data.code
    match data.numeric_type:
        case "Decimal": core_properties |= MASK_DECIMAL
        case "Digit":   core_properties |= MASK_DIGIT
        case "Numeric": core_properties |= MASK_NUMERIC

    if data.general_category[0] in "LNPS" or data.general_category == "Zs":
        core_properties |= MASK_PRINTABLE

    if special_casing is not None:
        lower = special_casing.lower
        upper = special_casing.upper
        title = special_casing.title
    else:
        lower = [data.simple_lowercase_mapping]
        upper = [data.simple_uppercase_mapping]
        title = [data.simple_titlecase_mapping]

    upper = add_special_casing(code, upper, special_casing_list)
    lower = add_special_casing(code, lower, special_casing_list)
    title = add_special_casing(code, title, special_casing_list)
    return lower, upper, title, core_properties


def add_special_casing(code, casing, special_casing_list):
    if len(casing) == 1:
        return casing[0] - code
    if len(casing) > 255:
        raise ValueError("cannot encode special casings that expand to more than 255 characters")
    idx = len(special_casing_list)
    special_casing_list.append(casing[0] | ((len(casing) & 0xFF) << 24))
    special_casing_list.extend(casing[1:])
    assert idx + 0xFF_FF_FF < 0xFF_FF_FF_FF
    return idx + 0xFF_FF_FF


def check_best_shift(unicode_data, prop_index_byte_size, min_shift=4, max_shift=10):
    # Total memory occupied:
    #
    # tot_size = sizeof(UnicodeChBlocksIdx) + sizeof(UnicodeChBlocks)
    #
    # sizeof(UnicodeChBlocksIdx) =    number_of_indices    *    index_byte_size
    #                            = ((0x10FFFF>>shift) + 1) * byte_size(len(blocks))
    #
    # size(UnicodeChBlocks) = property_indices_in_one_table * prop_index_byte_size * number_of_tables
    #                       =            2**shift           * prop_index_byte_size *   len(blocks)
    #
    # The memory occupied by _xcUnicodeChInfo and _xcUnicodeCaseExpansion is not taken into account as it does
    # not depend on SHIFT

    if min_shift > max_shift:
        raise ValueError(f"invalid shift range [{min_shift}, {max_shift}]")

    print(f"Finding best SHIFT in range [{min_shift}, {max_shift}]")

    best_shift = min_shift
    best_blocks_size = 0
    for i in range(max_shift - min_shift + 1):
        blocks = set()
        shift = i + min_shift
        for ch in unicode_data:
            blocks.add(ch >> (i + min_shift))
        block_count = len(blocks)
        tot_size = ((0x10FFFF>>shift) + 1) * byte_size(block_count) + 2**shift * prop_index_byte_size * block_count
        if i == 0:
            best_blocks_size = tot_size
        elif tot_size < best_blocks_size:
            best_blocks_size = tot_size
            best_shift = shift
        print(f"    SHIFT: {shift}, blocks: {len(blocks)}, total size: {tot_size}")

    print(f"Best shift: SHIFT={best_shift}\n")
    return best_shift


def byte_size(n):
    if n <= 255:
        return 1
    elif n <= 65535:
        return 2
    else:
        return 4


def type_from_size(n):
    if n == 1:
        return "u8"
    elif n == 2:
        return "u16"
    else:
        return "u32"


def generate_file(blocks, block_table, shift, shift_mask, special_casing_list, props):
    max_expansion_len = max(i >> 24 for i in special_casing_list)

    unicode_ch_blocks_data = []
    for block in blocks.values():
        block_data = []
        for i in range(0, shift_mask + 1):
            if i in block:
                block_data.append(block[i])
            else:
                block_data.append(0)
        unicode_ch_blocks_data.append(block_data)

    unicode_ch_blocks_idx_data = []
    for ch in range((0x10FFFF >> shift) + 1):
        if ch in block_table:
            unicode_ch_blocks_idx_data.append(block_table[ch])
        else:
            unicode_ch_blocks_idx_data.append(len(blocks) + 1)

    print(f"#define XC_MAX_CASE_EXPANSION {max_expansion_len}")

    with UnicodeDBWriter(DB_PATH, None) as db_f:
        db_f.write_code(f"""
            #include <xc.h>

            #define _XC_UCD_SHIFT {shift}
            #define _XC_UCD_MAX_BLOCK_IDX {0x10FFFF >> shift}

            """
        )

        db_f.write_array1d(
            "const XCUnicodeChInfo",
            "_xcUnicodeChInfo",
            [f"{{ {l}, {u}, {t}, {f} }}" for l, u, t, f in props],
            max_line_len=120
        )

        db_f.write_array1d(
            "const u32",
            "_xcUnicodeCaseExpansion",
            special_casing_list,
            max_line_len=120
        )

        unicode_ch_blocks_type = type_from_size(byte_size(len(props)))
        unicode_ch_blocks_idx_type = type_from_size(byte_size(len(blocks) + 1))

        db_f.write_array2d(
            f"const {unicode_ch_blocks_type}",
            "_xcUnicodeChBlocks",
            unicode_ch_blocks_data,
            format="3d",
            max_line_len=120
        )

        db_f.write_array1d(
            f"const {unicode_ch_blocks_idx_type}",
            "_xcUnicodeChBlocksIdx",
            unicode_ch_blocks_idx_data,
            format="3d",
            max_line_len=120
        )

        db_f.write_code(f"""
            XCUnicodeChInfo xcUnicodeGetChInfo(u32 codepoint) {{
                if (codepoint >> _XC_UCD_SHIFT > _XC_UCD_MAX_BLOCK_IDX) {{
                    return _xcUnicodeChInfo[0];
                }}
                {unicode_ch_blocks_idx_type} blockIdx = _xcUnicodeChBlocksIdx[codepoint >> _XC_UCD_SHIFT];
                {unicode_ch_blocks_type} chInfoIdx = _xcUnicodeChBlocks[blockIdx][codepoint & ((1 << _XC_UCD_SHIFT) - 1)];
                return _xcUnicodeChInfo[chInfoIdx];
            }}

            usize xcUnicodeExpandCase(u32 codepoint, u32 casing, u32 *outCodepoints) {{
                if (casing <= 0x10FFFF) {{
                    if (outCodepoints != NULL)
                        *outCodepoints = codepoint + casing;
                    return 1;
                }}
                u32 first_cp = _xcUnicodeCaseExpansion[casing - 0xFFFFFF];
                usize size = first_cp >> 24;

                if (outCodepoints == NULL)
                    return size;

                *outCodepoints++ = first_cp & 0xFFFFFF;
                for (usize i = 1; i < size; i++) {{
                    *outCodepoints++ = _xcUnicodeCaseExpansion[casing - 0xFFFFFF + i];
                }}
                return size;
            }}"""
        )


def main():
    print(f"UCD_DIR: {os.getcwd()}/{UCD_DIR}")
    print(f"DB_PATH: {os.getcwd()}/{DB_PATH}\n")

    with open(f"{UCD_DIR}/UnicodeData.txt", encoding="utf8") as f:
        unicode_data = make_unicode_data(f)

    with open(f"{UCD_DIR}/SpecialCasing.txt", encoding="utf8") as f:
        special_casing = make_special_casing(f)

    with open(f"{UCD_DIR}/DerivedCoreProperties.txt", encoding="utf8") as f:
        core_properties = make_core_properties(f)

    chars = {}
    special_casing_list = []
    props = set()

    for ch, data in unicode_data.items():
        ch_props = make_character_props(data, core_properties.get(ch, 0), special_casing.get(ch), special_casing_list)
        chars[ch] = ch_props
        props.add(ch_props)

    props = [(0, 0, 0, 0)] + list(props)
    props_table = {prop: i for i, prop in enumerate(props)}
    shift = check_best_shift(unicode_data, byte_size(len(props)))
    shift_mask = (1<<shift) - 1

    blocks = {}
    block_table = {}
    idx = 0

    for ch, ch_props in chars.items():
        block_key = ch >> shift
        if block_key not in blocks:
            block = {}
            blocks[block_key] = block
            block_table[block_key] = idx
            idx += 1
        else:
            block = blocks[block_key]
        block[ch & shift_mask] = props_table[ch_props]

    generate_file(blocks, block_table, shift, shift_mask, special_casing_list, props)


if __name__ == "__main__":
    main()
