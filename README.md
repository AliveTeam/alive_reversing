# Alive_Reversing
An open source reimplementation of Oddworld: Abe's Exoddus.

## Contributing
If you'd like to contribute to our reverse engineering efforts, make sure to follow our reverse engineering guidelines:

### IDA Guidelines
- Use IDA 7
- Use our git hosted IDB.
- All **function/variable/vtables/anything** must end with their corrosponding address. *(Example: **Input_GetKeyState_4EDD20**)*
- Function names should begin with their appropriate category. *(Example: **IO_Open_ASync_4EADA0**)*
- Mark all functions that are completely reverse as decompiled.
- All variables should begin with 's' if their scope is in a class. 'g' if global. (Just use 's' if not sure)
- VTables use the following format: **vClassName_Address**. (Example: **vSlig_547460**)
- Functions that belong exclusivly to VTables should use the following format: **ClassName_FunctionName_Address**. *(Example: **Slig_Ctor_547460**)*

### Code Guidelines
- Todo
