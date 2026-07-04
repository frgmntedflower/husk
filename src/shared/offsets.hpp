#pragma once
#include <cstdint>

// CS:S steampipe (Windows) static offsets.
//
// Module bases are resolved at runtime via TH32CS_SNAPMODULE; everything here
// is relative to those bases.
//
// Sources:
//   - JulianOzelRose/CSS-MultiHack-Internal (confirmed, steampipe build)
//   - Therdel/cssHack Cheat Engine table (cross-ref, older/Linux build)
//
// Any value marked [UNVERIFIED] was not found in a primary source — confirm
// with Cheat Engine before relying on it.

namespace offsets {

    // -------------------------------------------------------------------------
    // client.dll
    // -------------------------------------------------------------------------
    namespace client {
        // Pointer to the local CBasePlayer object.
        constexpr uintptr_t dwLocalPlayer  = 0x5F4B68;

        // Array of CBaseEntity pointers, stride 0x10 per slot.
        // Read as: base + dwEntityList + index * 0x10 → CBaseEntity*
        constexpr uintptr_t dwEntityList   = 0x6098C8;

        // Force-input globals (write to trigger the action for one tick).
        constexpr uintptr_t dwForceJump    = 0x677300;
        constexpr uintptr_t dwForceAttack  = 0x677310;

        // World-to-screen projection matrix (float[4][4]).
        // [UNVERIFIED] — find with Cheat Engine's pointer scanner or sig-scan
        // "F3 0F 10 05 ?? ?? ?? ?? F3 0F 59 C8" in client.dll.
        constexpr uintptr_t dwViewMatrix   = 0x0; // TODO
    }

    // -------------------------------------------------------------------------
    // engine.dll
    // -------------------------------------------------------------------------
    namespace engine {
        // View angles the engine is using (read = current aim, write = set aim).
        constexpr uintptr_t dwViewAngles   = 0x53E4E4;

        // Connected player count.
        constexpr uintptr_t dwNumPlayers   = 0x6DA960;

        // IClientState pointer and key sub-offsets.
        // [UNVERIFIED] — these match the CSGO layout; verify for CSS.
        constexpr uintptr_t dwClientState       = 0x0; // TODO
        constexpr uintptr_t dwClientState_State = 0x108; // 6 = fully in-game
    }

    constexpr int MAX_PLAYERS = 32;

} // namespace offsets
