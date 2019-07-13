from ._stuff import *

import collections

def ds18b20(
    *,
    prefix,
    port,
    pin,
    header=True,
):
    dico = collections.OrderedDict()
    dico["_DS"] = prefix


    filename = LIBDIR + "/ds18b20." + ("h" if header else "c")
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

    constants = collections.OrderedDict({
        "DS18B20_port": port,
        "DS18B20_pin": f"PIN{pin}_bm",
        'DS18B20_CMD_CONVERTTEMP': '0x44',
        'DS18B20_CMD_RSCRATCHPAD': '0xbe',
        'DS18B20_CMD_WSCRATCHPAD': '0x4e',
        'DS18B20_CMD_CPYSCRATCHPAD': '0x48',
        'DS18B20_CMD_RECEEPROM': '0xb8',
        'DS18B20_CMD_RPWRSUPPLY': '0xb4',
        'DS18B20_CMD_SEARCHROM': '0xf0',
        'DS18B20_CMD_READROM': '0x33',
        'DS18B20_CMD_MATCHROM': '0x55',
        'DS18B20_CMD_SKIPROM': '0xcc',
        'DS18B20_CMD_ALARMSEARCH': '0xec',
        }
        )

    defined, content = generate_define("DS18B20", prefix, constants, content)

    if header:
        return protect_header(f"DS18B20_{prefix}", defined + "\n" + content)
    return content
