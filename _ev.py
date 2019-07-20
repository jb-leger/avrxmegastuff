from ._stuff import *

import collections


def ev(
    *, prefix, cmd_open_close_pindesc, sensor_open_close_pindesc, header=True
):
    assert len(cmd_open_close_pindesc)==2
    assert len(sensor_open_close_pindesc)==2
    assert cmd_open_close_pindesc[0].port == cmd_open_close_pindesc[1].port
    assert sensor_open_close_pindesc[0].port == sensor_open_close_pindesc[1].port
    dico = collections.OrderedDict()
    dico["EVPREFIX"] = prefix

    filename = LIBDIR + "/ev." + ("h" if header else "c")
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
            "EV_CMD_PORT": cmd_open_close_pindesc[0].port,
            "EV_CMD_OPEN_bm": cmd_open_close_pindesc[0].pin_bm,
            "EV_CMD_CLOSE_bm": cmd_open_close_pindesc[1].pin_bm,
            "EV_SENSOR_PORT": sensor_open_close_pindesc[0].port,
            "EV_SENSOR_OPEN_bm": sensor_open_close_pindesc[0].pin_bm,
            "EV_SENSOR_CLOSE_bm": sensor_open_close_pindesc[1].pin_bm,
            "EV_SENSOR_OPEN_PINCTRL": sensor_open_close_pindesc[0].pinctrl,
            "EV_SENSOR_CLOSE_PINCTRL": sensor_open_close_pindesc[1].pinctrl,
        }
    )

    defined, content = generate_define("EV", prefix, constants, content)

    if header:
        return protect_header(f"EV_{prefix}", defined + "\n" + content)
    return content
