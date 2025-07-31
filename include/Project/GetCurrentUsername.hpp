#pragma once

#include <string>

#ifdef _WIN32
// #include <DbgHelp.h>
#include <Windows.h>
#include <lmcons.h>
#else
#include <pwd.h>
#include <unistd.h>
#endif

/*!
 * Возвращает имя текущего пользователя операционной системы.
 * @return
 */
inline std::string getCurrentUsername()
{
#ifdef _WIN32
    char username[UNLEN + 1];
    unsigned long size = UNLEN + 1;
    if (GetUserNameA(username, &size))
    {
        return std::string(username);
    }
#else
    uid_t uid = geteuid();
    struct passwd* pw = getpwuid(uid);
    if (pw)
    {
        return std::string(pw->pw_name);
    }
#endif
    return "Unknown";
}