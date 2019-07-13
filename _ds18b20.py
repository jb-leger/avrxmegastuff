from ._stuff import *

import collections

def ds18b20(
    *,
    prefix,
    port,
    pin,
    header=True,
):
    dico = collections.OrderedDict(
        {
            "DS18B20port": port,
            "DS18B20pin": f"PIN{pin}_bm",
        }
    )
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

    if header:
        return protect_header(f"DS18B20_{prefix}", content)
    return content
