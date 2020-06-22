import os
print os.getcwd()

className = "test"

cmakePath = os.getcwd() + "\..\Source\AliveLibAO\CMakeLists.txt"
print(cmakePath)

startTag = "SET(AliveLibSrcAO"

endTag = ")"

f = open(cmakePath, "r")
content = f.read()
f.close()

pos = content.find("SET(AliveLibSrcAO")
if pos == -1:
    print("Can't find start tag")

before = content[:pos+17]
after = content[pos+18:]

f = open(cmakePath, "w")
f.write(before + "\n")
f.write("   " + className + ".hpp\n")
f.write("   " + className + ".cpp\n")
f.write(after)
f.close()

h = open(os.getcwd() + "\\..\\Source\\AliveLibAO\\" + className + ".hpp", "w")
h.write("""#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

END_NS_AO

""")
h.close()

c = open(os.getcwd() + "\\..\\Source\\AliveLibAO\\" + className + ".cpp", "w")
c_data = """#include "stdafx_ao.h"
#include "Function.hpp"
#include "replace_me.hpp"

START_NS_AO

END_NS_AO

"""
c.write(c_data.replace("replace_me.hpp", className + ".hpp"))
c.close()
         
print("cheers lads")
