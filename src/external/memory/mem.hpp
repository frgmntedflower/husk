#pragma once
#include <cstdint>
#include <string>
#include <windows.h>

namespace mem {

class Process {
public:
  explicit Process(const std::string &name);
  ~Process();

  Process(const Process &) = delete;
  Process &operator=(const Process &) = delete;

  bool valid() const;
  DWORD pid() const;
  uintptr_t module_base(const std::string &module_name) const;

  template <typename T> T read(uintptr_t addr) const {
    T val{};
    ReadProcessMemory(handle_, reinterpret_cast<LPCVOID>(addr), &val, sizeof(T),
                      nullptr);
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
  DWORD pid_ = 0;
};

} // namespace mem
