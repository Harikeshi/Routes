#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Widgets {
/*!
 * Класс управлением ПЛ.
 */
class SubWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit SubWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        const auto mainLayout = new QVBoxLayout(this);

        // Label Submarine
        speedLabel = new QLabel("Submarine: ", this);
        mainLayout->addWidget(speedLabel);

        // Label SP: [     ]
        const auto enterLayout = new QHBoxLayout();
        enterLayout->addWidget(new QLabel("Speed: ", this));
        speedInput = new QLineEdit(this);
        speedInput->setText("10");
        emit sendSpeedChanged(speedInput->text().toInt());

        QIntValidator* validator = new QIntValidator(1, 30, speedInput);
        speedInput->setValidator(validator);

        enterLayout->addWidget(speedInput);
        mainLayout->addLayout(enterLayout);

        // [ Set Path ]
        checkButton = new QPushButton("Set Path", this);
        checkButton->setChecked(false);
        checkButton->setCheckable(true); // Включаем функцию фиксации
        checkButton->setStyleSheet("QPushButton { }");
        mainLayout->addWidget(checkButton);

        // [ Reset ]
        resetButton = new QPushButton("Reset", this);
        mainLayout->addWidget(resetButton);

        connect(speedInput, &QLineEdit::textChanged, this, &SubWidget::actualSpeed);
        connect(this, &SubWidget::sendSpeedChanged, this, &SubWidget::changeSpeed);

        connect(checkButton, &QPushButton::toggled, this, &SubWidget::changeDrawing);
        connect(resetButton, &QPushButton::clicked, this, &SubWidget::reset);
    }

signals:
    void checkBottomChanged();
    void sendSpeedChanged(int);
    void resetButtomPushed();

    void sendReset();

private slots:
    void changeSpeed(double speed)
    {
        speedLabel->setText("Submarine: " + QString("%1").arg(speed) + " m/s.");
    }

    void changeDrawing()
    {
        emit checkBottomChanged();
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

        emit sendSpeedChanged(result);
    }

public slots:

    // setButtonColor
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

    void setSpeedInput(double speed)
    {
        speedInput->setText(QString("%1").arg(speed));
    }

    // Сброс
    void reset()
    {
        // TODO: Скорость к дефолтной из файла например
        emit resetButtomPushed();
    }

private:
    QLabel* speedLabel;
    QLineEdit* speedInput;

    QPushButton* checkButton;
    QPushButton* resetButton;
};
} // namespace Widgets
