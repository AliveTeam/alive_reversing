from idautils import *
from idaapi import *
from idc import *
import urllib2

def ida_get_function_colour(function_address):
    function = idaapi.get_func(function_address)
    if not function:
        return 0
    return function.color



def toAddressList(html):
    lines = html.split('\n')
    ret = []
    for line in lines:
        addr = line.split(" ")[0].strip()
        if len(addr) > 0:
            ret.append(int(addr, 16))
    return ret



def main():
    totalFuncs = 0
    totalInstructions = 0
    
    for segea in Segments():
        for funcea in Functions(segea, SegEnd(segea)):
            functionName = GetFunctionName(funcea)
            if functionName.startswith("SEH_"):
                flags = GetFunctionFlags(funcea)
                flags |= FUNC_LIB
                SetFunctionFlags(funcea, flags)

if __name__ == '__main__':
    main()
