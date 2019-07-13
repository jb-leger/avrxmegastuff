from ._stuff import *

import collections


def lcd4(
    *, prefix, data_port, data_pins4567, e_port, e_pin, rs_port, rs_pin, header=True
):
    dico = collections.OrderedDict()
    dico["LCDPREFIX"] = prefix

    filename = LIBDIR + "/lcd4." + ("h" if header else "c")
    with open(filename) as f:
        content = f.read()

    for k, v in dico.items():
        content = content.replace(k, v)

    filtered = content.splitlines()

    while filtered and not filtered[0]:
        del filtered[0]
    while filtered and not filtered[-1]:
        del filtered[-1]

    content = "\n".join(filtered)

    constants = collections.OrderedDict(
        {
            "LCD_DATA_PORT": data_port,
            "LCD_DATA_PIN4": f"PIN{data_pins4567[0]}_bm",
            "LCD_DATA_PIN5": f"PIN{data_pins4567[1]}_bm",
            "LCD_DATA_PIN6": f"PIN{data_pins4567[2]}_bm",
            "LCD_DATA_PIN7": f"PIN{data_pins4567[3]}_bm",
            "LCD_E_PORT": e_port,
            "LCD_E_PIN": f"PIN{e_pin}_bm",
            "LCD_RS_PORT": rs_port,
            "LCD_RS_PIN": f"PIN{rs_pin}_bm",
            "LCD_CMD_LINEONE": "0x00",
            "LCD_CMD_LINETWO": "0x40",
            "LCD_CMD_CLEAR": "0b00000001",
            "LCD_CMD_HOME": "0b00000010",
            "LCD_CMD_ENTRYMODE": "0b00000110",
            "LCD_CMD_DISPLAYOFF": "0b00001000",
            "LCD_CMD_DISPLAYON": "0b00001100",
            "LCD_CMD_FUNCTIONRESET": "0b00110000",
            "LCD_CMD_FUNCTIONSET4BIT": "0b00101000",
            "LCD_CMD_SETCURSOR": "0b10000000",
        }
    )

    defined, content = generate_define("LCD", prefix, constants, content)

    if header:
        return protect_header(f"LCD4_{prefix}", defined + "\n" + content)
    return content
