# gk-unpack

**Українською.** Маленька окрема програма: розпаковує один потік Oodle Kraken — стиснені байти на вході, розпаковані на
виході. Нічого не знає про ігри чи формати файлів. Її запускає GK Prospector (закрита програма Game Klondike) як окрему
програму; цей репозиторій — повний вихідний код саме `gk-unpack.exe`, яку GK Prospector кладе поруч із собою.

**English.** A tiny standalone program that decompresses one Oodle Kraken stream: compressed bytes in, raw bytes out.
It knows nothing about any game or file format. GK Prospector (a closed-source Game Klondike program) runs it as a separate
program; this repository is the complete source of the `gk-unpack.exe` that GK Prospector places next to itself.

## Usage
```
gk-unpack <unpacked size> < compressed stream > unpacked bytes
```
Exit code 0 — done; 1 — bad arguments or input (the reason is printed to stderr). Bytes after the end of the stream are
ignored, as the official decoder does.

## Build (Windows, Visual Studio C++ build tools)
```
build.cmd
```
GitHub Actions builds it from the public repository on every change (Actions → Build → artifact `gk-unpack`);
tags `v*` are published as releases.

## Licence
GPL-3.0-or-later (`LICENSE`). Built on **ooz** by Powzix (https://github.com/powzix/ooz) — the files in `ooz/` are copied
from it unchanged (commit `05038060aa68f9187ae9923b2388ca8db40e58d1`, 2019-02-11). `unpack.cpp` © 2026 Game Klondike.
