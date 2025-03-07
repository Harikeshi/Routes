#pragma once

#include <QWidget>

#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <string>

class SubmarineWidget : public QWidget
{
    Q_OBJECT

public:
    SubmarineWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QGridLayout* mainLayout = new QGridLayout(this);

        // Поле для ввода скорости
        QLabel* title = new QLabel("Submarine form", this);
        mainLayout->addWidget(title, 0, 0);

        mainLayout->addWidget(new QLabel("Speed:", this), 1, 0);
        speedInput = new QLineEdit(this);
        mainLayout->addWidget(speedInput, 1, 1);

        mainLayout->addWidget(new QLabel("Current position:", this), 2, 0);
        currentPositionX = new QLabel("-", this); // current position
        currentPositionY = new QLabel("-", this); // current position
        mainLayout->addWidget(new QLabel("X:", this), 3, 0);
        mainLayout->addWidget(currentPositionX, 3, 1);
        mainLayout->addWidget(new QLabel("Y:", this), 4, 0);
        mainLayout->addWidget(currentPositionY, 4, 1);

        mainLayout->addWidget(new QLabel("Current Speed:", this), 5, 0);
        currentSpeed = new QLabel("-", this); // current speed
        mainLayout->addWidget(currentSpeed, 5, 1);

        checkButton = new QPushButton("Set Path", this);
        checkButton->setChecked(false);
        checkButton->setCheckable(true); // Включаем функцию фиксации
        //checkButton->setStyleSheet("QPushButton { background-color: green; }"); // Цвет по умолчанию

        mainLayout->addWidget(checkButton, 6, 0);

        QPushButton* resetButton = new QPushButton("Reset", this);
        mainLayout->addWidget(resetButton, 6, 1);

        QPushButton* randomButton = new QPushButton("Random", this);
        mainLayout->addWidget(randomButton, 7, 0);

        // connect(randomButton, &QPushButton::clicked, this, &SubmarineWidget::reset);
        connect(resetButton, &QPushButton::clicked, this, &SubmarineWidget::reset);
        connect(checkButton, &QPushButton::toggled, this, &SubmarineWidget::changeCheckButton);

        connect(speedInput, &QLineEdit::textChanged, this, &SubmarineWidget::actualSpeed);
    }

signals:
    void checkBottomChanging(bool);
    void sendReset();
    void sendSpeed(double);

private slots:
    void actualSpeed()
    {
        auto text = speedInput->text();

        double result = 1;

        if (!text.isEmpty())
        {
            bool containsDigit = false;
            for (int i = 0; i < text.length(); ++i)
            {
                if (text[i].isDigit())
                {
                    containsDigit = true;
                    break;
                }
            }

            if (containsDigit)
            {
                result = text.toDouble();
            }
        }

        emit sendSpeed(result);
    }

public slots:

    void setCurrentPosition(const std::pair<QPointF, double>& pair)
    {
        // currentPositionX->setText(QString("%1, %2").arg(std::round(pair.first.x() * 100.0) / 100.0).arg(std::round(pair.first.y() * 100.0) / 100.0));
        currentPositionX->setText(QString("%1").arg(pair.first.x()));
        currentPositionY->setText(QString("%1").arg(pair.first.y()));

        currentSpeed->setText(QString("%1").arg(pair.second));
    }

    void changeButtonColor(const bool checked)
    {
        if (checked)
        {
            checkButton->setStyleSheet("QPushButton { background-color: red; }");
        }
        else
        {
            checkButton->setStyleSheet("QPushButton { }");
        }
    }

    void changeButton(const bool value)
    {
        checkButton->setChecked(value);

        changeButtonColor(value);
    }

    void changeCheckButton(const bool checked)
    {
        checkButton->setChecked(checked);

        changeButtonColor(checked);

        emit checkBottomChanging(checked);
    }

    // Сброс
    void reset()
    {
        speedInput->setText("1");
        currentPositionX->setText("-");
        currentPositionY->setText("-");
        currentSpeed->setText("-");

        emit sendReset();
    }

private:
    bool drawing;

    QPushButton* checkButton;
    QLineEdit* speedInput;
    QLabel* currentPositionX;
    QLabel* currentPositionY;
    QLabel* currentSpeed;
};
