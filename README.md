# R.E.L.I.V.E.
An Open-Source Engine Replacement for Oddworld: Abe's Oddysee and Oddworld: Abe's Exoddus.

R.E.L.I.V.E. is a fan-made, open-source project that intends to become a fully compatible engine replacement for Oddworld Inhabitants' iconic first two games. The project's goals include fixing the original games' bugs and also eventually providing a modding / level creation interface, alongside of course making it possible to study or use the engine for new projects.

For more details, please check the project's website: https://aliveteam.github.io/

## Function Keys
| Key | Name |
| :-: | :--: |
| F5 | Quiksave (Exoddus only) | 
| F6 | Quikload (Exoddus only) |
| F10 | Screen Filter |
| F11 | Keep Aspect Ratio |
| F12 | Fullscreen |

## Contributing

Anyone who wishes to contribute is encouraged to join the project's [Discord](
https://discord.gg/khs6KKS), where most of the communication happens.

By contributing to this project, the contributor agrees and accepts that their code will be licensed under a GPL-compatible license (most likely the [MIT/Expat](https://opensource.org/licenses/MIT) license) in the future.

## Building R.E.L.I.V.E.

### Cloning

Since the project uses third-party repositories for some of its functions, you need to clone the project using the `--recursive` flag.

```
git clone --recursive https://github.com/AliveTeam/alive_reversing.git
```

#### **Regardless of your platform, you need to create a folder called `build` in your repository root! `.gitignore` is configured to ignore this folder. This helps to prevent polluting the commits with binaries.**

<details>
<summary>Build on Windows using `msbuild`</summary>

[SDL](https://libsdl.org/download-2.0.php) and [CMake](https://cmake.org/) is required to build the project.

1. `cd build`
2. `cmake -S .. -B . -DSDL2_DIR=PATH_TO_YOUR_SDL2_DIRECTORY`
3. `msbuild /p:Configuration=Debug;Platform=Win32 relive.sln /m`
 
</details>



<details>
<summary>Build on Windows using Visual Studio 2022</summary>

[SDL](https://github.com/libsdl-org/SDL/releases/download/release-2.30.6/SDL2-devel-2.30.6-VC.zip) is required to build the project.

1. Launch VS2022 and click on `Continue without code`.
2. Goto `File` -> `Open` -> `CMake`. Then choose the `CMakeLists.txt` from the root of this cloned git repository.
3. Select `Project` -> `CMake Settings` and click on `Edit JSON` in the top right corner.
4. Change `generator` to `Visual Studio 17 2022` for `x86` or `Visual Studio 17 2022 Win64` for `x64`
5. Change `inheritEnvironments` to `msvc_x86` for `x86` or `msvc_x64_x64` for `x64`
6. Edit the line `cmakeCommandArgs` to contain `-DSDL2_DIR=PATH_TO_YOUR_SDL2_DIRECTORY`, and then press `ctrl+s` to save and generate your files.
7. Invoke `Build` -> `Build all` to build the project.

</details>


<details>
<summary>Build on Linux</summary>

1. Install SDL2 using your package manager.
2. `cd build`
3. `cmake -S .. -B .`
4. `make -j$(nproc)`
5. You'll find your executable in the `Source` folder under `relive`.
6. You can optionally install the package using `make install` or create a Debian-compatible package using `cpack -G DEB`.

</details>

## Testing

### Hook Manager

For testing we provide an application called Hook Manager, which lets you freely change whether specific functions use the original code or the rewritten one. This lets you figure out whether bugs are the result of errors in the rewrite or whether they are part of the original game.

The hook manager can be downloaded from [this](https://github.com/AliveTeam/AliveHookManager/releases/tag/1.0) link. You also need a patched executable for the game of your choice, which you can download [here](https://cdn.discordapp.com/attachments/505078398087987201/807014898717425674/patched_dlls.zip).

To use the manager you need to do the following:

You need to obtain the .map and .dll files for whichever game you want to debug. You can do this in one of two ways:

- Set the `AO_ROOT` and `AE_ROOT` environment variables to their respective game's install folders.
- Compile R.E.L.I.V.E. using the `msbuild` method.
- Your .map and .dll files will be placed in the install folders.

OR:

- Download the .map and DLL files from AppVeyor:
  - Visit [AppVeyor](https://ci.appveyor.com/project/paulsapps/alive-reversing?branch=master)
  - Click on whichever platform you need.
  - Click on Artifacts on the top right corner of the page.
  - Download `build\RELIVE_Binaries_Full_Debug_[ARCHITECTURE]_[BUILDNUMBER].zip`.
- Drop the .map and DLL files from the build folder into the respective game's installation folder.

Regardless of which way you chose, this is how you finalize the process:
- Drop the respective game's patched exe into their installation folder.
- Start the hook manager.
- Select the checkbox for the function(s) you want to check for bugs.
- To start the game with the changes you've made you need to click on the "Save and Launch" icon.
