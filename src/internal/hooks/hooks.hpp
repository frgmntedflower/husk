#pragma once

// VMT / detour hook stubs — fill in when transitioning to internal.
namespace hooks {

void install();
void remove();

// Individual hook stubs (add per-function hooks here)
// e.g.: void hook_painttraverse();
//       void hook_createmove();

} // namespace hooks
