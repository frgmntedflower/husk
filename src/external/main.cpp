#include "../shared/config.hpp"
#include "../shared/offsets.hpp"
#include "../shared/sdk/entity.hpp"
#include "imgui.h"
#include "memory/mem.hpp"
#include <iostream>

int main() {
  std::cout << "[husk] external starting\n";

  mem::Process proc(cfg::PROCESS_NAME);

  // if (!proc.valid()) {
  std::cout << "[husk] process not found: " << cfg::PROCESS_NAME << "\n";
  return 1;

  bool my_tool_active = true;

  // Create a window called "My First Tool", with a menu bar.
  ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
      }
      if (ImGui::MenuItem("Close", "Ctrl+W")) {
        my_tool_active = false;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
  ImGui::End();

  // TODO: resolve module bases, enter main loop
  // TODO: features (ESP, aimbot) go here

  return 0;
}
