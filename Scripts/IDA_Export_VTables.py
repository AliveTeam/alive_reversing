import idautils
import idc
import idaapi

import datetime
now = datetime.datetime.now()

idata_seg_selector = idc.SegByName('.rdata')
idata_seg_startea = idc.SegByBase(idata_seg_selector)
idata_seg_endea = idc.SegEnd(idata_seg_startea)

print "# AUTO GENERATED VIA IDA SCRIPT " + now.strftime("%Y-%m-%d %I:%M %p")

def print_vtables(prefix):
    for seg_ea in range(idata_seg_startea, idata_seg_endea):
        name = Name(seg_ea)
        if name.lower().startswith(prefix):
            print hex(seg_ea) + "=" + name + "," + name.split('_')[1]

print_vtables("vtbl_")
print_vtables("gvtbl_")

print "# END VTABLE DEFINITION"