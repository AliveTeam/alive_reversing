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
- Ensure you use spaces instead of tabs (4 spaces per tab)
- Ensure any shared vars/arrays are wrapped in ALIVE_VAR or ALIVE_ARY macros. This is so that when running as part of the real game they point to the original data. And when running in its own binary it becomes its own copy of the data.
- Ensure any structures sizes are validated with ALIVE_ASSERT_SIZEOF(StructureType, Size); this helps catch aligment issues at compile time, invidivual fields may still have alignment issues but this is a quick and easy sanity check of the general case.
- Ensure your function is marked with the EXPORT macro from FunctionFwd.hpp otherwise it will not be replaced/hooked at runtime in the real game. Also if a function must be stubbed so that you can call it ensure its body starts with NOT_IMPLEMENTED(); macro so that the real games function is called instead of the stub.
- "Deoptimize" reversed functions as much as possible, for instance:
  - Replace any constructs with equivalents that are easier to read but have the same effect.
  - Undo loop unswitching https://en.wikipedia.org/wiki/Loop_unswitching
  - Remove duplicated/redundant local vars.
  - In addition to making vars as local as possible also make them as const as possible, this aids readablitiy and also makes it easier to spot more vars that have become redundant or are actually just copies of others.
  - Make each local as "local" as possible, for instance:
```
        int x;
        [lots of code]
        x = bar();
```
  Can be replaced with:
```
        [lots of code]
        const int x = bar();
```
- Todo other guidelines

### Useful reversing hints/tip/videos
 - Comparing C to machine language https://www.youtube.com/watch?v=yOyaJXpAYZQ
 - Real-world Decompilation with IDA Pro https://www.youtube.com/watch?v=vb18UVF4a_o&index=1&list=PLQgy5xfkFQv2FyKPwYHkgy25yCyy7JGWG
 - Time lapse decompliation of a function from this project. See description for more hints, possibly play at a slower speed to see roughly what is going on. https://www.youtube.com/watch?v=pxm3F_pQHMk&feature=youtu.be

### How to build
- Clone the repo with the --recursive option so that submodules are cloned.
- Download https://www.libsdl.org/release/SDL2-devel-2.0.4-VC.zip
- Create a build directory in repo root
- Create a SDL2 directory in the build directory
- Extract SDL2-devel-2.0.4-VC.zip into freshly created SDL2 directory
- Open a visual studio developer command prompt and cd into the build directory
- Run "cmake .. -DSDL2_DIR=c:\Alive_Repo_Root\build\SDL2"
- Run "msbuild /p:Configuration=Debug;Platform=Win32 ar.sln /m" to compile
- Note that the output dll is hard coded to output at "C:\GOG Games\Abes Exoddus\AliveDll.dll"
- Note that you need to either obtain a modified copy of Exoddus.exe or modify Exoddus.exe yourself to add AliveDll.dll to the imports list

