# qt-editor

What is this?

A level/path editor for the RELIVE engine. The original AE/AO engines have limits on collision, object and map size. As RELIVE has changes to overcome this maps/paths exported to LVLs with this editor will not work with the original engine.

The long term goal is that an editor will be built directly into the RELIVE engine. But there are many big pieces of engineering work to complete before this can happen so this editor is a stop gap until then. The hope is that any maps created with this editor will also be forwards compatible with whatever future RELIVE changes are implemented.

How to build?

Windows MSVC build instructions

1. Download Qt5 and install using the open source option
2. Clone this repo with --recursive to get all submodules
3. mkdir build
4. cd build
5. set QT_DIR=C:\Qt\5.15.2\msvc2019_64\lib\cmake\Qt5
6. set path=%path%;C:\Qt\5.15.2\msvc2019_64
7. cmake ..

Now you can open the .sln in VS and build or use msbuild from the command line.

To debug from VS set the env in the debug properties to be PATH=C:\Qt\5.15.2\msvc2019_64\bin or whatever your Qt install path is.

Windows Qt creator

1. Open QtCreator
2. File -> Open file or project
3. Choose the CMakeLists.txt from this repo that has been cloned with --recursive
4. Build -> Build all projects
