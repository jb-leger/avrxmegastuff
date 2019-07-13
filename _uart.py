from ._stuff import *

import collections


def uart(
    *,
    prefix,
    name,
    txpin,
    header=True,
    tx=False,
    rx=False,
    txlen=64,
    rxlen=64,
    baudrate=9600,
    char_size=8,
    rxtrigger="\n",
    rxhandler=None,
):
    if rx and rxhandler is None:
        raise Exception
    if not rx and rxhandler is not None:
        raise Exception
    if not rx and not tx:
        raise Exception
    dico = collections.OrderedDict()

    if rxhandler is not None:
        dico["UART_rxhandler"] = rxhandler

    dico["USARTPREFIX"] = prefix

    filename = LIBDIR + "/uart." + ("h" if header else "c")
    with open(filename) as f:
        content = f.read()

    for k, v in dico.items():
        content = content.replace(k, v)

    filtered = []
    intx, inrx = False, False
    for l in content.splitlines():
        if l.startswith("#endif"):
            inrx, intx = False, False
        elif l.startswith("#if rx"):
            inrx = True
        elif l.startswith("#if tx"):
            intx = True
        else:
            if not inrx and not intx:
                filtered.append(l)
            elif inrx and rx:
                filtered.append(l)
            elif intx and tx:
                filtered.append(l)

    while filtered and not filtered[0]:
        del filtered[0]
    while filtered and not filtered[-1]:
        del filtered[-1]

    content = "\n".join(filtered)

    constants = collections.OrderedDict(
        {
            "UART_port": f"PORT{name[0]}",
            "UART_txpin": f"PIN{txpin}_bm",
            "UART_rxlen": f"{rxlen}",
            "UART_txlen": f"{txlen}",
            "UART_baudrate": f"{baudrate}",
            "UART_chsize": f"{char_size}",
            "UART_rxtrigger": "'" + rxtrigger.encode("unicode_escape").decode() + "'",
            "UART_rxen": "true" if rx else "false",
            "UART_txen": "true" if tx else "false",
            "UART_USART": f"USART{name}",
            "UART_DRE_vect": f"USART{name}_DRE_vect",
            "UART_RXC_vect": f"USART{name}_RXC_vect",
        }
    )

    defined, content = generate_define("UART", prefix, constants, content)

    if header:
        return protect_header(f"UART_{prefix}", defined + "\n" + content)
    return content
