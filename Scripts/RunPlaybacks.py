import os
import subprocess

RELIVE_EXE_PATHS = ["../build/Source/relive/Debug/relive.exe"]
EXODDUS_DATA_PATHS = ["C:/GOG Games/Abes Exoddus", "C:/Spiele/Oddworld Abes Exoddus"]
ODDYSEE_DATA_PATHS = ["C:/GOG Games/Abes Oddysee", "C:/Spiele/Oddworld Abes Oddysee"]

def StartPlayback(playbackFileName, gameDataPaths):
    for relive_exe in RELIVE_EXE_PATHS:
        relive_exe = os.path.abspath(relive_exe)
        if os.path.exists(relive_exe):
            for game_data_path in gameDataPaths:
                if os.path.exists(os.path.join(game_data_path, playbackFileName)):
                    subprocess.Popen([relive_exe, "-renderer=gl", f"-play={playbackFileName}", "-fastest"], cwd=game_data_path)
                    return

os.chdir(os.path.dirname(__file__))

print("running AE")
StartPlayback("full.dat", EXODDUS_DATA_PATHS)

print("running AO")
StartPlayback("full.ao", ODDYSEE_DATA_PATHS)
