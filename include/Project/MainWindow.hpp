#pragma once

#include <QAction>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QSplitter>
#include <QWidget>

#include "JsonOperations.hpp"

#include "DataWidget.hpp"
#include "InformationWidget.hpp"
#include "SubmarineWidget.hpp"
#include "VisWidget.hpp"

// TODO: Переделать на QJson

class MainWindow : public QMainWindow
{
    using Route = Visual::Models::Route;
    using Perimeter = Visual::Models::Perimeter;

    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        // Меню
        QMenu* fileMenu = menuBar()->addMenu("Файл");
        QAction* openRequest = new QAction("Загрузить Request.json", this);
        QAction* openResult = new QAction("Загрузить Result.json", this);
        fileMenu->addAction(openRequest);
        fileMenu->addAction(openResult);

        // Главный splitter (горизонтальный)
        QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);

        // Левый виджет
        visWidget = new VisWidget(this);
        visWidget->resize(800, 800);
        QVBoxLayout* leftLayout = new QVBoxLayout(visWidget);
        visWidget->setLayout(leftLayout);

        // Правый splitter
        QSplitter* rightSplitter = new QSplitter(Qt::Vertical, this);

        // Верхний правый виджет
        dataWidget = new DataWidget(this);
        QVBoxLayout* rightTopLayout = new QVBoxLayout(dataWidget);
        dataWidget->setLayout(rightTopLayout);

        // Средний правый виджет
        subWidget = new SubmarineWidget(this);
        QVBoxLayout* rightMiddleLayout = new QVBoxLayout(subWidget);
        subWidget->setLayout(rightMiddleLayout);

        // Нижний правый виджет
        infoWidget = new InformationWidget(this);
        QVBoxLayout* rightBottomLayout = new QVBoxLayout(infoWidget);
        infoWidget->setLayout(rightBottomLayout);

        // Добавляем правые виджеты в правый splitter
        rightSplitter->addWidget(dataWidget);
        rightSplitter->addWidget(subWidget);
        rightSplitter->addWidget(infoWidget);

        // Добавляем левый visWidget и правый splitter в главный splitter
        mainSplitter->addWidget(visWidget);
        mainSplitter->addWidget(rightSplitter);

        // Устанавливаем главный splitter как центральный виджет
        setCentralWidget(mainSplitter);

        // Настройка размеров splitter
        mainSplitter->setHandleWidth(15);
        rightSplitter->setHandleWidth(10);
        mainSplitter->setStretchFactor(2, 3);  // Левый виджет занимает 2/3 пространства
        rightSplitter->setStretchFactor(1, 1); // Правый splitter занимает 1/3 пространства

        connect(openRequest, &QAction::triggered, this, &MainWindow::initPerimeter);
        connect(openResult, &QAction::triggered, this, &MainWindow::initRoutes);

        // SubWidget
        connect(subWidget, &SubmarineWidget::checkBottomChanging, visWidget, &VisWidget::setDrawing);
        connect(subWidget, &SubmarineWidget::sendReset, visWidget, &VisWidget::targetClear);
        connect(subWidget, &SubmarineWidget::sendSpeed, visWidget, &VisWidget::setTargetSpeed);
        connect(subWidget, &SubmarineWidget::startRandom, visWidget, &VisWidget::startRandomTarget);

        // VisWidget
        connect(visWidget, &VisWidget::sendChangedDrawing, subWidget, &SubmarineWidget::changeButton);
        connect(visWidget, &VisWidget::sendTargetPosition, subWidget, &SubmarineWidget::setCurrentPosition);
        connect(visWidget, &VisWidget::sendTargetSpeed, subWidget, &SubmarineWidget::setCurrentSpeed);
        connect(visWidget, &VisWidget::sendTargetSpeed, subWidget, &SubmarineWidget::setSpeedInput);
        connect(visWidget, &VisWidget::sendIntersectionResult, this, &MainWindow::setIntersectionInfo);
    }

