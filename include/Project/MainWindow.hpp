#pragma once

#include <QAction>
#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QSplitter>
#include <QStackedLayout>
#include <QWidget>

#include "./Scene/Actors/InRegion.hpp"
#include "./Widgets/SceneWidget.hpp"

#include "MessageType.hpp"

#include "./Widgets/DataWidget.hpp"
#include "./Widgets/InformationWidget.hpp"
#include "./Widgets/SpeedWidget.hpp"
#include "./Widgets/SubmarineWidget.hpp"
#include "./Widgets/TimeWidget.hpp"

#include "./Data/Initializer.hpp"

class MainWindow : public QMainWindow
{
    using Route = Scene::Objects::Models::Route;
    using Perimeter = Scene::Objects::Models::Perimeter;
    using TimeWidget = Widgets::TimeWidget;
    using CoordinateSystem = Scene::Entities::CoordinateSystem;

    using SceneWidget = Widgets::SceneWidget;
    using SpeedWidget = Widgets::SpeedWidget;

    using DataWidget = Widgets::DataWidget;
    using SubmarineWidget = Widgets::SubmarineWidget;
    using InformationWidget = Widgets::InformationWidget;

    using Initializer = Data::Initializer;
    using Request = Data::Request;
    using Report = Data::Report;

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

        scene = new SceneWidget(this);
        scene->setFocusPolicy(Qt::StrongFocus);

        scene->resize(800, 800);
        scene->setMouseTracking(false); // Отключить трекинг мыши
        QVBoxLayout* leftLayout = new QVBoxLayout(scene);
        scene->setLayout(leftLayout);

        /// start
        timeWidget = new TimeWidget(this);
        speedWidget = new SpeedWidget(this);

        QWidget* upperWidget = new QWidget(this);
        upperWidget->setGeometry(0, 0, 200, 56);                     // Такие же размеры, как у фона
        upperWidget->setAttribute(Qt::WA_TransparentForMouseEvents); // Игнорировать события мыши
        upperWidget->move(50, 0);
        upperWidget->show();
        upperWidget->setStyleSheet("background: transparent;"); // Прозрачный фон

        QVBoxLayout* sceneLayout = new QVBoxLayout(upperWidget);

        sceneLayout->addWidget(timeWidget);
        sceneLayout->addWidget(speedWidget);

        QStackedLayout* stackedLayout = new QStackedLayout(scene);

        stackedLayout->addWidget(upperWidget);
        stackedLayout->setCurrentIndex(1); // Показываем overlayWidget поверх

        // end

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
        mainSplitter->addWidget(scene);

        mainSplitter->addWidget(rightSplitter);

        // Устанавливаем главный splitter как центральный виджет
        setCentralWidget(mainSplitter);

        // Настройка размеров splitter
        mainSplitter->setHandleWidth(15);
        rightSplitter->setHandleWidth(10);
        mainSplitter->setStretchFactor(2, 3);  // Левый виджет занимает 2/3 пространства
        rightSplitter->setStretchFactor(1, 1); // Правый splitter занимает 1/3 пространства

        connect(openRequest, &QAction::triggered, this, &MainWindow::initRequestFromMenu);
        connect(openResult, &QAction::triggered, this, &MainWindow::initReportFromMenu);

        // SubWidget
        /* 
        connect(subWidget, &SubmarineWidget::sendReset, visWidget, &VisWidget::targetClear);
        connect(subWidget, &SubmarineWidget::sendSpeed, visWidget, &VisWidget::setTargetSpeed);
        */

        // connect(visWidget, &VisWidget::sendIntersectionResult, this, &MainWindow::setIntersectionInfo);*/

        // DataWidget
        //connect(dataWidget, &DataWidget::sendPath, this, &MainWindow::initJson);
        //connect(initializer, &Initializer::sendRequestJson, dataWidget, &DataWidget::initializeRequest);
        connect(dataWidget, &DataWidget::sendPath, this, &MainWindow::loadJson); // Получаем путь из списка

        //
        initializer = new Initializer();

        // Initializer <-> Main
        connect(initializer, &Initializer::sendMessage, this, &MainWindow::getInitializerMessage);
        connect(initializer, &Initializer::sendRequest, this, &MainWindow::receiveRequest);
        connect(initializer, &Initializer::sendReport, this, &MainWindow::receiveReport);

        // Scene
        connect(initializer, &Initializer::sendLimits, scene, &SceneWidget::setLimits);

        // Main <-> Scene
        connect(this, &MainWindow::sendValidateRequest, scene, &SceneWidget::reloadRequest);
        connect(this, &MainWindow::sendValidateReport, scene, &SceneWidget::reloadReport);
        connect(scene, &SceneWidget::sendDuringTime, this, &MainWindow::setCurrentTime);

        // Scene <-> Speed
        connect(this, &MainWindow::speedUp, speedWidget, &SpeedWidget::speedUp);
        connect(this, &MainWindow::speedDown, speedWidget, &SpeedWidget::speedDown);
        connect(speedWidget, &SpeedWidget::multiplierChanged, scene, &SceneWidget::setMultiplier);

        // Submarine <-> Scene
        connect(scene, &SceneWidget::sendDrawing, subWidget, &SubmarineWidget::changeButtonColor);
        connect(scene, &SceneWidget::sendTargetPosition, subWidget, &SubmarineWidget::setCurrentPosition);
        connect(scene, &SceneWidget::sendTargetSpeed, subWidget, &SubmarineWidget::setCurrentSpeed);
        connect(scene, &SceneWidget::sendTargetSpeed, subWidget, &SubmarineWidget::setSpeedInput);

