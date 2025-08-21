#pragma once

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <cmath>

class MovingObjects : public QWidget {
Q_OBJECT
public:
    MovingObjects(QWidget *parent = nullptr) : QWidget(parent) {
        // Инициализация сцены и представления
        scene = new QGraphicsScene(this);
        view = new QGraphicsView(scene);
        view->setSceneRect(0, 0, 600, 400);

        // Создаем элементы управления
        QLabel *countLabel = new QLabel("Количество объектов (2-5):");
        countSlider = new QSlider(Qt::Horizontal);
        countSlider->setRange(2, 5);
        countSlider->setValue(3);

        QLabel *distLabel = new QLabel("Расстояние между объектами:");
        distSlider = new QSlider(Qt::Horizontal);
        distSlider->setRange(20, 100);
        distSlider->setValue(50);

        QPushButton *turnLeftBtn = new QPushButton("Повернуть налево");
        QPushButton *turnRightBtn = new QPushButton("Повернуть направо");

        // Размещаем элементы в layout
        QVBoxLayout *mainLayout = new QVBoxLayout;
        QHBoxLayout *controlLayout = new QHBoxLayout;

        controlLayout->addWidget(turnLeftBtn);
        controlLayout->addWidget(turnRightBtn);

        mainLayout->addWidget(view);
        mainLayout->addWidget(countLabel);
        mainLayout->addWidget(countSlider);
        mainLayout->addWidget(distLabel);
        mainLayout->addWidget(distSlider);
        mainLayout->addLayout(controlLayout);

        setLayout(mainLayout);

        // Создаем таймер для анимации
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MovingObjects::moveObjects);
        timer->start(30);

        // Связываем элементы управления
        connect(countSlider, &QSlider::valueChanged, this, &MovingObjects::updateScene);
        connect(distSlider, &QSlider::valueChanged, this, &MovingObjects::updateScene);
        connect(turnLeftBtn, &QPushButton::clicked, this, &MovingObjects::turnLeft);
        connect(turnRightBtn, &QPushButton::clicked, this, &MovingObjects::turnRight);

        // Инициализация
        currentDirection = 0; // 0 - вправо, 1 - вниз, 2 - влево, 3 - вверх
        updateScene();
    }

private slots:

    void updateScene() {
        scene->clear();
        objects.clear();

        int count = countSlider->value();
        int distance = distSlider->value();

        // Создаем объекты
        for (int i = 0; i < count; ++i) {
            QGraphicsRectItem *rect = new QGraphicsRectItem(-15, -15, 30, 30);
            rect->setBrush(QColor::fromHsv(i * 360 / count, 255, 200));

            // Начальная позиция в зависимости от направления
            if (currentDirection == 0) { // вправо
                rect->setPos(50, 200 + (i - count / 2.0 + 0.5) * distance);
            } else if (currentDirection == 1) { // вниз
                rect->setPos(300 + (i - count / 2.0 + 0.5) * distance, 50);
            } else if (currentDirection == 2) { // влево
                rect->setPos(550, 200 + (i - count / 2.0 + 0.5) * distance);
            } else { // вверх
                rect->setPos(300 + (i - count / 2.0 + 0.5) * distance, 350);
            }

            scene->addItem(rect);
            objects.append(rect);
        }

        // Сбрасываем позицию
        movementProgress = 0;
    }

    void moveObjects() {
        int distance = distSlider->value();
        int count = objects.size();
        float speed = 2.0;

        // Увеличиваем прогресс движения
        movementProgress += speed;

        // Двигаем все объекты в зависимости от направления
        for (int i = 0; i < count; ++i) {
            QGraphicsRectItem *rect = objects[i];

            if (currentDirection == 0) { // движение вправо
                rect->setPos(50 + movementProgress,
                             200 + (i - count / 2.0 + 0.5) * distance);
            } else if (currentDirection == 1) { // движение вниз
                rect->setPos(300 + (i - count / 2.0 + 0.5) * distance,
                             50 + movementProgress);
            } else if (currentDirection == 2) { // движение влево
                rect->setPos(550 - movementProgress,
                             200 + (i - count / 2.0 + 0.5) * distance);
            } else { // движение вверх
                rect->setPos(300 + (i - count / 2.0 + 0.5) * distance,
                             350 - movementProgress);
            }

            // Поворачиваем прямоугольник в соответствии с направлением
            rect->setRotation(90 * currentDirection);
        }

        // Если достигли конца пути, останавливаемся
        if (movementProgress >= 500) {
            timer->stop();
        }
    }

    void turnLeft() {
        currentDirection = (currentDirection + 3) % 4; // поворот налево (270 градусов)
        startMovement();
    }

    void turnRight() {
        currentDirection = (currentDirection + 1) % 4; // поворот направо (90 градусов)
        startMovement();
    }

    void startMovement() {
        movementProgress = 0;
        if (!timer->isActive()) {
            timer->start();
        }
        updateScene();
    }

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QList<QGraphicsRectItem *> objects;
    QTimer *timer;
    QSlider *countSlider;
    QSlider *distSlider;

    int currentDirection; // 0 - вправо, 1 - вниз, 2 - влево, 3 - вверх
    float movementProgress;
};


