#pragma once

#include "Project/Models/Report.hpp"
#include "Project/Models/Request.hpp"

namespace Database {
class IRepository
{
public:
    IRepository(const QString& path)
    {
    }
    virtual size_t save(const Models::Request& request) = 0;
    virtual size_t save(const Models::Report& report) = 0;

    virtual Models::Request findRequestById(size_t id) = 0;
    virtual Models::Report findReportById(size_t id) = 0;

    virtual ~IRepository() = default;
};
} // namespace Database