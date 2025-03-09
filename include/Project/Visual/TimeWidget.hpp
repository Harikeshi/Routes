#pragma once

#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace Visual {
class TimeWidget : public QWidget
{
public:
    TimeWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        labelMinutes = new QLabel("min: 0", this);
        labelHours = new QLabel("hours: 0", this);
        labelDays = new QLabel("days: 0", this);

        layout->addWidget(labelMinutes);
        layout->addWidget(labelHours);
        layout->addWidget(labelDays);
    }

public slots:
    void updateTime(const double time)
    {
        // qint64 seconds = static_cast<qint64>(time * 60);

        // TODO: Можно добавить общее время
        int t = static_cast<int>(time);

        QDateTime dateTime; //  = QDateTime::currentDateTime();

        int minutes = t % 60;
        int hours = (t / 60) % 24;
        int days = t / (24 * 60);

        labelMinutes->setText(QString("min: %1").arg(minutes));
        labelHours->setText(QString("hours: %1").arg(hours));
        labelDays->setText(QString("days: %1").arg(days));
    }

    void reset()
    {
    }

private:
    QLabel* labelMinutes;
    QLabel* labelHours;
    QLabel* labelDays;
};
} // namespace Visual
