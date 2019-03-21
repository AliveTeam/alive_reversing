import math

# LOBYTE = self
# BYTE1 = + 00
# BYTE2 = + 0000

def twos_complement(value, bitWidth):
    if value >= 2**bitWidth:
        # This catches when someone tries to give a value that is out of range
        raise ValueError("Value: {} out of range of {}-bit value.".format(value, bitWidth))
    else:
        return value - int((value << 1) & 2**bitWidth)
    
num = 65534
numAsBinaryString = bin(num)[2:]

# 0x1CCCC =1.8
fp = 0x140000

print "2's comp: " + str(twos_complement(num, 16))

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
