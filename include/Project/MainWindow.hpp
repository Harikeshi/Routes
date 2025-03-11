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
        visWidget->setMouseTracking(false); // Отключить трекинг мыши

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

        connect(openRequest, &QAction::triggered, this, &MainWindow::initFromRequest);
        connect(openResult, &QAction::triggered, this, &MainWindow::initFromReport);

        // SubWidget
        connect(subWidget, &SubmarineWidget::checkBottomChanging, visWidget, &VisWidget::setDrawing);
        connect(subWidget, &SubmarineWidget::sendReset, visWidget, &VisWidget::targetClear);
        connect(subWidget, &SubmarineWidget::sendSpeed, visWidget, &VisWidget::setTargetSpeed);

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
    void keyPressEvent(QKeyEvent* event) override
    {
        switch (event->key())
        {
        case Qt::Key_Equal:
            visWidget->upSpeed(1);
            break;
        case Qt::Key_Plus:
            visWidget->resetSpeed();
            break;
        case Qt::Key_Minus:
            // Уменьшить скорость
            visWidget->downSpeed(2);
            break;
        case Qt::Key_S:
            try
            {
                this->start();
            }
            catch (std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
            break;
        case Qt::Key_L:
            visWidget->setShowLines();
            break;
        case Qt::Key_B:
            visWidget->setShowFull();
            break;
        case Qt::Key_P:
            visWidget->pause();
            break;
        case Qt::Key_F:
            try
            {
                // initFromRequest
                this->loadRequest();
                infoWidget->addMessage("Request загружен.", InformationWidget::MessageType::Info);
            }
            catch (const std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
            break;
        case Qt::Key_R:
            try
            {
                // initFromReport
                this->loadReport();
                infoWidget->addMessage("Report загружен.", InformationWidget::MessageType::Info);
            }
            catch (const std::runtime_error& ex)
            {
                infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            }
            break;
        case Qt::Key_X:
            visWidget->setDrawing(true);
            break;
        // Очистка путей и точки цели
        case Qt::Key_C:
            visWidget->targetClear(); // Tagret clear()
            break;
        }
    }

    QString openJsonFile(const QString& title)
    {
        auto fileName = QFileDialog::getOpenFileName(this, title, "", "JSON Files (*.json)");

        if (fileName.isEmpty())
        {
            throw std::runtime_error("Не удалось открыть файл. Операция отменена пользователем.");
        }

        return fileName;
    }

private slots:

    void initFromReport()
    {
        try
        {
            auto path = openJsonFile("Открыть result.json файл(Выходные данные).");
            this->loadReport(path);
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);
            return;
        }

        infoWidget->addMessage("Пути успешно загружены.", InformationWidget::MessageType::Success);
    }

    // TODO: Добавить любую загрузку request.json
    void initFromRequest() // TODO:
    {
        QJsonObject json;

        try
        {
            auto path = openJsonFile("Открыть request.json файл(Входные данные).");
            json = this->loadRequest(path);
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), InformationWidget::MessageType::Error);

            return;
        }

        // Инициализация дерева request.json
        dataWidget->initialize(json);

        infoWidget->addMessage("Периметр успешно загружен.", InformationWidget::MessageType::Success);
    }

protected:
    QJsonObject loadReport(const QString path = "/home/harikeshi/work/routes/tresult.json")
    {
        // TODO: логически можно убрать
        if (!requestLoaded_)
            throw std::runtime_error("Периметер не загружен!");

        QJsonObject obj = JsonHelpers::jsonFromFile(path);

        visWidget->setRoutes(JsonHelpers::parseToRoutes(obj));

        reportLoaded_ = true;

        // Проверка если загрузка происходила не в начале
        if (requestLoaded_)
        {
            // Обнуляем пределы и еще раз вычисляем
            visWidget->resetLimits();
            visWidget->initLimitsFromPerimeter();
            visWidget->setRoutesRadius();
        }
        // Инициализация пределов
        visWidget->initLimitsFromRoutes();

        // Инициализация движущихся Объектов.
        visWidget->setRoutesModels(Visual::Objects::Arrow, 0.01 * visWidget->getLimits().getMaxBorderLength()); // 1%

        // Запустить Начальную сцену
        if (requestLoaded_ && reportLoaded_)
            visWidget->loadFirstScreen();

        return obj;
    }

    QJsonObject loadRequest(const QString& path = "/home/harikeshi/work/routes/trequest.json")
    {
        QJsonObject obj = JsonHelpers::jsonFromFile(path);

        // TODO: Если не загрузили возможно надо откатить
        visWidget->setPerimeter(JsonHelpers::parseToPolygon(obj));

        visWidget->setShipParameters(JsonHelpers::parseShip(obj));

        visWidget->setTargetParameters(JsonHelpers::parseTarget(obj));

        requestLoaded_ = true;

        if (reportLoaded_)
        {
            visWidget->resetLimits();
            visWidget->initLimitsFromRoutes();
        }

        visWidget->initLimitsFromPerimeter();

        // Запустить начальную сцену
        if (requestLoaded_ && reportLoaded_)
            visWidget->loadFirstScreen();

        return obj;
    }

    void start()
    {
        if (!requestLoaded_)
        {
            throw std::runtime_error("Request не загружен.");
        }
        if (!reportLoaded_)
        {
            throw std::runtime_error("Report не загружен.");
        }

        visWidget->start();
    }

private:
    // Должны хранить данные кораблей
    // TODO: Указатель на абстрактную сцену
    VisWidget* visWidget;

    DataWidget* dataWidget;
    InformationWidget* infoWidget;
    SubmarineWidget* subWidget;

    bool requestLoaded_ = false;
    bool reportLoaded_ = false;
};
