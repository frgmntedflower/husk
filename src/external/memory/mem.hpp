#pragma once
#include <cstdint>
#include <iostream>
#include <minwindef.h>
#include <string>
#include <tlhelp32.h>
#include <windows.h>

namespace mem {

class Process {
public:
  explicit Process(const std::string &name);
  ~Process();

  Process(const Process &) = delete;
  Process &operator=(const Process &) = delete;

  bool valid() const;
  DWORD find_pid();
  uintptr_t module_base(const std::string &module_name) const;
  DWORD get_pid();
  HANDLE open_proc();
  uintptr_t get_module_base(DWORD pid, const char *module_name);

  uintptr_t get_client_base();

  void setPid();

  template <typename T> T read(uintptr_t addr) const {
    T val{};
    SIZE_T bytesRead = 0;
    BOOL ok = ReadProcessMemory(handle_, reinterpret_cast<LPCVOID>(addr), &val,
                                sizeof(T), &bytesRead);
    if (!ok) {
      std::cout << "[husk] ReadProcessMemory failed at 0x" << std::hex << addr
                << std::dec << " err=" << GetLastError() << "\n";
    }
    return val;
  }

  template <typename T> bool write(uintptr_t addr, const T &val) const {
    SIZE_T written = 0;
    return WriteProcessMemory(handle_, reinterpret_cast<LPVOID>(addr), &val,
                              sizeof(T), &written) &&
           written == sizeof(T);
  }

private:
  HANDLE handle_ = nullptr;
  std::string process_name;
  uintptr_t client_dll_base;
  DWORD pid_ = 0;
};

} // namespace mem
