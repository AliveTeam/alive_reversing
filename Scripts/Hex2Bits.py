import math

# LOBYTE = self
# BYTE1 = + 00
# BYTE2 = + 0000

num = 6
numAsBinaryString = bin(num)[2:]

fp = 0x80000

print "Converting " + hex(num) + " bits " + numAsBinaryString

for idx, zeroOrOne in enumerate(numAsBinaryString):
    if zeroOrOne == '1':
        splitBits = '0b1' + "0" * (len(numAsBinaryString) - 1 - idx)
        print hex(int(splitBits, 2))
print "Done"

print ""

print "Converting " + hex(fp) + " to fixed point calc"

fpV = (fp / float(0x10000))
if (math.floor(fpV) == fpV):
    print "FP_FromInteger(" + str(int(fpV)) + ");"
else:
    print "FP_FromDouble(" + str(fpV) + ");"
    
#print hex(1<<21)
