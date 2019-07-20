from ._stuff import *

import itertools
import collections


def buttons(*, prefix, desc, interrupt=0, header=True):
    port = set(p.port for p in itertools.chain(*(x.values() for x in desc.values())) if p is not None)
    assert len(port) == 1
    port = port.pop()

    constants = {"BUTTONS_PORT": port}
    enums = {}
    all_bm = []
    all_pinctrl = []
    for btnname,btndesc in desc.items():
        values_enum = enums[f"BUTTONS_{btnname}"] = []
        for btnstate,pindesc in btndesc.items():
            if pindesc is not None:
                bm = f"BUTTONS_{btnname}_{btnstate}_bm"
                pinctrl = f"BUTTONS_{btnname}_{btnstate}_pinctrl"
                all_bm.append(bm)
                all_pinctrl.append(pinctrl)
                constants[bm] = pindesc.pin_bm
                constants[pinctrl] = pindesc.pinctrl
            values_enum.append(f"BUTTONS_{btnname}_{btnstate}_val")

    constants["BUTTONS_INT_vect"] = f"{port}_INT{interrupt}_vect"
    constants["BUTTONS_INTMASK"] = f"INT{interrupt}MASK"
    constants["BUTTONS_INTLVL_gm"] = f"PORT_INT{interrupt}LVL_gm"
    constants["BUTTONS_INTLVL_level_gc"] = f"PORT_INT{interrupt}LVL_LO_gc"
    
    port = "BUTTONS_PORT"
    content = ""

    ## states
    if not header:
        for btnname in desc.keys():
            content += f"volatile BUTTONS_{btnname}_t buttons_{btnname.lower()}_state;\n"
        content += "\n"

    ## buttons init
    content += "void buttons_init()"
    if header:
        content += ";\n"
    else:
        content += "\n{\n"
        content += f"    {port}.DIRCLR = " + ' | '.join(all_bm) + ';\n'
        for pinctrl in all_pinctrl:
            content += f"    {pinctrl} = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;\n"
        content += "\n"
        for btnname in desc.keys():
            content += f"    buttons_{btnname.lower()}_state = buttons_read_state_{btnname.lower()}();\n"
        content += f"    {port}.BUTTONS_INTMASK = " + ' | '.join(all_bm) + ';\n'
        content += f"    {port}.INTCTRL = ({port}.INTCTRL & ~BUTTONS_INTLVL_gm) | BUTTONS_INTLVL_level_gc;\n"
        content += "}\n\n"

    ## interrupt
    if not header:
        content += f"ISR(BUTTONS_INT_vect)\n"
        content += "{\n"
        for btnname in desc.keys():
            content += f"    BUTTONS_{btnname}_t buttons_{btnname.lower()}_new_state = buttons_read_state_{btnname.lower()}();\n"
        for btnname in desc.keys():
            content += f"    if(buttons_{btnname.lower()}_new_state != buttons_{btnname.lower()}_state)\n"
            content += "    {\n"
            content += f"        buttons_{btnname.lower()}_state = buttons_{btnname.lower()}_new_state;\n"
            content += f"        buttons_{btnname.lower()}_handler(buttons_{btnname.lower()}_state);\n"
            content += "    }\n"
        content += "}\n\n"

    ## buttons state readers
    for btnname,btndesc in desc.items():
        content += f"BUTTONS_{btnname}_t buttons_read_state_{btnname.lower()}()"
        if header:
            content += ";\n"
        else:
            content += "\n{\n"
            default_btnstate = [btnstate for btnstate,pindesc in btndesc.items() if pindesc is None][0]
            content += f"    BUTTONS_{btnname}_t ret;\n"
            content += "   "
            for btnstate,pindesc in btndesc.items():
                if pindesc is None:
                    continue
                content += f" if(!({port}.IN & BUTTONS_{btnname}_{btnstate}_bm))\n"
                content += f"        ret = BUTTONS_{btnname}_{btnstate}_val;\n"
                content += f"    else"
            content += "\n"
            content += f"        ret = BUTTONS_{btnname}_{default_btnstate}_val;\n"
            content += "    return ret;\n"
            content += "}\n\n"

    ## handler (header only)
    if header:
        content += "\n"
        for btnname in desc.keys():
            content += f"void buttons_{btnname.lower()}_handler(BUTTONS_{btnname}_t state);\n"

    content = content.replace("buttons_", f"{prefix}_")
    filtered = content.splitlines()
    while filtered and not filtered[0]:
        del filtered[0]
    while filtered and not filtered[-1]:
        del filtered[-1]
    content = "\n".join(filtered)
    defined_enums, content = generate_enums("BUTTONS", prefix, enums, content)
    defined, content = generate_define("BUTTONS", prefix, constants, content)

    if header:
        return protect_header(f"BUTTONS_{prefix}", defined_enums + "\n" + defined + "\n" + content)
    return content
