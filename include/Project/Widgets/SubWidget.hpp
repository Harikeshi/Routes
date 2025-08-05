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
        drawing = false;

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

        // [ Straight | Twisting ]
        paintButton = new QPushButton("Straight");
        paintButton->setChecked(false);
        paintButton->setCheckable(true); // Включаем функцию фиксации
        connect(paintButton, &QPushButton::toggled, this, [this](bool checked) {
            paintButton->isChecked() ? paintButton->setText("Straight") : paintButton->setText("Winding");
        });

        paintButton->setEnabled(false);
        mainLayout->addWidget(paintButton);

        // [ Reset ]
        resetButton = new QPushButton("Reset", this);
        mainLayout->addWidget(resetButton);

        // this->setLayout(mainLayout); // Не нужно, т.к. передали this в конструктор QVBoxLayout

        connect(speedInput, &QLineEdit::textChanged, this, &SubWidget::actualSpeed);
        connect(this, &SubWidget::sendSpeedChanged, this, &SubWidget::changeSpeed);

        connect(checkButton, &QPushButton::toggled, this, &SubWidget::changeDrawing);
        //  connect(paintButton, &QPushButton::toggled, this, &SubWidget::changeDrawing);
        connect(resetButton, &QPushButton::clicked, this, &SubWidget::reset);
    }

signals:
    void checkBottomChanged();
    void paintBottomChanged();
    void sendSpeedChanged(int);
    void resetButtomPushed();

    void sendReset();
    void sendSpeed(double);

private slots:
    void changeSpeed(double speed)
    {
        speedLabel->setText("Submarine: " + QString("%1").arg(speed) + " m/s.");
    }

    void changeDrawing()
    {
        emit checkBottomChanged();
    }

    void changePainting()
    {
        emit paintBottomChanged();
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

    static void changeButtonImage(QPushButton* button, const bool checked)
    {
        // TODO: Добавить картинки какие требуется поменять
        if (checked)
        {
            button->setStyleSheet("QPushButton { background-color: red; }");
        }
        else
        {
            button->setStyleSheet("QPushButton { }");
        }

        button->update();
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
    bool drawing;

    QLabel* speedLabel;
    QLineEdit* speedInput;

    QPushButton* checkButton;
    QPushButton* paintButton;
    QPushButton* resetButton;
};
} // namespace Widgets
