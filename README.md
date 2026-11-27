# C/C++ Tetris

A Tetris game

## Controls

| Key | Action |
| --- | --- |
| `A` / `D` | Move left / right |
| `S` | Soft drop |
| `W` | Rotate clockwise |
| `Space` | Hard drop |
| `P` | Pause / resume |
| `Q` | Quit |

## Build and Run

```powershell
cl /std:c++17 /EHsc /O2 main.cpp /Fe:tetris.exe
.\tetris.exe
```

For a non-interactive check:

```powershell
.\tetris.exe --self-test
```