import os
LIBDIR = os.sep.join(__file__.split(os.sep)[:-1]+['libs'])

def protect_header(name, s):
    return f'#ifndef H_{name}\n#define H_{name} 1\n\n{s}\n#endif'

def generate_define(orig_prefix, prefix, constants, content):
    defined = ""
    for cstname, cstvalue in constants.items():
        prefixed_cstname = cstname.replace(orig_prefix, prefix.upper())
        defined += f"#define {prefixed_cstname} {cstvalue}\n"
        content = content.replace(cstname, prefixed_cstname)
    return defined, content
