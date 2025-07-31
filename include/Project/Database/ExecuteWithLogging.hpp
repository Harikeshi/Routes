#ifndef EXECUTE_WITH_LOGGING_HPP
#define EXECUTE_WITH_LOGGING_HPP

#include "ErrorLogRepository.hpp"
#include <exception>
#include <memory>
#include <string>

// TODO: Декоратор для ловли исключениф в отдельных модулях
/*
 *
 * Использование
    executeWithLogging([&]() {
    // Код, который может выбросить исключение
    }, errorRepo, "UserModule");
 *
*/
template<typename Func>
inline void executeWithLogging(Func func,
                               std::shared_ptr<ErrorLogRepository> repo,
                               const std::string& context)
{
    try
    {
        func();
    }
    catch (const pqxx::sql_error& e)
    {
        repo->logError(e.what(), e.query(), "SQL/" + context, "ERROR");
        throw; // Пробрасываем дальше
    }
    catch (const std::exception& e)
    {
        repo->logError(e.what(), "", context, "ERROR");
        throw;
    }
    catch (...)
    {
        repo->logError("Unknown error", "", context, "CRITICAL");
        throw;
    }
}

#endif
