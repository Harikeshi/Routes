#pragma once

#include <QDateTime>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

#include "../MessageType.hpp"
#include "Project/Database/DatabaseConnection.hpp"
#include "Project/Database/InformationRepository.hpp"

namespace Widgets {
/*!
 * Класс вывода информации.
 */
class InformationWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit InformationWidget(QWidget* parent = nullptr)
        : QWidget(parent), repository{}
    {
        try
        {
            //            repository = Database::DatabaseRepository{Database::DatabaseConnection::createConnection("127.0.0.1")};
            //            repository = Database::InformationRepository{Database::DatabaseConnection::createConnection("192.168.205.130")};
            repository = Database::InformationRepository{Database::DatabaseConnection::createConnection("192.168.50.52")};
            qDebug() << "Info connected to postgresql!";
        }
        catch (...)
        {
            qDebug() << "Info haven't connected to postgresql!";
            repository = Database::InformationRepository{};
            //addMessage("Невозможно установить соединение.", MessageType::Error);
        }

        QVBoxLayout* layout = new QVBoxLayout(this);
        textEdit = new QPlainTextEdit(this);

        // Настройка внешнего вида
        textEdit->setReadOnly(true);
        textEdit->setWordWrapMode(QTextOption::NoWrap);
        textEdit->setFont(QFont("Monospace", 9));
        layout->addWidget(textEdit);
        layout->setMargin(0);
    }

public slots:

    void
    addMessage(const QString& message)
    {
        if (!repository.isNull())
            repository.updateInfo(message.toStdString(), getTypeString(MessageType::Info).toStdString());

        // QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss]");
        QString typeStr = getTypeString(MessageType::Info);
        QColor color = getTypeColor(MessageType::Info);
        // QString formatted = QString("<span style='color:%1;'>%2 %3: %4</span>")
        QString formatted = QString("<span style='color:%1;'> %2</span>")
                                .arg(color.name())
                                // .arg(timestamp)
                                // .arg(typeStr)
                                .arg(message.toHtmlEscaped());

        textEdit->appendHtml(formatted);

        // Автоскролл к новому сообщению
        QScrollBar* bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
    }

    void addMessage(const QString& message, MessageType type)
    {
        if (!repository.isNull())
            repository.updateInfo(message.toStdString(), getTypeString(type).toStdString());

        // QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss]");
        // QString typeStr = getTypeString(type);
        QColor color = getTypeColor(type);

        // QString formatted = QString("<span style='color:%1;'>%2 %3: %4</span>")
        QString formatted = QString("<span style='color:%1;'> %2</span>")
                                .arg(color.name())
                                // .arg(timestamp)
                                // .arg(typeStr)
                                .arg(message.toHtmlEscaped());

        // Добавление HTML-форматированного сообщения
        textEdit->appendHtml(formatted);

        // Автоскролл к новому сообщению
        QScrollBar* bar = textEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
    }

    void clearMessages()
    {
        textEdit->clear();
    }

private:
    QPlainTextEdit* textEdit;

    QString getTypeString(MessageType type) const
    {
        switch (type)
        {
        case Error:
            return "ERROR";
        case Warning:
            return "WARNING";
        case Success:
            return "SUCCESS";
        default:
            return "INFO";
        }
    }

    QColor getTypeColor(MessageType type) const
    {
        switch (type)
        {
        case Error:
            return Qt::red;
        case Warning:
            return QColor(255, 165, 0); // Orange
        case Success:
            return Qt::darkGreen;
        default:
            return Qt::blue;
        }
    }

private:
    //    Database::DatabaseRepository repository; // {Database::DatabaseConnection::createConnection()};
    Database::InformationRepository repository; //{Database::DatabaseConnection::createConnection("192.168.50.52")};
};
} // namespace Widgets
