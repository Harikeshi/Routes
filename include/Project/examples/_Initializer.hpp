#pragma once

#include "../Models/Report.hpp"
#include "../Models/Request.hpp"
#include <QObject>

class _Initializer : public QObject
{
    Q_OBJECT
public:
    static _Initializer& instance()
    {
        static _Initializer instance;
        return instance;
    }

    const Models::Request& currentRequest() const
    {
        return m_currentRequest;
    }
    const Models::Report& currentReport() const
    {
        return m_currentReport;
    }

    void setCurrentRequest(const Models::Request& request)
    {
        m_currentRequest = request;
        emit requestChanged(request);
    }

    void setCurrentReport(const Models::Report& report)
    {
        m_currentReport = report;
        emit reportChanged(report);
    }

signals:
    void requestChanged(const Models::Request& request);
    void reportChanged(const Models::Report& report);

private:
    _Initializer() = default;
    ~_Initializer() = default;

    Models::Request m_currentRequest;
    Models::Report m_currentReport;
};