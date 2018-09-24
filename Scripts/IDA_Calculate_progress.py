from idautils import *
from idaapi import *
from idc import *
import urllib2

def ida_get_function_colour(function_address):
    function = idaapi.get_func(function_address)
    if not function:
        return 0
    return function.color

def downloadFile(url):
    response = urllib2.urlopen(url)
    html = response.read()
    return html

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

    totalLibFuncs = 0
    totalLibFuncsInstructions = 0

    totalDecompiledFuncs = 0
    totalDecompiledInstructions = 0
    
    funcsToImplement = toAddressList(downloadFile("https://gist.githubusercontent.com/paulsapps/71951b013063e30e675151da8958a2a7/raw/12635e56aed832d73890b65025e97ffd2fee054b/gistfile1.txt"))
    totalTodoFuncs = 0
    totalTodoInstructions = 0

    totalTodoDoneFuncs = 0
    totalTodoDoneInstructions = 0

    for segea in Segments():
        for funcea in Functions(segea, SegEnd(segea)):
            functionName = GetFunctionName(funcea)
            instructions = len(list(FuncItems(funcea)))

            totalFuncs = totalFuncs + 1
            totalInstructions = totalInstructions + instructions

            # is decompiled?
            if ida_get_function_colour(funcea) == 0xEEFFF0:
                totalDecompiledFuncs = totalDecompiledFuncs + 1
                totalDecompiledInstructions = totalDecompiledInstructions + instructions

            # is lib function?
            if GetFunctionFlags(funcea) & FUNC_LIB:
                totalLibFuncs = totalLibFuncs + 1
                totalLibFuncsInstructions = totalLibFuncsInstructions + instructions

            # Only count towards required impl if not already impl'd and isn't marked as lib func
            if funcea in funcsToImplement:
                if ida_get_function_colour(funcea) == 0xEEFFF0:
                    totalTodoDoneFuncs = totalTodoDoneFuncs + 1
                    totalTodoDoneInstructions = totalTodoDoneInstructions + instructions
                    totalTodoFuncs = totalTodoFuncs + 1
                    totalTodoInstructions = totalTodoInstructions + instructions
                    continue
                
                if GetFunctionFlags(funcea) & FUNC_LIB:
                    print functionName + " has been marked as a lib function, skipping"
                    continue

                totalTodoFuncs = totalTodoFuncs + 1
                totalTodoInstructions = totalTodoInstructions + instructions

    print "total funcs " + str(totalFuncs) + " insts " + str(totalInstructions)
    print "total lib funcs " + str(totalLibFuncs) + " insts " + str(totalLibFuncsInstructions)
    print "total decomp funcs " + str(totalDecompiledFuncs) + " insts " + str(totalDecompiledInstructions)

    totalFuncsNoLibs = totalFuncs - totalLibFuncs
    totalInstructionsNoLibs = totalInstructions - totalLibFuncsInstructions

    print "total without lib funcs " + str(totalFuncsNoLibs) + " insts " + str(totalInstructionsNoLibs)

    print "total todo funcs " + str(totalTodoFuncs) + " insts " + str(totalTodoInstructions)

    print "overall complete funcs %.2f" % ((float(totalDecompiledFuncs) / float(totalFuncsNoLibs)) * 100.0) + "% (" + str(totalDecompiledFuncs) + "/" + str(totalFuncsNoLibs) + ")"
    print "overall complete insts %.2f" % ((float(totalDecompiledInstructions) / float(totalInstructionsNoLibs)) * 100.0) + "% (" + str(totalDecompiledInstructions) + "/" + str(totalInstructionsNoLibs) + ")"
    
    print "next milestone complete funcs %.2f" % ((float(totalTodoDoneFuncs) / float(totalTodoFuncs)) * 100.0) + "% (" + str(totalTodoDoneFuncs) + "/" + str(totalTodoFuncs) + ")"
    print "next milestone complete insts %.2f" % ((float(totalTodoDoneInstructions) / float(totalTodoInstructions)) * 100.0) + "% (" + str(totalTodoDoneInstructions) + "/" + str(totalTodoInstructions) + ")"

    #for func in funcsToImplement:
    #    print "required func is " + func

if __name__ == '__main__':
    main()
