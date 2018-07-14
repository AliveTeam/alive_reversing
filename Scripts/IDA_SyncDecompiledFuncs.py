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

def set_stubbed(function_address):
    idc.set_color(function_address, CIC_FUNC, 0x5EDAFA)

def is_stubbed(function_address):
    function = idaapi.get_func(function_address)
    if not function:
        return False
    return function.color == 0x5EDAFA

def func_exists(function_address):
    for segment in Segments():
        # get all functions
        for function_ea in Functions(segment, SegEnd(segment)):
            if function_address == function_ea:
                return True
    return False

def asHex(value):
    return (hex(value).rstrip("L") or "0").upper()

def set_func_colors(funcs, isStubs):
  for func in funcs:
        func = func.strip();
        func = long(func)
        if (func_exists(func)):
            if isStubs:
                if is_stubbed(func) == False:
                    print "Marking " + asHex(func) + " as stubbed"
                    set_stubbed(func)
                else:
                    print asHex(func) + " already marked as stubbed"
            else:
                if is_decompiled(func) == False:
                    print "Marking " + asHex(func) + " as decompiled"
                    set_decompiled(func)
                else:
                    print asHex(func) + " already marked as decompiled"
        else:
            print asHex(func) + " function not found in IDB!"

def main():
    with open('C:\GOG Games\Abes Exoddus\decompiled_functions.txt', 'r') as f:
        decompiledFuncs = [line for line in f]

    set_func_colors(decompiledFuncs, False)

    with open('C:\GOG Games\Abes Exoddus\stubbed_functions.txt', 'r') as f:
        stubbedFuncs = [line for line in f]

    set_func_colors(stubbedFuncs, True)

if __name__ == '__main__':
    main()
