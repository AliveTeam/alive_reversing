# relive_path_dump

Dumps every path of every `.lvl` file in an Abe's Exoddus (or Oddysee) install
to JSON, via `ReliveAPI::EnumeratePaths` and `ReliveAPI::ExportPathBinaryToJson`.

## Build

Wired into the top-level `CMakeLists.txt`. Build the `relive_path_dump` target as usual.

## Run

```
relive_path_dump <game_dir> <out_dir>
```

Writes `<out_dir>/<lvl>_path<id>.json` per path and `<out_dir>/manifest.json`
(exported and failed entries). Exits non-zero if nothing was exported.

## Status

Syntax-checked against the alive_reversing headers only; not yet built or run
against real game data.
