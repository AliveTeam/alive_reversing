# Sets all char[] text strings to const char[] if they're used in certain functions.
# This makes code readability a little better.
# For use in Oddworld: Abe's Exoddus NoStubs Edition.
# sub_4630F0 is a good place to make sure this script works.

import idautils
import idc
import idaapi

def set_chars_to_const_good(ref_address):
    for x in XrefsTo(ref_address):
        if DecodeInstruction(x.frm).itype != 16: # If not a call func, continue.
            continue;
        func_args = idaapi.get_arg_addrs(x.frm)
        if func_args is not None and len(func_args) > 0:
            paramAddr = GetOperandValue(func_args[0], 0)
            if paramAddr < idaapi.get_imagebase():
                continue
            paramType = GetType(paramAddr)
            if paramType == "char[]" or paramType is None:
                print "Setting", hex(paramAddr), GetString(paramAddr), "called from", hex(x.frm), "to const char[]"
                SetType(paramAddr, "const char[]")
                
def set_chars_to_const_hacky(ref_address):
    for x in XrefsTo(ref_address):
        if DecodeInstruction(x.frm).itype != 16: # If not a call func, continue.
            continue;
        paramAddr = GetOperandValue(x.frm - 5, 0)
        if paramAddr < idaapi.get_imagebase():
            continue
        paramType = GetType(paramAddr)
        if paramType == "char[]" or paramType is None:
            print "Setting", hex(paramAddr), GetString(paramAddr), "called from", hex(x.frm), "to const char[]"
            SetType(paramAddr, "const char[]")

def set_chars_to_const(ref_address):
    print "Searching for char[] args called from", hex(ref_address)
    set_chars_to_const_good(ref_address)
    set_chars_to_const_hacky(ref_address)

set_chars_to_const(0x49C170)
set_chars_to_const(0x4DBE00)
set_chars_to_const(0x4DBE70)