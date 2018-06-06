from idautils import *
from idaapi import *
from idc import *

# returns the function address by its name
def get_ea_from_name(name):
    for segment in Segments():
        for function_ea in Functions(segment, SegEnd(segment)):
            if name == GetFunctionName(function_ea):
                return function_ea
    return None

# returns the forwarded function, None otherwise
def get_stub_forward(ea):
    instructions = list(FuncItems(ea))
    if len(instructions) == 1 and GetMnem(instructions[0]) == "jmp":
        return GetOpnd(instructions[0], 0)
    return None

def main():
    # find stub functions that forward to other real functions
    for segment in Segments():
        # get all functions
        for stub_ea in Functions(segment, SegEnd(segment)):
            forward_name = get_stub_forward(stub_ea)
            if forward_name is not None:
                stub_name = GetFunctionName(stub_ea)
                # find all references to the stub, to replace them by direct calls to the forwarded func
                for ref_ea in CodeRefsTo(stub_ea, 1):
                    ref_name = GetFunctionName(ref_ea)
                    ref_instructions = list(FuncItems(ref_ea))
                    for ref_instruction in ref_instructions:
                        if GetMnem(ref_instruction) == "call" and GetOpnd(ref_instruction, 0) == stub_name:
                            # Replace call to stub to the forwarded function
                            where = get_ea_from_name(forward_name) - ref_instruction - 5
                            # TODO: sometimes get_ea_from_name returns None
                            PatchByte(ref_instruction + 1, (where >> 0) & 0xff)
                            PatchByte(ref_instruction + 2, (where >> 8) & 0xff)
                            PatchByte(ref_instruction + 3, (where >> 16) & 0xff)
                            PatchByte(ref_instruction + 4, (where >> 24) & 0xff)
                            print "ref %s now calls forward %s instead of stub %s" % (ref_name, forward_name, stub_name)

if __name__ == '__main__':
    main()
