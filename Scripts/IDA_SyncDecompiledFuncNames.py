from idautils import *
from idaapi import *
from idc import *

def ida_func_exists(function_address):
    for segment in Segments():
        # get all functions
        for function_ea in Functions(segment, SegEnd(segment)):
            if function_address == function_ea:
                return True
    return False

def LineToInt(line):
    line = line.strip();
    return long(line)

def EnsureKey(address, dict):
    if not dict.has_key(address):
        dict[address] = ""

# line will be a string of the form 4393888=Fleech::AI_0_430BA0
def AddFunction(line, dict):
    address = LineToInt(line.split('=')[0])
    EnsureKey(address, dict)
    funcName = line.split('=')[1].strip()
    # Remove AO::
    pos = funcName.find("::")
    if pos != -1:
        funcName = funcName[pos+2:]
    dict[address] = funcName

def sync_function_name(adddress, name):
    if ida_func_exists(adddress):
        currentName = GetFunctionName(adddress)
        if currentName != name:
            print "rename: " + currentName + " to " + name
            idc.MakeNameEx(adddress, name, idc.SN_NOWARN)
 
def main():
    functionDataDict = {}

    with open('C:\\GOG Games\\Abes Oddysee\\function_names.txt', 'r') as f:
        for line in f:
            AddFunction(line, functionDataDict)

    for address in functionDataDict.iterkeys():
        data = functionDataDict[address]
        sync_function_name(address, data)

if __name__ == '__main__':
    main()
