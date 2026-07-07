#pragma once
#include "vec3.hpp"
#include <cstdint>

// CBasePlayer / CBaseEntity data-member offsets for CS:S (steampipe, Windows).
//
// Sources:
//   - JulianOzelRose/CSS-MultiHack-Internal README (primary, confirmed)
//   - Therdel/cssHack CT table (cross-ref)
//
// External mode: read with mem::Process::read<T>(entity_ptr + offset).
// Internal mode: dereference directly as *(T*)(entity_ptr + offset).
//
// [UNVERIFIED] = not found in a primary source; confirm with Cheat Engine.

namespace offsets::entity {

// --- Identity / state ---
constexpr uintptr_t m_iHealth = 0x00D0;   // int    — 0 when dead
constexpr uintptr_t m_iTeamNum = 0x00D8;  // int    — 2 = T, 3 = CT
constexpr uintptr_t m_lifeState = 0x00CF; // uint8  — 0 = alive  [UNVERIFIED]
constexpr uintptr_t m_iCrosshairId =
    0x1B20; // int  — entity index in crosshair (0 = none)

// --- Movement / position ---
constexpr uintptr_t m_vecOrigin = 0x428; // Vec3  — world-space feet position
constexpr uintptr_t m_vecVelocity =
    0x0330;                            // Vec3  — current velocity [UNVERIFIED]
constexpr uintptr_t m_fFlags = 0x0440; // int   — FL_ONGROUND = (1<<0)

// --- Aim ---
constexpr uintptr_t m_vecPunchAngle = 0x127C; // Vec3 — server-side recoil punch

// --- Flash ---
constexpr uintptr_t m_flFlashMaxAlpha =
    0x1A54; // float — write 0.f to remove flash
constexpr uintptr_t m_flFlashDuration =
    0x1A4C; // float — write 0.f to remove flash

// --- Bones ---
// Pointer to the bone matrix array (CBoneCache), 3x4 float matrices per bone.
// Bone 8 = head in CS:S. Read as: *(uintptr_t*)(entity_ptr + m_dwBoneMatrix)
constexpr uintptr_t m_dwBoneMatrix = 0x0EEC; // uintptr_t [UNVERIFIED]

} // namespace offsets::entity

// Flat snapshot of the fields we care about per player.
// Populated externally via ReadProcessMemory; in internal mode fill directly.
struct PlayerData {
  Vec3 origin;
  Vec3 velocity;
  Vec3 punchAngle;
  int health;
  int team;
  uint8_t lifeState; // 0 = alive
  int crosshairId;
  bool valid; // false if slot is empty or any read failed
};
