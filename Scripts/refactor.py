import math
import win32clipboard
import os

anim_enum = {
    0x1 :  "AnimFlags::eBit1",
    0x2 :  "AnimFlags::eBit2_Animate",
    0x4 :  "AnimFlags::eBit3_Render",
    0x8 :  "AnimFlags::eBit4",
    0x10 : "AnimFlags::eBit5_FlipX",
    0x20 : "AnimFlags::eBit6_FlipY",
    0x40 : "AnimFlags::eBit7_SwapXY",
    0x80 : "AnimFlags::eBit8_Loop",
    0x100 : "AnimFlags::eBit9",
    0x200 : "AnimFlags::eBit10_alternating_flag",
    0x400 : "AnimFlags::eBit11_bToggle_Bit10",
    0x800 : "AnimFlags::eBit12_ForwardLoopCompleted",
    0x1000 : "AnimFlags::eBit13_Is8Bit",
    0x2000 : "AnimFlags::eBit14_Is16Bit",
    0x4000 : "AnimFlags::eBit15_bSemiTrans",
    0x8000 : "AnimFlags::eBit16_bBlending",
    0x10000 : "AnimFlags::eBit17",
    0x20000 : "AnimFlags::eBit18_IsLastFrame",
    0x40000 : "AnimFlags::eBit19_LoopBackwards",
    0x80000 : "AnimFlags::eBit20_use_xy_offset",
    0x100000 : "AnimFlags::eBit21",
    0x200000 : "AnimFlags::eBit22_DeadMode",
    0x400000 : "AnimFlags::eBit23",
    0x800000 : "AnimFlags::eBit24",
    0x1000000 : "AnimFlags::eBit25_bDecompressDone",
}

base_game_object_enum = {
    0x1 :  "Options::eListAddFailed_Bit1",
    0x2 :  "Options::eUpdatable_Bit2",
    0x4 :  "Options::eDead_Bit3",
    0x8 :  "Options::eDrawable_Bit4",
    0x10 : "Options::eIsBaseAnimatedWithPhysicsObj_Bit5",
    0x20 : "Options::eIsBaseAliveGameObject_Bit6",
    0x40 : "Options::eCanExplode_Bit7",
    0x80 : "Options::eInteractive_Bit8",
    0x100 : "Options::eSurviveDeathReset_Bit9",
    0x200 : "Options::eUpdateDuringCamSwap_Bit10",
    0x400 : "Options::eCantKill_Bit11",
}

def check(transformed, expected):
    print(transformed)
    assert transformed == expected

