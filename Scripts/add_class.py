import os
print os.getcwd()


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

print(before)
print("   test.hpp")
print("   test.cpp")
print(after)

print("cheers lads")
