# uni-ui — Qt6 frontend for `uni`

Native desktop UI for the `uni` package manager (0.6.0+).

## Build dependencies

- CMake ≥ 3.21
- Qt6 (Widgets + Concurrent) ≥ 6.4
- C++17 compiler (g++ / clang)

On Ubuntu / Debian / Pika OS:

```sh
sudo apt install build-essential cmake qt6-base-dev qt6-tools-dev
```

## Build

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/uni-ui
```

## Install

From the repo root, run:

```sh
./packaging/install-ui.sh
```

This builds the binary, copies it to `~/.local/bin/uni-ui`, drops a desktop
entry into `~/.local/share/applications/`, and (on systemd hosts) enables a
user timer that refreshes the package index every 6 hours.

## Runtime dependencies

`uni-ui` shells out to `uni` for all real work. The `uni` CLI must be on PATH
(or set the explicit path in Settings → CLI path).

The first launch builds a local SQLite index of available packages in
`~/.local/share/uni/index.db`. Subsequent launches use the cached index for
sub-second search.

## Architecture

See PRD §5 (in repository root or release notes) for the full design. The
short version:

- `MainWindow` hosts a `SearchBar`, `ResultsModel` + `ResultsDelegate`, a
  `PackageDetailPane`, and a `DownloadQueue` at the bottom.
- `ProcessManager` owns all `QProcess` interactions with `uni` (search,
  install, info, sources benchmark, index refresh).
- `IndexWatcher` re-runs the current query when the index DB mtime changes.
- `SourceBenchmark` reads the cached `sources.json` immediately and triggers
  a background refresh; ranks flow into the model and re-sort results live.
