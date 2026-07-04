#include "mem.hpp"
#include <cstdio>
#include <handleapi.h>
#include <minwindef.h>
#include <tlhelp32.h>

namespace mem {

Process::Process(const std::string &name) {
  // TODO: snapshot processes, find pid by name, OpenProcess
}

Process::~Process() {
  if (handle_ && handle_ != INVALID_HANDLE_VALUE)
    CloseHandle(handle_);
}

bool Process::valid() const {
  return handle_ && handle_ != INVALID_HANDLE_VALUE;
}
DWORD Process::pid() const {

  HANDLE hProcessSnap;
  PROCESSENTRY32 pe32;
  DWORD result = 0;

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hProcessSnap)
    return (FALSE);

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hProcessSnap, &pe32)) {
    CloseHandle(hProcessSnap);
    printf("Failed to gather info on system procs");
    return (0);
  }

  return pid_;
}

uintptr_t Process::module_base(const std::string &module_name) const {
  // TODO: TH32CS_SNAPMODULE, walk Module32First/Next, match szModule
  return 0;
}

} // namespace mem