def process(line):
    pos = line.find("field_0_VTbl")
    if pos == -1:
        pos = line.find("field_0_VTable")

    if pos != -1:
         parts = line.replace(";", "").split("_")
         addr = parts[len(parts)-1]
         line = "SetVTable(this, 0x" + addr + ");"

    pos = line.find("__thiscall")
    if pos != -1:
        beforeThisCall = line[0: pos]
        afterThisCall = line[pos + 11: line.find("(")]
        args = line[line.find("(") : line.rfind(")")+1]
        if args.find(",") != -1:
            args = "(" + args[args.find(",")+1:].strip()
        else:
            args = "()"
        line = "EXPORT " + beforeThisCall + afterThisCall + args

    line = line.replace("field_0_mBase.", "")
    line = line.replace("&this->field_0_mBase", "this")
    line = line.replace("this->", "")
    line = line.replace("LOBYTE(field_6_flags) |= 4u", "field_6_flags.Set(BaseGameObject::eDead)")

    line = line.replace("1835626049", "ResourceManager::Resource_Animation")
    line = line.replace("0x6D696E41", "ResourceManager::Resource_Animation")
    line = line.replace("'minA'", "ResourceManager::Resource_Animation")

    line = line.replace("'minA'", "ResourceManager::Resource_Animation")

    line = line.replace("Map::TLV_Reset_446870(&gMap_507BA8, ", "gMap_507BA8.TLV_Reset_446870(")

    line = line.replace("field_A8_xpos = 0;", "field_A8_xpos = FP_FromInteger(0);")
    line = line.replace("field_AC_ypos = 0;", "field_AC_ypos = FP_FromInteger(0);")
    line = line.replace("field_B4_velx = 0;", "field_B4_velx = FP_FromInteger(0);")
    line = line.replace("field_B8_vely = 0;", "field_B8_vely = FP_FromInteger(0);")
    line = line.replace("field_BC_sprite_scale = 0;", "field_BC_sprite_scale = FP_FromInteger(0);")

    line = line.replace("field_A8_xpos = 0x10000;", "field_A8_xpos = FP_FromInteger(1);")
    line = line.replace("field_AC_ypos = 0x10000;", "field_AC_ypos = FP_FromInteger(1);")
    line = line.replace("field_B4_velx = 0x10000;", "field_B4_velx = FP_FromInteger(1);")
    line = line.replace("field_B8_vely = 0x10000;", "field_B8_vely = FP_FromInteger(1);")
    line = line.replace("field_BC_sprite_scale = 0x10000;", "field_BC_sprite_scale = FP_FromInteger(1);")

    line = line.replace(" == 0x8000 ", " == FP_FromDouble(0.5) ")
    line = line.replace(" == 0x10000 ", " == FP_FromInteger(1) ")

    line = line.replace("BaseGameObject::ctor_487E10(this, 1)", "ctor_487E10(1)")
    line = line.replace("BaseAnimatedWithPhysicsGameObject::ctor_417C10(this)", "ctor_417C10()")
    line = line.replace("BaseAliveGameObject::ctor_401090(this)", "ctor_401090()")

    line = line.replace("return BaseAliveGameObject::dtor_401000(this)", "return dtor_401000()")
    line = line.replace("return BaseAnimatedWithPhysicsGameObject::dtor_417D10(this)", "return dtor_417D10()")
    line = line.replace("return BaseGameObject::dtor_487DF0(this)", "return dtor_487DF0()")

    pos = line.find("DynamicArray::Remove_Item_404520")
    if pos != -1:
        # todo
        pass

    pos = line.find("DynamicArray::Push_Back_404450")
    if pos != -1:
        # todo
        pass

    pos = line.find("field_10_anim.field_4_flags")
    if pos != -1:
        pos = line.find("&=")
        if pos != -1 and line.find("HI") == -1:
            # Flags are being cleared
            flagsLiteral = line[pos+2 :].strip()
            extractedLit = GetLiteral(flagsLiteral)
            base = 10
            if hasHexChar(extractedLit[0]) or extractedLit[0].startswith("0x"):
                base = 16
            numberAsEnumBits = number2Enum(int(extractedLit[0], base), anim_enum)

            startPos = pos
            endPos = startPos + extractedLit[1]

            lineBefore = line[:startPos]
            lineAfter = line[endPos+3:]
            line = lineBefore.strip() + ".Clear(" + numberAsEnumBits + ")" + lineAfter
            line = line.replace("LOWORD(field_10_anim.field_4_flags)", "field_10_anim.field_4_flags")

    tmp = ""
    pos = line.find("Math_FixedPoint_Multiply_451040")
    if pos != -1:
          tmp = line[pos + 31 :]

    pos = line.find("Math_FixedPoint_Divide_450FB0")
    if pos != -1:
          tmp = line[pos + 29 :]

    if tmp != "":
        # Find the 2nd argument
        pos = tmp.find(",")
        if pos != -1:
            tmp = tmp[pos + 1:]
            pos = tmp.find(")")
            if pos != -1:
                tmp = extractHexOrDecString(tmp.strip())
                base = 10
                if hasHexChar(tmp) or tmp.startswith("0x"):
                    base = 16
                line = line + " // " + AsFP(int(tmp, base))

    return line

def bit_not(n, numbits=8):
    return (1 << numbits) - 1 - n

def extractHexOrDecString(line):
    ret = ""

    hexStrRemoved = False
    if line.startswith("0x"):
        line = line[2:]
        hexStrRemoved = True

    for c in line:
        if c.isspace():
            continue
        elif c.isnumeric():
            ret = ret + c
        elif c in ['A', 'B', 'C', 'D', 'E', 'F']:
            ret = ret + c
        else:
            break

    if hexStrRemoved:
        ret = "0x" + ret
    return ret

def hasHexChar(line):
    isHex = False
    for c in line:
        if c in ['A', 'B', 'C', 'D', 'E', 'F']:
            isHex = True
            break
    return isHex

def GetLiteral(line):
    ret = ""
    strLen = 0
    startPos = 0
    if line.startswith("~"):
        startPos = 1
        strLen = 1
    elif line.startswith("0x"):
        startPos = 2
        strLen = 2

    line = line[startPos:]
    ret = extractHexOrDecString(line)
    strLen = strLen + len(ret)

    if hasHexChar(ret):
        bitcount = int((len(ret) / 2) * 8)
        ret = str(bit_not(int(ret, 16), bitcount))

    if ret != "":
        if line[strLen-1] == 'u':
            strLen = strLen + 1

    return [ret, strLen]

