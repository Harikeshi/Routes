#pragma once

#include <QString>

namespace Database {
struct ReportRowModel
{
    size_t report_id;
    size_t request_id;
    QString message;
    QDateTime date;
    QString owner;
};
} // namespace Database