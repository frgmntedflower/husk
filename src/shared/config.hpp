#pragma once

// Feature toggles — flip at compile time or turn into runtime config later.
namespace cfg {
constexpr bool AIMBOT_ENABLED = false;
constexpr bool ESP_ENABLED = true;

// Aimbot
constexpr float AIM_FOV = 5.0f; // degrees
constexpr float AIM_SMOOTH = 8.0f;

// ESP
constexpr bool ESP_SHOW_BOX = true;
constexpr bool ESP_SHOW_HEALTH = true;
constexpr bool ESP_SHOW_NAME = false;
constexpr bool ESP_ENEMIES_ONLY = true;

constexpr const char *PROCESS_NAME = "cstrike_win64.exe"; // CS:S process
} // namespace cfg