        connect(subWidget, &SubmarineWidget::checkBottomChanged, scene, &SceneWidget::changeDrawing);
        connect(subWidget, &SubmarineWidget::sendReset, scene, &SceneWidget::resetTarget);
        //connect(subWidget, &SubmarineWidget::sendSpeed, scene, &SceneWidget::setTargetSpeed);
    }

signals:
    // Отсылаем значения для инициализации в определенной последовательности
    void sendValidateRequest(Request);
    void sendValidateReport(Report);

public slots:
    void setCurrentTime(const double& time)
    {
        timeWidget->updateTime(time);
    }

private slots:
    void getInitializerMessage(const QString& message, const MessageType type) const
    {
        infoWidget->addMessage(message, type);

        qDebug() << message;
    }

    // Отправка данных request в Scene
    void receiveRequest(const Request& request)
    {
        if (request.isLoaded())
        {
            requestLoaded = true;
            emit sendValidateRequest(request);

            infoWidget->addMessage("Request был загружен полностью.", MessageType::Success);
        }
        else
        {
            infoWidget->addMessage("Request был загружен не полностью.", MessageType::Warning);
        }
    }

    // Отправка данных report в Scene
    void receiveReport(const Report& report)
    {
        if (!requestLoaded)
        {
            infoWidget->addMessage("Загрузите Request. Нельзя построить маршруты.", MessageType::Error);
            return;
        }

        if (report.isLoaded())
        {
            reportLoaded = true;
            emit sendValidateReport(report);

            infoWidget->addMessage("Report был загружен полностью.", MessageType::Success);
        }
        else
        {
            infoWidget->addMessage("Report был загружен не полностью.", MessageType::Warning);
        }
    }

public:
    // Получаем строку к файлу json из меню
    QString getPathFromMenu(const QString& title)
    {
        auto fileName = QFileDialog::getOpenFileName(this, title, "", "JSON Files (*.json)");

        if (fileName.isEmpty())
        {
            throw std::runtime_error("Не удалось открыть файл. Операция отменена пользователем.");
        }

        return fileName;
    }
    // TODO:: соединить  DatabaseInitializer reloadRequest(request) и virtual bool reloadRequest(const Request& , const bool&)

private slots:
    // Слот для загрузки из пути в базу данных
    void loadJson(const QString& path)
    {
        // Получить по пути Json
        QJsonObject obj = Operations::jsonFromFile(path);

        // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
        initializer->loadFromJson(obj);

        //// Инициализация дерева request.json
        //dataWidget->clear();

        //dataWidget->initialize(obj);
    }

    //void setIntersectionInfo(const QString& str)
    //{
    //    infoWidget->addMessage(str, InformationWidget::MessageType::Success);
    //}

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        switch (event->key())
        {
        case Qt::Key_Q:
            scene->change();
            break;
        case Qt::Key_Equal:
            this->upSpeed(1);
            break;
        case Qt::Key_Plus:
            this->speedReset();
            break;
        case Qt::Key_Minus:
            this->downSpeed(2);
            break;
        case Qt::Key_S:
            this->start();
            break;
        case Qt::Key_L:
            scene->lines();
            break;
        case Qt::Key_B:
            scene->full();
            break;
        case Qt::Key_P:
            this->pause();
            break;
        case Qt::Key_F: // TODO: Для тестов
            this->setup();
            break;
        case Qt::Key_X:
            this->drawing();
            break;
        // Очистка путей и точки цели
        case Qt::Key_C:
            //scene->targetClear(); // Tagret clear()
            break;
        }
    }

private slots:
    void initReportFromMenu()
    {
        QString path;

        try
        {
            path = getPathFromMenu("Открыть report.json файл(Входные данные).");
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), MessageType::Error);

            return;
        }

        this->loadJson(path);
    }

    void initRequestFromMenu() // TODO:
    {
        QString path;

        try
        {
            path = getPathFromMenu("Открыть request.json файл(Входные данные).");
        }
        catch (std::runtime_error& ex)
        {
            infoWidget->addMessage(ex.what(), MessageType::Error);

            return;
        }

        this->loadJson(path);
    }

private:
    void setup()
    {
        QJsonObject obj = Operations::jsonFromFile("d:\\test\\request.json");
        // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
        initializer->loadFromJson(obj);

        obj = Operations::jsonFromFile("d:\\test\\result.json");
        // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
        initializer->loadFromJson(obj);
    }

    void drawing()
    {
        scene->setDrawing(true);
    }

    // Команды
    void start()
    {
        if (!requestLoaded || !reportLoaded)
        {
            if (!requestLoaded)
                infoWidget->addMessage("Request не загружен.", MessageType::Error);

            if (!reportLoaded)
            {
                infoWidget->addMessage("Report не загружен.", MessageType::Error);
            }
        }
        else
        {
            scene->start();
        }
    }

    void pause()
    {
        if (!requestLoaded && !reportLoaded)
        {
            infoWidget->addMessage("Не все Данные загружены.", MessageType::Error);
        }
        else
        {
            scene->pause();
        }
    }

    void upSpeed(const double& up)
    {
        emit speedUp(up);
    }

    void downSpeed(const int& down)
    {
        emit speedDown(down);
    }

    void speedReset()
    {
        speedWidget->setSpeed(1);
    }

signals:
    void speedUp(const double&);
    void speedDown(const int&);

private:
    // TODO: Указатель на абстрактную сцену
    SceneWidget* scene;

    TimeWidget* timeWidget;
    SpeedWidget* speedWidget;

    DataWidget* dataWidget;
    InformationWidget* infoWidget;
    SubmarineWidget* subWidget;

    Initializer* initializer;

    bool requestLoaded = false;
    bool reportLoaded = false;
};
