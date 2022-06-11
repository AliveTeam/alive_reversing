import math
import sys

# LOBYTE = self
# BYTE1 = + 00
# BYTE2 = + 0000

def to_enum(value):
    match value:
        case 0:
            return "eFloor_0 | eDynamicCollision_32"
        case 1:
            return "eWallLeft_1"
        case 2:
            return "eWallRight_2"
        case 3:
            return "eCeiling_3"
        case 4:
            return "eBackgroundFloor_4 | eBackgroundDynamicCollision_36"
        case 5:
            return "eBackgroundWallLeft_5"
        case 6:
            return "eBackgroundWallRight_6"
        case 7:
            return "eBackgroundCeiling_7"
        case 8:
            return "eTrackLine_8"
        case 9:
            return "eArt_9"
        case 10:
            return "eBulletWall_10"
        case 11:
            return "eMineCarFloor_11"
        case 12:
            return "eMineCarWall_12"
        case 13:
            return "eMineCarCeiling_13"
        case 14:
            return "eBackgroundMineCarFloor_14"
        case 15:
            return "eBackgroundMineCarWall_15"
        case 16:
            return "eBackgroundMineCarCeiling_16"
        case 17:
            return "eFlyingSligCeiling_17"
        case 18:
            return "eBackgroundFlyingSligCeiling_18"
    print("DEAD on " + str(value))

 
def twos_complement(value, bitWidth):
    if value >= 2**bitWidth:
        # This catches when someone tries to give a value that is out of range
        raise ValueError("Value: {} out of range of {}-bit value.".format(value, bitWidth))
    else:
        return value - int((value << 1) & 2**bitWidth)

if (len(sys.argv) != 2):
    print("Usage: {sys.argv[0]} [number]")
    #sys.exit(0)

input = 0x20000 
numAsBinaryString = bin(input)[2:]

print("2's comp: " + str(twos_complement(input, 32)))

print("Converting " + hex(input) + " bits " + numAsBinaryString)

for idx, zeroOrOne in enumerate(numAsBinaryString):
    if zeroOrOne == '1':
        splitBits = '0b1' + "0" * (len(numAsBinaryString) - 1 - idx)
        print(hex(int(splitBits, 2)) + " bit idx " + str(len(numAsBinaryString) - 1 - idx) + " " + to_enum(len(numAsBinaryString) - 1 - idx))
print("Done")

print("")

print("Converting " + hex(input) + " to fixed point calc")

fpV = (input / float(0x10000))
if (math.floor(fpV) == fpV):
    print("FP_FromInteger(" + str(int(fpV)) + ");")
else:
    print("FP_FromDouble(" + str(fpV) + ");")
