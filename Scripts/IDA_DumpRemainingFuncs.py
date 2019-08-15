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
            instructions = len(list(FuncItems(funcea)))

            # is decompiled?
            if ida_get_function_colour(funcea) == 0xEEFFF0:
                continue # ignore it

            # is lib function?
            if GetFunctionFlags(funcea) & FUNC_LIB:
                # ignore it
                continue

            totalFuncs = totalFuncs + 1
            totalInstructions = totalInstructions + instructions
            print str(funcea) + " = " + functionName

    print "total funcs " + str(totalFuncs) + " insts " + str(totalInstructions)

if __name__ == '__main__':
    main()
