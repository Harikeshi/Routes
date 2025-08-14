#pragma once

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>

namespace Widgets {
class UpdateProgressBar final : public QWidget
{
    Q_OBJECT

public:
    UpdateProgressBar(QWidget* parent = nullptr)
        : QWidget(parent), m_totalSeconds(7200), m_currentSeconds(1800)
    {
        setMinimumHeight(40);
        setMouseTracking(false);
    }

public slots:
    void setTotalTime(int seconds)
    {
        m_totalSeconds = qMax(1, seconds);
        m_currentSeconds = 0;

        update();
    }

    void setCurrentTime(int seconds)
    {
        seconds = qBound(0, seconds, m_totalSeconds);
        if (m_currentSeconds != seconds)
        {
            m_currentSeconds = seconds;
            update();
        }
    }

    void clear()
    {
        m_currentSeconds = 0;

        update();
    }
    void reset()
    {
        m_totalSeconds = 7200;
        m_currentSeconds = 1800;

        update();
    }

signals:
    void timeChanged(double newTime);

protected:
    void paintEvent(QPaintEvent*) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Фон
        painter.fillRect(rect(), QColor(50, 50, 50));

        // Основные параметры шкалы
        int margin = 10;
        int scaleHeight = 8;
        int scaleY = height() - scaleHeight - 15;
        QRect scaleRect(margin, scaleY, width() - 2 * margin, scaleHeight);

        // Средняя линия
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawLine(scaleRect.left(), scaleRect.center().y(), scaleRect.right(), scaleRect.center().y());

        // Прогресс
        int progressWidth = scaleRect.width() * m_currentSeconds / m_totalSeconds;
        painter.setPen(QPen(QColor(0, 150, 200), 3));
        painter.drawLine(scaleRect.left(), scaleRect.center().y(), scaleRect.left() + progressWidth, scaleRect.center().y());

        //        // Временные метки
        //        // TODO: Делить по минутам.
        //        painter.setPen(Qt::white);
        //        QFont font = painter.font();
        //        font.setPixelSize(9);
        //        painter.setFont(font);
        //
        //        int totalMinutes = m_totalSeconds / 60;
        //        int step = (totalMinutes > 120) ? 30 : 15;
        //
        //        for (int minutes = 0; minutes <= totalMinutes; minutes += step)
        //        {
        //            int x = margin + (width() - 2 * margin) * minutes / totalMinutes;
        //            // Вертикальная линия метки
        //            painter.drawLine(x, scaleRect.top() - 5, x, scaleRect.bottom());
        //
        //            // Текст времени
        //            QString timeText = QString("%1:%2")
        //                                   .arg(minutes / 60)
        //                                   .arg(minutes % 60, 2, 10, QChar('0'));
        //            QRect textRect(x - 20, scaleRect.bottom() + 2, 40, 12);
        //            painter.drawText(textRect, Qt::AlignCenter, timeText);
        //        }

        // Ползунок
        int currentX = margin + (width() - 2 * margin) * m_currentSeconds / m_totalSeconds;
        painter.setBrush(QColor(255, 100, 100));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPoint(currentX, scaleRect.center().y()), 6, 6);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            updateTimeFromMouse(event->pos());
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            updateTimeFromMouse(event->pos());
        }
    }

private:
    void updateTimeFromMouse(const QPoint& pos)
    {
        int margin = 10;
        int x = qBound(margin, pos.x(), width() - margin);
        setCurrentTime((x - margin) * m_totalSeconds / (width() - 2 * margin));

        emit timeChanged(m_currentSeconds);
    }

    int m_totalSeconds;
    int m_currentSeconds;
};
} // namespace Widgets