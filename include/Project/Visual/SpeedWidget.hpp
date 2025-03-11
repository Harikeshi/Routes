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

        multiplier = new QLabel("Speed Multiplier: x1", this);
        // multiplier->setMinimumWidth(150);

        layout->addWidget(multiplier);
    }

    void update(const int speedMultiplier)
    {
        this->multiplier->setText(QString("Speed Multiplier: x%1").arg(speedMultiplier));
    }

private:
    QLabel* multiplier;
};