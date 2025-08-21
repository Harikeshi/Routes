#pragma once

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPen>
#include <QTimer>
#include <cmath>

class TrajectorySegment {
public:
    QLineF line;
    double speed;

    TrajectorySegment(const QPointF &p1, const QPointF &p2, double s)
            : line(p1, p2), speed(s) {}
};

class MovingObject {
public:
    QVector<QPointF> path;
    QVector<double> segmentLengths;
    QVector<double> speedFactors;
    double totalProgress = 0.0;
    int currentSegment = 0;

    void addSegment(const QLineF &segment, double speedFactor = 1.0) {
        if (path.isEmpty()) {
            path.append(segment.p1());
        }
        path.append(segment.p2());
        segmentLengths.append(segment.length());
        speedFactors.append(speedFactor);
    }

    QPointF getCurrentPosition() const {
        if (path.size() < 2) return QPointF();

        double remainingProgress = totalProgress;
        for (int i = 0; i < segmentLengths.size(); ++i) {
            double segProgress = remainingProgress / segmentLengths[i];

            if (segProgress <= 1.0 || i == segmentLengths.size() - 1) {
                return QLineF(path[i], path[i + 1]).pointAt(qMin(segProgress, 1.0));
            }

            remainingProgress -= segmentLengths[i];
        }

        return path.last();
    }
};

class Formation : public QGraphicsItem {
public:
    QVector<TrajectorySegment> mainTrajectory;
    QVector<MovingObject> objects;
    double distanceBetweenObjects;
    bool showTrajectories = true;
    double formationProgress = 0.0;
    double formationSpeed = 0.5; // Скорость движения формации (0-1)

    Formation(const QVector<TrajectorySegment> &trajectory,
              int objectCount, double distance)
            : mainTrajectory(trajectory), distanceBetweenObjects(distance) {
        // Создаем объекты
        for (int i = 0; i < objectCount; ++i) {
            objects.append(MovingObject());
        }
        calculateTrajectories();
    }

    void calculateTrajectories() {
        if (mainTrajectory.isEmpty() || objects.isEmpty()) return;

        // Рассчитываем основную траекторию как единый путь
        QVector<QPointF> mainPath;
        QVector<double> mainLengths;

        mainPath.append(mainTrajectory[0].line.p1());
        for (const auto &segment: mainTrajectory) {
            mainPath.append(segment.line.p2());
            mainLengths.append(segment.line.length());
        }

        // Рассчитываем траектории для каждого объекта
        for (int i = 0; i < objects.size(); ++i) {
            double offset = distanceBetweenObjects * (i - (objects.size() - 1) / 2.0);

            // Для каждого сегмента основной траектории
            for (int j = 0; j < mainTrajectory.size(); ++j) {
                const auto &segment = mainTrajectory[j];

                if (j == 0) {
                    // Первый сегмент - добавляем начальную позицию
                    QLineF perpendicular = getPerpendicular(segment.line, 1.0);
                    QPointF startPos = segment.line.p1() + perpendicular.p2() * offset;
                    objects[i].addSegment(QLineF(startPos, segment.line.p1()));
                }

                // Добавляем основной сегмент с учетом смещения
                if (j < mainTrajectory.size() - 1) {
                    // Для всех сегментов кроме последнего
                    QLineF adjustedSegment = calculateOffsetSegment(j, offset);
                    objects[i].addSegment(adjustedSegment, 1.0);
                } else {
                    // Последний сегмент - просто параллельное смещение
                    QLineF lastSegment = segment.line;
                    QLineF normal = getPerpendicular(lastSegment, offset);
                    objects[i].addSegment(QLineF(lastSegment.p1() + normal.p2(),
                                                 lastSegment.p2() + normal.p2()));
                }
            }
        }
    }

    QRectF boundingRect() const override {
        QRectF rect;
        for (const auto &obj: objects) {
            for (const auto &point: obj.path) {
                rect |= QRectF(point, QSizeF(1, 1));
            }
        }
        return rect.adjusted(-50, -50, 50, 50);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // Рисуем основную траекторию
        QPen mainPen(Qt::blue, 2, Qt::DashLine);
        painter->setPen(mainPen);
        for (const auto &segment: mainTrajectory) {
            painter->drawLine(segment.line);
        }

        if (showTrajectories) {
            // Рисуем траектории объектов
            QPen objTrajPen(Qt::gray, 1, Qt::DotLine);
            painter->setPen(objTrajPen);
            for (const auto &obj: objects) {
                for (int i = 0; i < obj.path.size() - 1; ++i) {
                    painter->drawLine(obj.path[i], obj.path[i + 1]);
                }
            }
        }

        // Рисуем объекты
        QPen objPen(Qt::black, 2);
        QBrush objBrush(Qt::red);
        painter->setPen(objPen);
        painter->setBrush(objBrush);

        for (const auto &obj: objects) {
            QPointF pos = obj.getCurrentPosition();
            painter->drawEllipse(pos, 5, 5);
        }

        // Рисуем линию строя
        if (objects.size() >= 2) {
            QLineF formationLine(objects.first().getCurrentPosition(),
                                 objects.last().getCurrentPosition());
            QPen formationPen(Qt::green, 1, Qt::DashLine);
            painter->setPen(formationPen);
            painter->drawLine(formationLine);
        }
    }

