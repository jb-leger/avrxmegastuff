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
    dico = collections.OrderedDict(
        {
            "USARTname": name,
            "USARTport": f"PORT{name[0]}",
            "USARTtxpin": f"PIN{txpin}_bm",
            "USARTprefix": prefix,
            "USARTrxlen": f"{rxlen}",
            "USARTtxlen": f"{txlen}",
            "USARTbaudrate": f"{baudrate}",
            "USARTchsize": f"{char_size}",
            "USARTrxtrigger": "'" + rxtrigger.encode("unicode_escape").decode() + "'",
            "USARTrxen": 'true' if rx else 'false',
            "USARTtxen": 'true' if tx else 'false',
        }
    )

    if rxhandler is not None:
        dico["USARTrxhandler"] = rxhandler

    if tx:
        dico["_USART_txbuffer"] = f"{prefix}_txbuffer"
        dico["_USART_txidxA"] = f"{prefix}_txidxA"
        dico["_USART_txidxB"] = f"{prefix}_txidxB"
        dico["_USART_putchar"] = f"{prefix}_putchar"
        dico["_USART_putzchar"] = f"{prefix}_putzchar"
        dico["_USART_putfloat"] = f"{prefix}_putfloat"
        dico["_USART_printf"] = f"{prefix}_printf"

    if rx:
        dico["_USART_rxbuffer"] = f"{prefix}_rxbuffer"
        dico["_USART_rxidx"] = f"{prefix}_rxidx"

    dico["_USART_init"] = f"{prefix}_init"
    dico["_USART"] = f"USART{name}"

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

    if header:
        return protect_header(f"UART_{prefix}", content)
    return content
