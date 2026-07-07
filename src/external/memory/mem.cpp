#include "mem.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <handleapi.h>
#include <iostream>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include <winnt.h>

namespace mem {

Process::Process(const std::string &name) {
  // TODO: snapshot processes, find pid by name, OpenProcess
  process_name = name;
  find_pid();
  open_proc();
  client_dll_base = get_module_base(pid_, "client.dll");
  printf("[husk] client.dll base: 0x%llx\n", client_dll_base);
}

Process::~Process() {
  if (handle_ && handle_ != INVALID_HANDLE_VALUE)
    CloseHandle(handle_);
}

bool Process::valid() const {
  return handle_ && handle_ != INVALID_HANDLE_VALUE;
}

DWORD Process::get_pid() { return pid_; }

DWORD Process::find_pid() {

  HANDLE hProcessSnap;
  PROCESSENTRY32 pe32;

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (INVALID_HANDLE_VALUE == hProcessSnap)
    return (FALSE);

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hProcessSnap, &pe32)) {
    CloseHandle(hProcessSnap);
    std::cout << "Failed to gather info on system procs" << "\n";
    return (0);
  }

  do {
    if (0 == strcmp(process_name.c_str(), pe32.szExeFile)) {
      pid_ = pe32.th32ProcessID;
      break;
    }
  } while (Process32Next(hProcessSnap, &pe32));

  CloseHandle(hProcessSnap);

  return pid_;
}

HANDLE Process::open_proc() {
  HANDLE localHandle;
  if (pid_ == 0)
    return INVALID_HANDLE_VALUE;

  localHandle =
      OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid_);
  if (localHandle == INVALID_HANDLE_VALUE) {
    std::cout << "ERROR OPENING THE PROCESS" << pid_ << "\n";
    return INVALID_HANDLE_VALUE;
  }

  handle_ = localHandle;
  return handle_;
}

uintptr_t Process::get_module_base(DWORD pid, const char *moduleName) {
  HANDLE hSnap =
      CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
  if (hSnap == INVALID_HANDLE_VALUE) {
    printf("[husk] snapshot failed: %lu\n", GetLastError());
    return 0;
  }

  MODULEENTRY32 me32;
  me32.dwSize = sizeof(MODULEENTRY32);

  uintptr_t base = 0;
  if (Module32First(hSnap, &me32)) {
    do {
      if (_stricmp(me32.szModule, moduleName) == 0) {
        base = (uintptr_t)me32.modBaseAddr;
        break;
      }
    } while (Module32Next(hSnap, &me32));
  } else {
    printf("[husk] Module32First failed: %lu\n", GetLastError());
  }

  CloseHandle(hSnap);
  return base;
}

uintptr_t Process::get_client_base() { return client_dll_base; }

} // namespace mem
