#pragma once

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTextStream>
#include <QWidget>
#include <vector>

class PointsWidget : public QWidget
{
    Q_OBJECT

public:
    PointsWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // setMouseTracking(true);
    }

public:
    void mousePress(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            points.push_back(event->pos());
            update(); // Перерисовываем виджет
        }
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            points.push_back(event->pos());
            update(); // Перерисовываем виджет
        }
    }

    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);
        QPainter painter(this);

        // Рисуем все точки
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);

        for (const QPoint& point : points)
        {
            painter.drawEllipse(point, 3, 3);
        }

        // Соединяем точки линиями, если их больше одной
        if (points.size() > 1)
        {
            painter.setPen(Qt::red);
            for (size_t i = 1; i < points.size(); ++i)
            {
                painter.drawLine(points[i - 1], points[i]);
            }
        }
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        if (event->key() == Qt::Key_S)
        {
            // todo:
            // savePointsToFile();
        }
        else if (event->key() == Qt::Key_C)
        {
            points.clear();
            update();
        }
    }

private:
    //    void savePointsToFile()
    //    {
    //        QFile file("points.txt");
    //        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    //        {
    //            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи");
    //            return;
    //        }
    //
    //        QTextStream out(&file);
    //        for (const QPoint& point : points)
    //        {
    //            out << point.x() << " " << point.y() << "\n";
    //        }
    //
    //        file.close();
    //        QMessageBox::information(this, "Сохранено", "Точки сохранены в файл points.txt");
    //    }

    std::vector<QPoint> points;
};