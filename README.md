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
<summary>Build on Windows using Visual Studio 2022</summary>

#### Prerequisites
1. [CMake](https://cmake.org/)
2. [SDL2](https://github.com/libsdl-org/SDL/releases)

### Building
1. Cd into the build directory:
```
cd build
```

2. Generate the solution file:
```
cmake -S .. -B . -DSDL2_DIR=YOUR_SDL2_PATH
```

For example, if you installed SDL2 at `C:\SDL2` you would run:
```
cmake -S .. -B . -DSDL2_DIR=C:\SDL2
```

3. After cmake is done, open the generated `relive.sln` file within your `build` folder with Visual Studio 2022.
4. To start the build, click on `Build` -> `Build Solution` and wait for the build to finish.
5. Once the build has finished successfully, you'll find the relive executable in `build/Source/relive/Debug` and `SDL2.dll` in `build/Source/AliveDllAE/Debug`.

</details>


<details>
<summary>Build on Linux</summary>

#### Prerequisites

1. [CMake](https://cmake.org/)
2. [SDL2](https://github.com/libsdl-org/SDL/releases)

#### Ubuntu
```
sudo apt install cmake libsdl2-dev
```

1. Cd into the build directory:
```
cd build
```

2. Generate the makefile:
```
cmake -S .. -B .
```

3. Build relive:
```
make -j$(nproc)
```

4. Once the build has finished successfully, you'll find the relive executable in `build/Source/relive/Debug`.
5. You can optionally install the package using `make install` or create a Debian-compatible package using `cpack -G DEB`.

</details>
