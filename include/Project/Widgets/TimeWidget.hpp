#pragma once

#include <QApplication>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QDateTime>

namespace Widgets {
class TimeWidget : public QWidget
{
public:
    TimeWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        labelTime = new QLabel("Time: 0:0:0", this);
        labelTime->setMinimumWidth(100);
        this->resize(200, 20);
        labelTime->setFont(QFont{"Arial", 10, QFont::Bold});
        // labelTime->setAlignment(Qt::AlignTop);
        // labelTime->setContentsMargins(0, 0, 0, 0);
    }

public slots:
    void updateTime(const double time)
    {
        // qint64 seconds = static_cast<qint64>(time * 60);

        // TODO: Можно добавить общее время
        int t = static_cast<int>(time);

        QDateTime dateTime; // = QDateTime::currentDateTime();

        int seconds = t % 60;
        int minutes = (t / 60);
        int hours = t / (60 * 60);

        labelTime->setText(QString("Time: %1:%2:%3").arg(hours).arg(minutes).arg(seconds));
    }

    void reset()
    {
    }

private:
    QLabel* labelTime;
};
} // namespace Widgets