def bits(n):
    while n:
        b = n & (~n+1)
        yield b
        n ^= b

def number2Enum(num, enumVals):
    ret = ""
    for b in bits(num):
        v = enumVals.get(b)
        if ret != "":
            ret = ret + " | "
        ret = ret + v
    return ret

def AsFP(input):
    fpV = (input / float(0x10000))
    if (math.floor(fpV) == fpV):
        return "FP_FromInteger(" + str(int(fpV)) + ");"
    else:
        return "FP_FromDouble(" + str(fpV) + ");"

def tests():
    # TODO
    # Animation::SetFrame_402AC0(&this->field_0_mBase.field_10_anim, (v10 >> 1) + 1);
    # Animation::Get_FrameHeader_403A00(&this->field_0_mBase.field_10_anim, -1);

    check(GetLiteral("123"), ["123", 3])
    check(extractHexOrDecString("0x280000);"), "0x280000")

    check(process("v5 = Math_FixedPoint_Multiply_451040(this->field_0_mBase.field_0_mBase.field_BC_scale, 2621440);"), "v5 = Math_FixedPoint_Multiply_451040(field_BC_scale, 2621440); // FP_FromInteger(40);")
    check(process("v5 = Math_FixedPoint_Multiply_451040(this->field_0_mBase.field_0_mBase.field_BC_scale, 0x280000);"), "v5 = Math_FixedPoint_Multiply_451040(field_BC_scale, 0x280000); // FP_FromInteger(40);")

    # TODO: Flag setting + handle HIWORD/HIBYTE and constant bit shifting
    # LOBYTE(field_10_anim.field_4_flags) |= 0x10u;

    check(process("v5 = Math_FixedPoint_Divide_450FB0(this->field_0_mBase.field_0_mBase.field_BC_scale, 2621440);"), "v5 = Math_FixedPoint_Divide_450FB0(field_BC_scale, 2621440); // FP_FromInteger(40);")
    check(process("v5 = Math_FixedPoint_Divide_450FB0(this->field_0_mBase.field_0_mBase.field_BC_scale, 0x280000);"), "v5 = Math_FixedPoint_Divide_450FB0(field_BC_scale, 0x280000); // FP_FromInteger(40);")

    check(GetLiteral("~4u;"), ["4", 3])
    check(GetLiteral("0xFFFBu;"), ["4", 6])
    check(process("LOWORD(this->field_0_mBase.field_0_mBase.field_10_anim.field_4_flags) &= ~4u;"), "field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);")
    check(number2Enum(4, anim_enum), "AnimFlags::eBit3_Render")
    check(number2Enum(0x3, anim_enum), "AnimFlags::eBit1 | AnimFlags::eBit2_Animate")
    check(process("this->field_0_mBase.field_0_VTbl = (BaseGameObject_VTable_Union *)&vTbl_LiftMover_5440D4;"), "SetVTable(this, 0x5440D4);")
    check(process("signed __int16 __thiscall Elum::sub_411E40(_DWORD *this)"), "EXPORT signed __int16 Elum::sub_411E40()")
    check(process("signed __int16 __thiscall Elum::sub_411E40(_DWORD *this, int asdf)"), "EXPORT signed __int16 Elum::sub_411E40(int asdf)")
    check(process("Event_Broadcast_417220(1, &this->field_0_mBase.field_0_mBase.field_0_mBase);"), "Event_Broadcast_417220(1, this);")
    check(process("LOBYTE(this->field_0_mBase.field_0_mBase.field_6_flags) |= 4u;"), "field_6_flags.Set(BaseGameObject::eDead);")
    check(process("Map::TLV_Reset_446870(&gMap_507BA8, a3, -1, 0, 0);"), "gMap_507BA8.TLV_Reset_446870(a3, -1, 0, 0);")


def main():
    # get clipboard data
    win32clipboard.OpenClipboard()
    data = win32clipboard.GetClipboardData()
    win32clipboard.CloseClipboard()

    new_lines = ""
    lines = data.split("\r\n")
    for line in lines:
        print("processing: " + line)
        new_lines = new_lines + process(line)  + "\n"

    # set clipboard data
    win32clipboard.OpenClipboard()
    if new_lines != "":
        win32clipboard.EmptyClipboard()
        win32clipboard.SetClipboardText(new_lines)
    win32clipboard.CloseClipboard()

tests()
#main()
