from ._stuff import *

import collections


def lcd4(
    *,
    prefix,
    data_port,
    data_pins4567,
    e_port,
    e_pin,
    rs_port,
    rs_pin,
    header=True,
):
    dico = collections.OrderedDict(
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
        }
    )
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

    if header:
        return protect_header(f"LCD4_{prefix}", content)
    return content
