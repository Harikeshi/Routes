#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QWidget>

namespace Widgets {
class SpeedWidget : public QWidget
{
    Q_OBJECT
public:
    SpeedWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        multiplier = new QLabel("Speed Multiplier: 1", this);
        multiplier->setMinimumWidth(200);
        this->resize(200, 20);
        multiplier->setFont(QFont{"Arial", 10, QFont::Bold});
        // labelTime->setAlignment(Qt::AlignTop);
        // labelTime->setContentsMargins(0, 0, 0, 0);
    }

signals:
    void multiplierChanged(double);

public slots:
    void speedDown(const int& mult)
    {
        if (number == 1)
        {
            number = 0.9;
        }
        else if (number < 1)
        {
            if (number - 0.1 > 0.1)
            {
                number -= 0.1;
            }
        }
        else if (number > 1)
        {
            if (number - mult > 0)
            {
                number -= mult;
            }
            else
            {
                number = 0.9;
            }
        }

        setText();

        emit multiplierChanged(number);
    }

    void speedUp(const double& mult)
    {
        if (number < 1)
        {
            qDebug() << "<1";
            number += 0.1;
        }
        else
        {
            number += mult;
        }

        setText();

        emit multiplierChanged(number);
    }

    void setSpeed(const double mult)
    {
        number = mult;

        setText();

        emit multiplierChanged(number);
    }

    void setText()
    {
        multiplier->setText(QString("Speed Multiplier: ") + QString::number(number, 'f', 2));
        update();
    }

private:
    QLabel* multiplier;

    double number{1};
};
} // namespace Widgets