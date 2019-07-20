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

def generate_enums(orig_prefix, prefix, enums, content):
    to_subtitute = []
    defined = ""
    for enumname, enumvalues in enums.items():
        prefixed_enumname = enumname.replace(orig_prefix, prefix.upper())
        defined += f"typedef enum {prefixed_enumname}_enum\n"
        defined += "{\n"
        for value in enumvalues:
            prefixed_value = value.replace(orig_prefix, prefix.upper())
            defined += f"    {prefixed_value},\n"
            to_subtitute.append((value, prefixed_value))
        defined += "} " + f"{prefixed_enumname}_t;\n\n"
        to_subtitute.append((f"{enumname}_enum", f"{prefixed_enumname}_enum"))
        to_subtitute.append((f"{enumname}_t", f"{prefixed_enumname}_t"))
    for old,new in to_subtitute:
        content = content.replace(old,new)
    return defined, content
