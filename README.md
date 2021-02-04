# R.E.L.I.V.E.
An Open-Source Engine Replacement for Oddworld: Abe's Oddysee and Oddworld: Abe's Exoddus.

R.E.L.I.V.E. is a fan-made, open-source project that intends to become a fully compatible engine replacement for Oddworld Inhabitants' iconic first two games. The project's goals include fixing the original games' bugs and also eventually providing a modding / level creation interface, alongside of course making it possible to study or use the engine for new projects.

For more details, please check the project's website: https://aliveteam.github.io/

## Contributing

Anyone who wishes to contribute is encouraged to join the project's [Discord](
https://discord.gg/khs6KKS), where most of the communication happens.

By contributing to this project, the contributor agrees and accepts that their code will be licensed under a GPL-compatible license in the future.

## Building R.E.L.I.V.E.

### Cloning

Since the project uses third-party repositories for some of its functions, you need to clone the project using the `--recursive` flag.

```
git clone --recursive https://github.com/AliveTeam/alive_reversing.git
```

#### **Regardless of your platform, you need to create a folder called `build` in your repository root! `.gitignore` is configured to ignore this folder. This helps to prevent polluting the commits with binaries.**

### Build on Windows using `msbuild`

[SDL](https://libsdl.org/download-2.0.php) and [CMake](https://cmake.org/) is required to build the project.

1. `cd build`
2. `cmake -S .. -B . -DSDL2_DIR=PATH_TO_YOUR_SDL2_DIRECTORY`
3. `msbuild /p:Configuration=Debug;Platform=Win32 ar.sln /m`

### Build on Windows using Visual Studio 2017

[SDL](https://libsdl.org/download-2.0.php) is required to build the project.

1. Launch VS2017.
2. Goto `File` -> `Open` -> `CMake`. Then choose the `CMakeLists.txt` from the root of this cloned git repository.
3. Select `CMake` -> `Change CMake settings` -> `CMakeLists.txt`.
4. Choose `x86-debug` and click `select`.
5. This will open `CMakeSettings.json`, edit the line `cmakeCommandArgs` to contain `-DSDL2_DIR=PATH_TO_YOUR_SDL2_DIRECTORY`, set your build directory to `build` and then click `generate` in the top yellow warning bar.
6. Invoke `CMake` -> `Build all` to build the project.

### Build on Linux

1. Install SDL2 using your package manager.
2. `cd build`
3. `cmake -S .. -B .`
4. `make -j$(nproc)`
5. You'll find your executables in the `Source` folder under `AliveExe`.
6. You can optionally install the package using `make install` or create a Debian-compatible package using `cpack -G DEB`.
