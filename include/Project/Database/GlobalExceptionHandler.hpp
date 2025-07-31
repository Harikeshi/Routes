// #ifndef GLOBAL_EXCEPTION_HANDLER_HPP
// #define GLOBAL_EXCEPTION_HANDLER_HPP
//
// #include "../GetStackTrace.hpp"
// #include "ErrorLogRepository.hpp"
//
// #include <exception>
// #include <memory>
// #include <string>
//
// /* Пример использования с дополнительным полем
// int main() {
//     auto dbConn = std::make_shared<PqxxConnection>("dbname=test user=postgres");
//     auto errorRepo = std::make_shared<ErrorLogRepository>(dbConn);
//
//     GlobalExceptionHandler::init(errorRepo);
//
//     try {
//         // Ваш код
//     } catch (...) {
//         GlobalExceptionHandler::logException(std::current_exception(), "MAIN");
//     }
// }
//   */
//
// class GlobalExceptionHandler
// {
// public:
//     static void init(std::shared_ptr<ErrorLogRepository> repo)
//     {
//         static auto handler = std::make_shared<GlobalExceptionHandler>(repo);
//         std::set_terminate([]() {
//             handler->handleTerminate();
//         });
//     }
//
//     void logException(const std::exception& e, const std::string& source)
//     {
//         repo_->logError(
//             e.what(),
//             getStackTrace(), // Реализуйте получение стека вызовов
//             source);
//     }
//
// public:
//     explicit GlobalExceptionHandler(const std::shared_ptr<ErrorLogRepository>& repo)
//         : repo_(repo)
//     {
//     }
//
//     void handleTerminate()
//     {
//         try
//         {
//             if (const auto e = std::current_exception())
//             {
//                 std::rethrow_exception(e);
//             }
//         }
//         catch (const std::exception& e)
//         {
//             logException(e, "GLOBAL_TERMINATE");
//         }
//         catch (...)
//         {
//             repo_->logError("Unknown exception", "", "GLOBAL_TERMINATE", "CRITICAL");
//         }
//         std::abort();
//     }
//
//     std::shared_ptr<ErrorLogRepository> repo_;
// };
//
// #endif
