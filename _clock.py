from ._stuff import *

def clock32M(header=True):
    filename = LIBDIR + '/clock32M.' + ('h' if header else 'c')
    with open(filename) as f:
        content = f.read()
    if header:
        return protect_header('clock32M', content)
    return content
