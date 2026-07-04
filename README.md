# husk

A Counter-Strike: Source cheat framework written in C++, built on Linux and cross-compiled to Windows via MinGW-w64.

Designed in two phases:
- **External** — runs as a standalone `.exe`, reads/writes game memory from outside the process via `ReadProcessMemory`/`WriteProcessMemory`
- **Internal** — compiled as a `.dll`, injected into `hl2.exe` directly for deeper access (VMT hooks, direct pointer dereference, overlays)

> Offline / local VM use only.

---

## Project Structure

```
husk/
├── CMakeLists.txt              # Root CMake — registers both targets
├── toolchain/
│   └── mingw64.cmake           # Cross-compile toolchain (Linux → Windows x64)
├── scripts/
│   └── build.sh                # Build both targets + copy outputs to ~/vm_share
├── vendor/                     # Third-party libs (minhook, imgui, etc. — add as needed)
└── src/
    ├── shared/                 # Code shared between external and internal
    │   ├── config.hpp          # Feature toggles and tuning values
    │   ├── offsets.hpp         # Static CS:S memory offsets (client.dll, engine.dll)
    │   └── sdk/
    │       ├── vec3.hpp        # Vec3 math type
    │       └── entity.hpp      # PlayerData struct + per-field netvar offsets
    ├── external/               # Produces: husk_external.exe
    │   ├── CMakeLists.txt
    │   ├── main.cpp            # Entry point — attach loop, main cheat loop
    │   └── memory/
    │       ├── mem.hpp         # Process class with typed read<T>/write<T> templates
    │       └── mem.cpp         # Process attach, module base resolution
    └── internal/               # Produces: husk_internal.dll
        ├── CMakeLists.txt
        ├── dllmain.cpp         # DllMain entry + cheat thread spawn
        └── hooks/
            └── hooks.hpp       # Hook install/remove interface stubs
```

---

## Building

Requirements: `mingw-w64`, `cmake >= 3.20`

```bash
# Install toolchain (Debian/Ubuntu)
sudo apt install mingw-w64 cmake

# Build both targets and copy to VM shared folder
bash scripts/build.sh
```

Outputs land in `build-win64/bin/` and are copied to `~/vm_share`:

| File                  | Type | Phase    |
|-----------------------|------|----------|
| `husk_external.exe`   | EXE  | External |
| `husk_internal.dll`   | DLL  | Internal |

Both are statically linked against the C/C++ runtime — no MinGW DLLs needed on the Windows VM.

---

## Architecture

### External phase

`husk_external.exe` runs as a normal Windows process alongside CS:S.

```
main.cpp
  └── mem::Process("hl2.exe")       — find PID, OpenProcess
        ├── module_base("client.dll") — TH32CS_SNAPMODULE walk
        ├── module_base("engine.dll")
        └── read<T>(addr)            — ReadProcessMemory wrapper
              └── features (ESP, aimbot, ...)
```

Memory access is always typed via the `read<T>` / `write<T>` templates on `mem::Process`. No raw casts scattered through feature code.

### Internal phase

`husk_internal.dll` is injected into `hl2.exe` (e.g. via `LoadLibrary` injection).

```
DllMain (DLL_PROCESS_ATTACH)
  └── CreateThread → cheat_thread
        ├── resolve engine interfaces (IClientEntityList, IEngineClient, ...)
        ├── hooks::install()   — VMT / detour hooks
        └── main loop         — CreateMove, PaintTraverse, etc.
```

In internal mode you get direct pointer access (no RPM overhead), access to engine interfaces, and the ability to render overlays via PaintTraverse.

### Shared layer

Both modes share the same offset and SDK definitions:

| File | Purpose |
|---|---|
| `config.hpp` | Single place to toggle features and tune values (FOV, smoothing, enemy-only, etc.) |
| `offsets.hpp` | Static offsets into `client.dll` and `engine.dll` — update these when the game patches |
| `sdk/vec3.hpp` | `Vec3` with length, dot, normalize — used for aim math and distance checks |
| `sdk/entity.hpp` | `PlayerData` (origin, health, team, lifeState) + per-field offsets (`m_iHealth`, `m_vecOrigin`, etc.) |

---

## Roadmap

### External
- [ ] `mem.cpp` — implement `find_pid` + `module_base`
- [ ] Read `localplayer`, `entitylist`, `viewmatrix` from `client.dll`
- [ ] ESP — world-to-screen, draw boxes/health in an overlay window
- [ ] Aimbot — angle calc, FOV filter, smoothing

### Internal
- [ ] Resolve `IClientEntityList`, `IEngineClient`, `IVEngineClient` via `CreateInterface`
- [ ] `hooks::install()` — VMT hook `CreateMove` and `PaintTraverse`
- [ ] Migrate ESP to `PaintTraverse` (access to `ISurface` draw calls)
- [ ] Migrate aimbot to `CreateMove` (direct `CUserCmd` manipulation)
- [ ] Add `minhook` or manual VMT swap to `vendor/`

---

## Notes

- Offsets in `offsets.hpp` target the steampipe CS:S build — reverify with Cheat Engine or a dumper after any game update
- The internal DLL has no unload logic yet — plan for a clean eject path (unhook, free resources, `FreeLibraryAndExitThread`) before adding stateful hooks
- `config.hpp` is compile-time only for now — wire it to a runtime config file or an ImGui menu when the feature set grows
