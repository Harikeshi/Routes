#pragma once

#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

namespace Widgets {
/*!
     * Класс выводит общее время.
     */
class TimeWidget final : public QWidget
{
public:
    TimeWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        labelTime = new QLabel("Time: 0:0:0", this);
        labelTime->setMinimumWidth(100);
        this->resize(200, 20);
        labelTime->setFont(QFont{"Arial", 10, QFont::Bold});

        mainLayout->addWidget(labelTime);

        multiplier = new QLabel("Multiplier: x1", this);
        multiplier->setMinimumWidth(200);
        this->resize(200, 20);
        multiplier->setFont(QFont{"Arial", 10, QFont::Bold});

        mainLayout->addWidget(multiplier);
        this->setMaximumWidth(200);
        this->setMaximumHeight(70);
    }

    void reset()
    {
        labelTime->setText(QString("Time: 0:0:0"));
        multiplier->setText(QString("Multiplier: x1"));
        defaultNumber = 1;
    }

    void updateTime(double time)
    {
        // qint64 seconds = static_cast<qint64>(time * 60);

        // TODO: Можно добавить общее время
        int t = static_cast<int>(time);

        QDateTime dateTime; // = QDateTime::currentDateTime();

        int seconds = t % 60;
        int minutes = (t / 60) % 60;
        int hours = t / (60 * 60);

        labelTime->setText(QString("Time: %1:%2:%3").arg(hours).arg(minutes).arg(seconds));
    }

    void setMultiplier()
    {
        multiplier->setText(QString("Multiplier: x") + QString::number(defaultNumber, 'f', 2));

        update();
    }

    double speedDown(int mult)
    {
        if (defaultNumber == 1)
        {
            defaultNumber = 0.9;
        }
        else if (defaultNumber < 1)
        {
            if (defaultNumber - 0.1 > 0.1)
            {
                defaultNumber -= 0.1;
            }
        }
        else if (defaultNumber > 1)
        {
            if (defaultNumber - mult > 0)
            {
                defaultNumber -= mult;
            }
            else
            {
                defaultNumber = 0.9;
            }
        }

        setMultiplier();

        return defaultNumber;
    }

    double speedUp(double mult)
    {
        if (defaultNumber < 1)
        {
            defaultNumber += 0.1;
        }
        else
        {
            defaultNumber += mult;
        }

        setMultiplier();

        return defaultNumber;
    }

    double setSpeed(double mult)
    {
        defaultNumber = mult;

        setMultiplier();

        return defaultNumber;
    }

private:
    QLabel* multiplier;
    double defaultNumber{1};

    QLabel* labelTime;
};
} // namespace Widgets
