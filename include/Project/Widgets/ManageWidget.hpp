#pragma once

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace Widgets {
/*!
 * Класс управления движением.
 */
class ManageWidget final : public QWidget
{
    Q_OBJECT
public:
    ManageWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setSpacing(2);                  // Устанавливаем расстояние между элементами
        layout->setContentsMargins(2, 2, 2, 2); // Убираем отступы по краям

        // [ start | stop ]
        playStopButton = new QPushButton(this);
        playStopButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        playStopButton->setCheckable(true);
        layout->addWidget(playStopButton);

        // [ pause ]
        pauseButton = new QPushButton(this);
        pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        layout->addWidget(pauseButton);

        // [ reset ]
        resetButton = new QPushButton("Reset", this);
        layout->addWidget(resetButton);

        // speed up / speed down
        QHBoxLayout* speedLayout = new QHBoxLayout();
        minusButton = new QPushButton(this);
        minusButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
        speedLayout->addWidget(minusButton);

        plusButton = new QPushButton(this);
        plusButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
        speedLayout->addWidget(plusButton);

        layout->addLayout(speedLayout);

        QHBoxLayout* visHasLayout = new QHBoxLayout();
        visionButton = new QPushButton(this);
        visionButton->setText("*"); // Используем эмодзи как временное решение
        visHasLayout->addWidget(visionButton);

        auto* bt = new QPushButton(this);
        bt->setText("$");

        connect(bt, &QPushButton::clicked, this, &ManageWidget::print);

        visHasLayout->addWidget(bt);
        calcButton = new QPushButton("Calc", this);
        visHasLayout->addWidget(calcButton);

        layout->addLayout(visHasLayout);
        setLayout(layout);

        this->initConnections();

    }

signals:
    void sendPlayButtonClicked(bool);
    void sendPauseButtonClicked();

    void sendMinusButtonClicked();
    void sendPlusButtonClicked();
    void clickVisionButton();

    void pushReset();
    void clickedCalculate();

    void btSignal(QString, size_t);
private:
    void print()
    {
        emit btSignal("Hello World!", 1);
    }

protected:
    void initConnections()
    {
        connect(playStopButton, &QPushButton::toggled, this, &ManageWidget::sendPlayButtonClicked);
        connect(pauseButton, &QPushButton::clicked, this, &ManageWidget::sendPauseButtonClicked);
        connect(resetButton, &QPushButton::clicked, this, &ManageWidget::sendReset);
        connect(minusButton, &QPushButton::clicked, this, &ManageWidget::sendMinusButtonClicked);
        connect(plusButton, &QPushButton::clicked, this, &ManageWidget::sendPlusButtonClicked);
        connect(visionButton, &QPushButton::clicked, this, &ManageWidget::visionButtonClicked);
        connect(calcButton, &QPushButton::clicked, this, &ManageWidget::calculateClicked);
    }

    void calculateClicked()
    {
        emit clickedCalculate();
    }

    void sendReset()
    {
        emit pushReset();
    }

public:
    void visionButtonClicked()
    {
        emit clickVisionButton();
    }

    void setPauseButtonImage(bool checked)
    {
        pauseButton->setIcon(style()->standardIcon(checked ? QStyle::SP_MediaPlay : QStyle::SP_MediaPause));
        playStopButton->setIcon(style()->standardIcon(checked ? QStyle::SP_MediaStop : QStyle::SP_MediaPlay));
    }

    void setPlayButtonImage(bool checked)
    {
        playStopButton->setIcon(style()->standardIcon(checked ? QStyle::SP_MediaStop : QStyle::SP_MediaPlay));
        pauseButton->setIcon(style()->standardIcon(checked ? QStyle::SP_MediaPlay : QStyle::SP_MediaPause));
    }

private:
    QPushButton* playStopButton;
    QPushButton* pauseButton;
    QPushButton* resetButton;
    QPushButton* minusButton;
    QPushButton* plusButton;
    QPushButton* visionButton;
    QPushButton* calcButton;
};
} // namespace Widgets
