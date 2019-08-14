import math
import sys

# LOBYTE = self
# BYTE1 = + 00
# BYTE2 = + 0000

def twos_complement(value, bitWidth):
    if value >= 2**bitWidth:
        # This catches when someone tries to give a value that is out of range
        raise ValueError("Value: {} out of range of {}-bit value.".format(value, bitWidth))
    else:
        return value - int((value << 1) & 2**bitWidth)

if (len(sys.argv) != 2):
    print(f"Usage: {sys.argv[0]} [number]")
    sys.exit(0)

input = int(sys.argv[1], 0)
numAsBinaryString = bin(input)[2:]

print("2's comp: " + str(twos_complement(input, 32)))

print("Converting " + hex(input) + " bits " + numAsBinaryString)

for idx, zeroOrOne in enumerate(numAsBinaryString):
    if zeroOrOne == '1':
        splitBits = '0b1' + "0" * (len(numAsBinaryString) - 1 - idx)
        print(hex(int(splitBits, 2)))
print("Done")

print("")

print("Converting " + hex(input) + " to fixed point calc")

fpV = (input / float(0x10000))
if (math.floor(fpV) == fpV):
    print("FP_FromInteger(" + str(int(fpV)) + ");")
else:
    print("FP_FromDouble(" + str(fpV) + ");")