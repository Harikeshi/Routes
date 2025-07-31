#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

namespace Widgets {
/*!
 * Старая версия класса управления ПЛ.
 */
class SubmarineWidget final : public QWidget
{
    Q_OBJECT

public:
    SubmarineWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        drawing = false;

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

        mainLayout->addWidget(checkButton, 6, 0);

        QPushButton* resetButton = new QPushButton("Reset", this);
        mainLayout->addWidget(resetButton, 6, 1);

        QPushButton* randomButton = new QPushButton("Random", this);
        mainLayout->addWidget(randomButton, 7, 0);

        connect(resetButton, &QPushButton::clicked, this, &SubmarineWidget::reset);

        connect(randomButton, &QPushButton::clicked, this, &SubmarineWidget::random);

        connect(checkButton, &QPushButton::toggled, this, &SubmarineWidget::changeDrawing);

        connect(speedInput, &QLineEdit::textChanged, this, &SubmarineWidget::actualSpeed);
    }

signals:
    void checkBottomChanged();

    void sendReset();
    void sendSpeed(double);
    void startRandom();

private slots:
    void changeDrawing()
    {
        emit checkBottomChanged();
    }

    void random()
    {
        emit startRandom();
    }

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
    void setCurrentPosition(const QPointF position)
    {
        currentPositionX->setText(QString("%1").arg(position.x()));
        currentPositionY->setText(QString("%1").arg(position.y()));
    }

    void setCurrentSpeed(const double speed)
    {
        currentSpeed->setText(QString("%1").arg(speed));
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

        checkButton->update();
    }

    void setSpeedInput(const double speed)
    {
        speedInput->setText(QString("%1").arg(speed));
    }

    // Сброс
    void reset()
    {
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
} // namespace Widgets