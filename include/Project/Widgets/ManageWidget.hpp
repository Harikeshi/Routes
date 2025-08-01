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

        // connect(playStopButton, &QPushButton::toggled, this, [this](bool checked) {
        //     playStopButton->setIcon(style()->standardIcon(
        //         checked ? QStyle::SP_MediaStop : QStyle::SP_MediaPlay));
        // });
        connect(playStopButton, &QPushButton::toggled, this, &ManageWidget::sendPlayButtonClicked);

        layout->addWidget(playStopButton);

        // [ pause ]
        pauseButton = new QPushButton(this);
        pauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

        connect(pauseButton, &QPushButton::clicked, this, &ManageWidget::sendPauseButtonClicked);

        layout->addWidget(pauseButton);

        // [ reset ]
        resetButton = new QPushButton("Reset", this);
        //        resetButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
        connect(resetButton, &QPushButton::clicked, this, &ManageWidget::sendReset);
        layout->addWidget(resetButton);

        // speed up / speed down
        QHBoxLayout* speedLayout = new QHBoxLayout();
        minusButton = new QPushButton(this);
        minusButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
        connect(minusButton, &QPushButton::clicked, this, &ManageWidget::sendMinusButtonClicked);
        speedLayout->addWidget(minusButton);

        plusButton = new QPushButton(this);
        plusButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
        connect(plusButton, &QPushButton::clicked, this, &ManageWidget::sendPlusButtonClicked);
        speedLayout->addWidget(plusButton);

        layout->addLayout(speedLayout);

        QHBoxLayout* visHasLayout = new QHBoxLayout();
        visionButton = new QPushButton(this);
        visionButton->setText("*"); // Используем эмодзи как временное решение
        connect(visionButton, &QPushButton::clicked, this, &ManageWidget::visionButtonClicked);

        // connect(visionButton, &QPushButton::toggled, this, [this](bool checked) {
        //     visionButton->setText(checked ? "🚫" : "👁");
        // });
        visHasLayout->addWidget(visionButton);

        //        hasButton = new QPushButton("ГАС", this);
        //        hasButton->setCheckable(true);
        //        // connect(hasButton, &QPushButton::toggled, this, [this](bool checked) {
        //        //     hasButton->setText(checked ? "Г̶А̶С̶" : "ГАС");
        //        // });
        //        hasButton->setEnabled(false);
        //        visHasLayout->addWidget(hasButton);
        calcButton = new QPushButton("Calc", this);
        calcButton->setCheckable(true);
       
        calcButton->setEnabled(false);
        visHasLayout->addWidget(calcButton);
        layout->addLayout(visHasLayout);

        setLayout(layout);
    }

signals:
    void sendPlayButtonClicked(bool);
    void sendPauseButtonClicked();

    void sendMinusButtonClicked();
    void sendPlusButtonClicked();
    void clickVisionButton();

    void pushReset();

protected:
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
