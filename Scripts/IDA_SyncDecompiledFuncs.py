from idautils import *
from idaapi import *
from idc import *
import urllib2

def ida_set_function_colour(function_address, colour):
    idc.set_color(function_address, CIC_FUNC, colour)

def ida_get_function_colour(function_address):
    function = idaapi.get_func(function_address)
    if not function:
        return 0
    return function.color

def ida_func_exists(function_address):
    for segment in Segments():
        # get all functions
        for function_ea in Functions(segment, SegEnd(segment)):
            if function_address == function_ea:
                return True
    return False

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
            ret.append(addr)
    return ret

class FunctionData():
    def __init__(self):
        self.bIsImpl = False
        self.bIsStub = False
        self.bIsCovered = False

    def ColourName(self):
        if self.bIsImpl:
            return "Decompiled"
        elif self.bIsStub and self.bIsCovered:
            return "Covered stub"
        elif self.bIsStub:
            return "Stub"
        else:
            return "Covered"

    def Colour(self):
        if self.bIsImpl:
            return 0xEEFFF0 #0xB4DED2
        elif self.bIsStub and self.bIsCovered:
            # Covered and stubbed
            return 0xC57AAF
        elif self.bIsStub:
            # None covered stub
            return 0xD2B4DE
        else:
            # Coverage only case
            return 0xA569BD

def LineToInt(line):
    line = line.strip();
    return long(line)

def EnsureKey(address, dict):
    if not dict.has_key(address):
        dict[address] = FunctionData()

def AddDecompiled(address, dict):
    EnsureKey(address, dict)
    dict[address].bIsImpl = True

def AddStubbed(address, dict):
    EnsureKey(address, dict)
    dict[address].bIsStub = True

def AddCovered(address, dict):
    print "Add covered " + asHex(address)
    EnsureKey(address, dict)
    dict[address].bIsCovered = True

def asHex(value):
    return (hex(value).rstrip("L") or "0").upper()

def sync_function_colour(address, functionData):
    if (ida_func_exists(address)):
        # Everything else is open season
        colourToSet = functionData.Colour()
        if ida_get_function_colour(address) == colourToSet:
            print asHex(address) + " already set to " + functionData.ColourName() + "(" + asHex(colourToSet) + ")"
        else:
            print "Set " + asHex(address) + " to " + functionData.ColourName() + "(" + asHex(colourToSet) + ")"
            ida_set_function_colour(address, colourToSet)
    else:
        print asHex(address) + " function not found in IDB!"


def main():
    functionDataDict = {}

    with open('C:\GOG Games\Abes Exoddus\decompiled_functions.txt', 'r') as f:
        for line in f:
            AddDecompiled(LineToInt(line), functionDataDict)

    with open('C:\GOG Games\Abes Exoddus\stubbed_functions.txt', 'r') as f:
        for line in f:
            AddStubbed(LineToInt(line), functionDataDict)

    funcsWithCoverage = toAddressList(downloadFile("https://gist.githubusercontent.com/paulsapps/ea894a929f02c7bb7c931af12ad08151/raw/38cf5fcd0f8ba6b27a2a08043f81be7f8b34b4e4/gistfile1.txt"))
    for func in funcsWithCoverage:
        print "func is " + func
        AddCovered(int(func, 16), functionDataDict)

    for address in functionDataDict.iterkeys():
        data = functionDataDict[address]
        sync_function_colour(address, data)

if __name__ == '__main__':
    main()
