#include "../shared/config.hpp"
#include "../shared/offsets.hpp"
#include "../shared/sdk/entity.hpp"
#include "dx9/windowing.hpp"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "memory/mem.hpp"
#include "sdk/vec3.hpp"
#include <cstdint>
#include <d3d9.h>
#include <iostream>

int main() {
  std::cout << "[husk] external starting\n";

  mem::Process proc(cfg::PROCESS_NAME);

  if (!proc.valid()) {
    std::cout << "[husk] process not found: " << cfg::PROCESS_NAME << "\n";
    return 1;
  }

  std::cout << "[husk] process with pid " << proc.get_pid() << " was opened \n";

  uintptr_t localPlayerEntityBase = proc.read<uintptr_t>(
      proc.get_client_base() + offsets::client::dwLocalPlayer);

  std::cout << "[husk] local player entity: 0x" << std::hex
            << localPlayerEntityBase << std::dec << "\n";

  Vec3 pos =
      proc.read<Vec3>(localPlayerEntityBase + offsets::entity::m_vecOrigin);

  dx9::setup_dx9_context();

  // TODO: resolve module bases, enter main loop
  // TODO: features (ESP, aimbot) go here

  return 0;
}
