import idaapi
import base64
import ida_hexrays

if idaapi.IDA_SDK_VERSION >= 690:
    from PyQt5.Qt import QApplication
else:
    from PySide.QtGui import QApplication

print "Oddworld Alive IDA Helper Version 0.1 by mlgthatsme"

def copy_to_clip(data):
    QApplication.clipboard().setText(data)

# Menu item icon
abe_icon_data_b64 = "iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAMAAADXqc3KAAABJlBMVEUAAAAAAAC4sQSZkwOWkQMVFAAAAAAAAAAAAACyqwSspQOHggNiXwJpZgJDQQE0MgEnJgEmJQAgHwEWFQAAAAAAAAAAAACMhgOSjAONhwOEgQOKhQN6dQJ9eANxbAJ1cgJbWQJdWQJYVAJGQwFAPQE4NQE0MgEtKgExLwEXFgAMCwAEBAAAAAD98wX//wUAAAD/+gX/9wXCuwT//AX/9QX+9QUKCQDv6AXd1gTa0gTQyASSjAM9OgEvLgEUEwAPDwD68AX37QXz7AXs5ATp4gTl2wTf2gS9tgSupwOOiAJ0cAJnYwJjXwJaVgI4NgEpKAH37wX17wXXzgTSywTMyASpogOmoAOgmQOWkAOOjQOFgAKBfAJ9dwJwawJvagJRTgJQTQJGRAF1GMLdAAAALXRSTlMABf3u6mkdGBD+9N7KyaWXkIV+bzYuCerp5uXe3dfX1MO+ubKyppyainRhRCqzTsXAAAABGUlEQVQoz23S1XLDMBBAUYWZkzIzaVfGoO0wQ5nx/3+ikzSpZLf3TXseVpoR4Z3trEUzLvKn4xsAaB8sOeenBaCUQj7pmPs9QKdBKGuHdF2agSYd2eauLaA/wXJQhFxnAax0LoKvIc1BraVFSEjaHDQlLsIu0EUQE3dvgsrYdIOiQkR4Y3AFqqUCo6xVqoE3yMEfvjXQ6iodEydy2M8hJZt6BT9hiGNLd59wiPTReMRXKGOvgsN1Dt4RIk4arD5GxC8Ph3jI1D/aCmXNkW519zgEosVifnbdy+vCxgURZJW9gEQ1KDfdOSKWqvbuWhJ199V9YivgecB3uMc3OUvs+cqVwdPAeE4SZwn5qpiXD//5Jpltb8z3e/oGL5MzHe3c2xYAAAAASUVORK5CYII="

act_icon = idaapi.load_custom_icon(data=base64.b64decode(abe_icon_data_b64), format="png")

class AliveHelper(idaapi.action_handler_t):
    def __init__(self, calling_action):
        idaapi.action_handler_t.__init__(self)
        self.calling_action = calling_action

    def activate(self, ctx):
        print "CALLING FROM %s" % (self.calling_action)

        for f in alive_functions:
            if f[0] == self.calling_action:
                if f[4] is not None:
                    f[4](0, ctx)
        return 1
        
    def update(self, ctx):
        for f in alive_functions:
            if f[0] == self.calling_action:
                if f[4] is not None:
                    return f[4](1, ctx)
        return 0 # idaapi.AST_ENABLE_FOR_FORM if ctx.form_type == idaapi.BWN_DISASM else idaapi.AST_DISABLE_FOR_FORM

type_fix_rules=[
    ['__cdecl', 'CC'],
    ['__thiscall', '__fastcall /*__thiscall*/'],
    ['_BYTE', 'unsigned char'],
]
    
def fix_type_string(input):
    result = input
    for i in type_fix_rules:
        result = result.replace(i[0], i[1])
    return result
        
# functions passed with check_active should return 1 if the button should be active, otherwise it is inactive

def alive_copy_unimplemented(check_active, ctx):
    if check_active == 1:
        return 0;
        
    decompiled = ida_hexrays.decompile(here())
    result = "EXPORT " + str(decompiled.type) + " { NOT_IMPLEMENTED; }"
    result = fix_type_string(result)
    copy_to_clip(result)
    print "Alive Function copied to clipboard!"
    
def alive_copy_decompiled(check_active, ctx):
    if check_active == 1:
        return 0;
        
    addr = here()
    
    for i in xrange(len(ctx.chooser_selection)):
        idx = ctx.chooser_selection.at(i)
        addr = ida_name.get_nlist_ea(idx)
        
    result = "EXPORT " + str(ida_hexrays.decompile(addr))
    result = fix_type_string(result)
    copy_to_clip(result)
    print "Alive Function copied to clipboard!"
    
def alive_copy_variable(check_active, ctx):
    if check_active == 1:
        return 0;
        
    result = ""
    
    if (ctx.widget_type == idaapi.BWN_DISASM):
        name_addr = here()
        name_str = Name(here())
        name_type = GetType(name_addr)
        name_addr_str = "0x" + "%x" % name_addr
        result += "ALIVE_VAR(1, %s, %s, %s, 0);\n" % (name_addr_str, name_type, name_str)
    else:     
        for i in xrange(len(ctx.chooser_selection)):
            
            idx = ctx.chooser_selection.at(i)
            name_addr = ida_name.get_nlist_ea(idx)
            name_str = ida_name.get_nlist_name(idx)
            
            name_type = GetType(name_addr)
            name_addr_str = "0x" + "%x" % name_addr
            # ALIVE_VAR(1, 0x5C2F00, int, sJoystickID_5C2F00, 0);
            result += "ALIVE_VAR(1, %s, %s, %s, 0);\n" % (name_addr_str, name_type, name_str)
        
    print result
    copy_to_clip(result)

alive_functions = [
    ['alive:copy_unimplemented', "Copy function NOT_IMPLEMENTED", [idaapi.BWN_PSEUDOCODE], act_icon, alive_copy_unimplemented],
    ['alive:copy_decompiled', "Copy function decompiled", [idaapi.BWN_PSEUDOCODE], act_icon, alive_copy_decompiled],
    ['alive:copy_variable', "Copy Variable Macro", [idaapi.BWN_NAMES, idaapi.BWN_DISASM], act_icon, alive_copy_variable]
]


if 'hooks' not in globals() or hooks is None:
    for f in alive_functions:
        print "Registering function %s" % f[0]
        idaapi.register_action(idaapi.action_desc_t(
            f[0],
            f[1],
            AliveHelper(f[0]),
            '',
            '',
            f[3]))
    
    class Hooks(idaapi.UI_Hooks):
        def finish_populating_tform_popup(self, form, popup):
            # print "Current Form", idaapi.get_tform_type(form)
            for f in alive_functions:
                for fmenu in f[2]:
                    if idaapi.get_tform_type(form) == fmenu:
                        idaapi.attach_action_to_popup(form, popup, f[0], None) 
                    

    hooks = Hooks()
    hooks.hook()


else:
    print "Actions found; unregistering."
    # No need to call detach_action_from_menu(); it'll be
    # done automatically on destruction of the action.
    
    for f in alive_functions:
        if idaapi.unregister_action(f[0]):
            print "Unregistered %s" % f[0]
        else:
            print "Failed to unregister action %s" % f[0]

    if hooks is not None:
        hooks.unhook()
        hooks = None