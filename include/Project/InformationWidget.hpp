#pragma once

#include <QDateTime>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>

class InformationWidget : public QWidget
{
    Q_OBJECT
public:
    enum MessageType
    {
        Error,
        Warning,
        Info,
        Success
    };

    explicit InformationWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
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
    void addMessage(const QString& message, MessageType type)
    {
        QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss]");
        QString typeStr = getTypeString(type);
        QColor color = getTypeColor(type);

        QString formatted = QString("<span style='color:%1;'>%2 %3: %4</span>")
                                .arg(color.name())
                                .arg(timestamp)
                                .arg(typeStr)
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
};