//
//#include <QApplication>
//#include <QGraphicsScene>
//#include <QGraphicsRectItem>
//#include <QGraphicsView>
//#include <QTimer>
//#include <QHBoxLayout>
//#include <QSlider>
//#include <QLabel>
//#include <QWidget>
//
//class MovingObjects : public QWidget {
//Q_OBJECT
//public:
//    MovingObjects(QWidget *parent = nullptr) : QWidget(parent) {
//        // Создаем сцену и представление
//        scene = new QGraphicsScene(this);
//        view = new QGraphicsView(scene);
//
//        // Создаем элементы управления
//        QLabel *countLabel = new QLabel("Количество объектов (2-5):");
//        countSlider = new QSlider(Qt::Horizontal);
//        countSlider->setRange(2, 5);
//        countSlider->setValue(3);
//
//        QLabel *distLabel = new QLabel("Расстояние между объектами:");
//        distSlider = new QSlider(Qt::Horizontal);
//        distSlider->setRange(10, 100);
//        distSlider->setValue(30);
//
//        // Размещаем элементы в layout
//        QVBoxLayout *layout = new QVBoxLayout;
//        layout->addWidget(view);
//        layout->addWidget(countLabel);
//        layout->addWidget(countSlider);
//        layout->addWidget(distLabel);
//        layout->addWidget(distSlider);
//        setLayout(layout);
//
//        // Создаем таймер для анимации
//        timer = new QTimer(this);
//        connect(timer, &QTimer::timeout, this, &MovingObjects::moveObjects);
//        timer->start(30);
//
//        // Связываем слайдеры с обновлением сцены
//        connect(countSlider, &QSlider::valueChanged, this, &MovingObjects::updateScene);
//        connect(distSlider, &QSlider::valueChanged, this, &MovingObjects::updateScene);
//
//        // Инициализируем сцену
//        updateScene();
//    }
//
//private slots:
//
//    void updateScene() {
//        scene->clear();
//        objects.clear();
//
//        int count = countSlider->value();
//        int distance = distSlider->value();
//
//        // Создаем объекты
//        for (int i = 0; i < count; ++i) {
//            QGraphicsRectItem *rect = new QGraphicsRectItem(0, 0, 30, 30);
//            rect->setBrush(Qt::blue);
//
//            // Располагаем объекты вертикально с заданным расстоянием
//            rect->setPos(50, 50 + i * distance);
//            scene->addItem(rect);
//            objects.append(rect);
//        }
//
//        // Устанавливаем начальную позицию
//        xPos = 50;
//        direction = 1;
//    }
//
//    void moveObjects() {
//        // Двигаем все объекты горизонтально
//        xPos += direction * 2;
//
//        // Если достигли границы, меняем направление
//        if (xPos > 350 || xPos < 50) {
//            direction *= -1;
//        }
//
//        // Обновляем позиции всех объектов
//        int distance = distSlider->value();
//        for (int i = 0; i < objects.size(); ++i) {
//            objects[i]->setPos(xPos, 50 + i * distance);
//        }
//    }
//
//private:
//    QGraphicsScene *scene;
//    QGraphicsView *view;
//    QList<QGraphicsRectItem *> objects;
//    QTimer *timer;
//    QSlider *countSlider;
//    QSlider *distSlider;
//    int xPos;
//    int direction;
//};
//
//
