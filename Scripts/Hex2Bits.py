num = 0xC000
numAsBinaryString = bin(num)[2:]

print "Converting " + hex(num) + " bits " + numAsBinaryString

for idx, zeroOrOne in enumerate(numAsBinaryString):
    if zeroOrOne == '1':
        splitBits = '0b1' + "0" * (len(numAsBinaryString) - 1 - idx)
        print hex(int(splitBits, 2))
print "Done"
