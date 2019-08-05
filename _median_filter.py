from ._stuff import *

import collections


def median_filter(
    *, prefix, data_type, idx_type, filter_len, header=True
):
    dico = {}
    dico["MFPREFIX"] = prefix
    dico["MFtype"] = f"{data_type}"
    dico["MFidxtype"] = f"{idx_type}"

    filename = LIBDIR + "/median_filter." + ("h" if header else "c")
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
            "MF_len": f"{filter_len}",
        }
    )

    defined, content = generate_define("MF", prefix, constants, content)

    if header:
        return protect_header(f"MF_{prefix}", defined + "\n" + content)
    return content
