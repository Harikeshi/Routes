// #ifndef GET_STACK_TRACE_HPP
// #define GET_STACK_TRACE_HPP
// #include <sstream>
// #include <string>
//
// #ifdef _WIN32
// #include <DbgHelp.h>
// #include <Windows.h>
// #pragma comment(lib, "dbghelp.lib")
//
// inline std::string getStackTrace(int maxFrames = 62)
// {
//     void* callstack[62];
//     HANDLE process = GetCurrentProcess();
//     SymInitialize(process, NULL, TRUE);
//
//     std::ostringstream stackTrace;
//     WORD frames = CaptureStackBackTrace(0, maxFrames, callstack, NULL);
//     SYMBOL_INFO* symbol = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
//     symbol->MaxNameLen = 255;
//     symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
//
//     for (WORD i = 0; i < frames; i++)
//     {
//         SymFromAddr(process, (DWORD64)(callstack[i]), 0, symbol);
//         stackTrace << i << ": " << symbol->Name << " (0x" << std::hex << symbol->Address << std::dec << ")\n";
//     }
//
//     free(symbol);
//     SymCleanup(process);
//     return stackTrace.str();
// }
//
// #else // Linux/macOS
// #include <cxxabi.h>
// #include <dlfcn.h>
// #include <execinfo.h>
//
// inline std::string getStackTrace(int maxFrames = 62)
// {
//     void* callstack[62];
//     std::ostringstream stackTrace;
//
//     int frames = backtrace(callstack, maxFrames);
//     char** strs = backtrace_symbols(callstack, frames);
//
//     for (int i = 0; i < frames; i++)
//     {
//         Dl_info info;
//         if (dladdr(callstack[i], &info))
//         {
//             int status;
//             char* demangled = abi::__cxa_demangle(info.dli_sname, NULL, NULL, &status);
//             stackTrace << i << ": " << (demangled ? demangled : info.dli_sname)
//                        << " (" << info.dli_fname << ")\n";
//             free(demangled);
//         }
//         else
//         {
//             stackTrace << i << ": " << strs[i] << "\n";
//         }
//     }
//     free(strs);
//     return stackTrace.str();
// }
// #endif
// #endif
