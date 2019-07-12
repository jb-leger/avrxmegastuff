import os
LIBDIR = os.sep.join(__file__.split(os.sep)[:-1]+['libs'])

def protect_header(name, s):
    return f'#ifndef H_{name}\n#define H_{name} 1\n\n{s}\n#endif'
