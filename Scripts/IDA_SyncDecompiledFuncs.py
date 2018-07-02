from idautils import *
from idaapi import *
from idc import *

def set_decompiled(function_address):
    idc.set_color(function_address, CIC_FUNC, 0xEEFFF0)

def is_decompiled(function_address):
    function = idaapi.get_func(function_address)
    if not function:
        return False
    return function.color == 0xEEFFF0

def func_exists(function_address):
    for segment in Segments():
        # get all functions
        for function_ea in Functions(segment, SegEnd(segment)):
            if function_address == function_ea:
                return True
    return False

def asHex(value):
    return (hex(value).rstrip("L") or "0").upper()

def main():
    with open('C:\GOG Games\Abes Exoddus\decompiled_functions.txt', 'r') as f:
        funcs = [line for line in f]

    for func in funcs:
        func = func.strip();
        func = long(func)
        if (func_exists(func)):
            if is_decompiled(func) == False:
                print "Marking " + asHex(func) + " as decompiled"
                set_decompiled(func)
            else:
                print asHex(func) + " already marked as decompiled"
        else:
            print asHex(func) + " function not found in IDB!"

if __name__ == '__main__':
    main()