private slots:
    void setIntersectionInfo(const QString& str)
    {
        infoWidget->addMessage(str, InformationWidget::MessageType::Success);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        QMainWindow::mousePressEvent(event);
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        // TODO: Сделать switch
        if (event->key() == Qt::Key_Equal)
        {
            visWidget->upSpeed(1);
        }
        // Сброс скрости к базовой
        else if (event->key() == Qt::Key_Plus)
        {
            visWidget->resetSpeed();
        }
        else if (event->key() == Qt::Key_Minus)
        // Уменьшить скорость
        {
            visWidget->downSpeed(2);
        }
        else if (event->key() == Qt::Key_S)
        {
            try
            {
                visWidget->start();
            }
            catch (std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
        }
        else if (event->key() == Qt::Key_L)
        {
            visWidget->changeShowLines();
        }
        else if (event->key() == Qt::Key_B)
        {
            visWidget->showCompletedRoutes();
        }
        else if (event->key() == Qt::Key_P)
        {
            visWidget->pause();
        }
        else if (event->key() == Qt::Key_F)
        {
            try
            {
                visWidget->setupP();
                infoWidget->addMessage("Периметер загружен.", InformationWidget::MessageType::Info);
            }
            catch (const std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
        }
        else if (event->key() == Qt::Key_R)
        {
            try
            {
                visWidget->setupR();
                infoWidget->addMessage("Пути загружены.", InformationWidget::MessageType::Info);
            }
            catch (const std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
        }
        else if (event->key() == Qt::Key_X)
        {
            visWidget->setDrawing(true);
        }
        // Очистка путей и точки цели
        else if (event->key() == Qt::Key_C)
        {
            visWidget->targetClear(); // Tagret clear()
        }

        visWidget->update();
    }

    // TODO: информация о загрузке файлов
    // TODO: Вынести в отдельный класс Json
    QByteArray openJsonFile(const QString& title)
    {
        QString fileName = QFileDialog::getOpenFileName(this, title, "", "JSON Files (*.json)");

        QByteArray barray;

        if (fileName.isEmpty())
        {
            throw std::runtime_error("Не удалось открыть файл. Операция отменена пользователем.");
        }

        QFile file(fileName);

        // Проверка
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            file.close();
            throw std::runtime_error("Не удалось открыть файл.");
        }

        barray = file.readAll();

        file.close();

        return barray;
    }
    // TODO: to JsonOper
    QJsonObject byteArrayToJson(const QByteArray& array)
    {
        QJsonDocument doc = QJsonDocument::fromJson(array);

        if (doc.isNull() || !doc.isObject())
        {
            throw std::runtime_error("Файл не удалось открыть.");
        }

        QJsonObject obj;
        try
        {
            obj = doc.object();
        }
        catch (std::runtime_error& ex)
        {
            throw ex;
        }

        return obj;
    }
private slots:

    void initRoutes() // TODO:
    {
        QJsonObject json;

        std::string fileName;
        try
        {
            auto array = openJsonFile("Открыть result.json файл(Выходные данные).");
            json = byteArrayToJson(array);
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            return;
        }

        try
        {
            QVector<Route> routes = JsonHelpers::parseToRoutes(json);

            visWidget->setRoutes(routes);

            //  visWidget->update();
        }
        catch (std::runtime_error& ex)
        {
            // TODO: Обработка ошибки
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);

            return;
        }

        infoWidget->addMessage("Пути успешно загружены.", InformationWidget::MessageType::Success);
    }

    void initPerimeter() // TODO:
    {
        QJsonObject json;
        try
        {
            auto array = openJsonFile("Открыть request.json файл(Входные данные).");
            json = byteArrayToJson(array);
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);

            return;
        }

        try
        {
            Perimeter perimeter = JsonHelpers::parseToPolygon(json);

            visWidget->setPerimeter(perimeter);

            dataWidget->clear();
            dataWidget->initialize(json);

            visWidget->update();
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            return;
        }

        infoWidget->addMessage("Периметр успешно загружен.", InformationWidget::MessageType::Success);
    }

private:
    // Должны хранить данные кораблей
    // TODO: Указатель на абстрактную сцену
    VisWidget* visWidget;

    DataWidget* dataWidget;
    InformationWidget* infoWidget;
    SubmarineWidget* subWidget;
};