    void advance(int phase) override {
        if (!phase) return;

        // Обновляем прогресс всей формации
        formationProgress += formationSpeed * 0.01;
        if (formationProgress > 1.0) formationProgress = 0.0;

        // Рассчитываем позиции всех объектов на основе общего прогресса
        for (auto &obj: objects) {
            updateObjectProgress(obj);
        }

        update();
    }

private:
    QLineF getPerpendicular(const QLineF &line, double length) {
        QLineF normal = line.normalVector();
        normal.setLength(length);
        return normal;
    }

    QLineF calculateOffsetSegment(int segmentIndex, double offset) {
        const auto &currentSeg = mainTrajectory[segmentIndex].line;
        const auto &nextSeg = mainTrajectory[segmentIndex + 1].line;

        // Угол между сегментами
        double angle = currentSeg.angleTo(nextSeg);

        if (qFuzzyIsNull(angle)) {
            // Прямая линия - просто параллельное смещение
            QLineF normal = getPerpendicular(currentSeg, offset);
            return QLineF(currentSeg.p1() + normal.p2(),
                          currentSeg.p2() + normal.p2());
        } else {
            // Поворот - строим дугу
            QPointF center = currentSeg.p2();
            double radius = qAbs(offset);

            // Начальная точка дуги
            QLineF radiusLine(center, currentSeg.p1());
            radiusLine.setAngle(radiusLine.angle() + (offset > 0 ? 90 : -90));
            radiusLine.setLength(radius);
            QPointF arcStart = radiusLine.p2();

            // Конечная точка дуги
            radiusLine.setAngle(radiusLine.angle() + angle);
            QPointF arcEnd = radiusLine.p2();

            return QLineF(arcStart, arcEnd);
        }
    }

    void updateObjectProgress(MovingObject &obj) {
        // Рассчитываем общую длину пути объекта
        double totalLength = 0.0;
        for (double len: obj.segmentLengths) {
            totalLength += len;
        }

        // Пройденное расстояние для этого объекта
        double traveled = formationProgress * totalLength;

        // Находим текущий сегмент и прогресс на нем
        obj.currentSegment = 0;
        double remaining = traveled;

        while (obj.currentSegment < obj.segmentLengths.size() - 1 &&
               remaining > obj.segmentLengths[obj.currentSegment]) {
            remaining -= obj.segmentLengths[obj.currentSegment];
            obj.currentSegment++;
        }

        obj.totalProgress = traveled;
    }
};

class MainWindow1
        : public QGraphicsView {
public:
    MainWindow1(QWidget *parent = nullptr) : QGraphicsView(parent) {
        scene = new QGraphicsScene(this);
        setScene(scene);

        // Создаем основную траекторию (более сложную для демонстрации)
        QVector<TrajectorySegment> mainTraj;
        mainTraj.append(TrajectorySegment(QPointF(50, 100), QPointF(200, 100), 10.0));
        mainTraj.append(TrajectorySegment(QPointF(200, 100), QPointF(300, 50), 8.0));
        mainTraj.append(TrajectorySegment(QPointF(300, 50), QPointF(400, 150), 8.0));
        mainTraj.append(TrajectorySegment(QPointF(400, 150), QPointF(500, 100), 8.0));
        mainTraj.append(TrajectorySegment(QPointF(500, 100), QPointF(600, 100), 10.0));

        // Создаем формацию из 5 объектов с расстоянием 15 между ними
        formation = new Formation(mainTraj, 5, 15.0);
        scene->addItem(formation);

        // Настройка таймера для анимации
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
        timer->start(30);

        // Настройка вида
        setRenderHint(QPainter::Antialiasing);
        setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

        // Добавляем элементы управления
        addControlPanel();
    }

protected:
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Space) {
            formation->showTrajectories = !formation->showTrajectories;
            formation->update();
        } else if (event->key() == Qt::Key_Plus) {
            formation->formationSpeed += 0.05;
        } else if (event->key() == Qt::Key_Minus) {
            formation->formationSpeed = qMax(0.05, formation->formationSpeed - 0.05);
        }
        QGraphicsView::keyPressEvent(event);
    }

private:
    QGraphicsScene *scene;
    Formation *formation;
    QTimer *timer;

    void addControlPanel() {
        QGraphicsTextItem *text = scene->addText("Управление:\nПробел - траектории\n+/- - скорость");
        text->setPos(10, 10);
        text->setDefaultTextColor(Qt::black);
    }
};