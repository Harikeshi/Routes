#pragma once

#include <QLabel>
#include <QString>
#include <QWidget>

#include <QHBoxLayout>

class SpeedWidget : public QWidget
{
public:
    SpeedWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        // speed = new QLabel("Speed: 0 x", this);
        multiplier = new QLabel("Speed Multiplier: x1", this);
        multiplier->setMinimumWidth(150);

        // layout->addWidget(speed);
        layout->addWidget(multiplier);
    }

    void update(/*const double speed,*/ const int speedMultiplier)
    {
        // this->speed->setText(QString("Speed: %1 x").arg(speed));

        this->multiplier->setText(QString("Speed Multiplier: x%1").arg(speedMultiplier));
    }

private:
    // QLabel* speed;
    QLabel* multiplier;
};