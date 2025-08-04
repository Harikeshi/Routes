#pragma once

#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QStackedLayout>
#include <QWidget>

#include "./Scene/Actors/InRegion.hpp"
#include "./Widgets/SceneWidget.hpp"

#include "MessageType.hpp"

#include "./Widgets/CustomTable.hpp"
#include "./Widgets/DataWidget.hpp"
#include "./Widgets/InformationWidget.hpp"
#include "./Widgets/ManageWidget.hpp"
#include "./Widgets/SubWidget.hpp"
#include "Project/Widgets/UpdateProgress.hpp"

#include "Initializer.hpp"

class MainWindow : public QMainWindow
{
    using Route = Models::Route;
    using Perimeter = Models::Perimeter;
    using CoordinateSystem = Scene::Entities::CoordinateSystem;

    using SceneWidget = Widgets::SceneWidget;

    using DataWidget = Widgets::DataWidget;
    using CustomTable = Widgets::CustomTable;
    using ManageWidget = Widgets::ManageWidget;
    using SubWidget = Widgets::SubWidget;
    using InformationWidget = Widgets::InformationWidget;
    using UpdateProgressBar = Widgets::UpdateProgressBar;

    using Request = Models::Request;
    using Report = Models::Report;

    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        // #--------------------------------------------------------------------------------------
        // Меню
        // #--------------------------------------------------------------------------------------
        //        QMenu* fileMenu = menuBar()->addMenu("Файл");`
        //        QAction* openRequest = new QAction("Загрузить Request.json", this);
        //        QAction* openResult = new QAction("Загрузить Result.json", this);
        //        fileMenu->addAction(openRequest);
        //        fileMenu->addAction(openResult);
        // #--------------------------------------------------------------------------------------
        // MainLayout - Основная компоновка
        // #--------------------------------------------------------------------------------------
        QWidget* centralWidget = new QWidget(this);

        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        // Главный горизонтальный splitter (разделитель между Scene и правой панелью)
        QSplitter* mainHorizontalSplitter = new QSplitter(Qt::Horizontal, centralWidget);

        // SceneWidget
        scene = new SceneWidget(this);
        scene->setFocusPolicy(Qt::StrongFocus);
        scene->resize(800, 800);
        scene->setMouseTracking(false); // Отключить трекинг мыши
        mainHorizontalSplitter->addWidget(scene);

        // Вертикальная компоновка справа
        QSplitter* rightVerticalSplitter = new QSplitter(Qt::Vertical);

        // Виджеты для правой вертикальной панели
        dataWidget = new DataWidget(this); // TODO: this
        table = new CustomTable(this);
        rightVerticalSplitter->addWidget(dataWidget);
        rightVerticalSplitter->addWidget(table);

        // Горизонтальный splitter нижний
        QSplitter* innerHorizontalSplitter = new QSplitter(Qt::Horizontal);
        manage = new ManageWidget(this);

        sub = new SubWidget(this);

        innerHorizontalSplitter->addWidget(manage);
        innerHorizontalSplitter->addWidget(sub);
        rightVerticalSplitter->addWidget(innerHorizontalSplitter);

        infoWidget = new InformationWidget(this);
        rightVerticalSplitter->addWidget(infoWidget);

        // Стиль для разделителей
        QString splitterStyle =
            "QSplitter::handle {"
            "   background: #555555;"
            "   width: 2px;"
            "   height: 2px;"
            ""
            "}";
        mainHorizontalSplitter->setStyleSheet(splitterStyle);
        rightVerticalSplitter->setStyleSheet(splitterStyle);
        innerHorizontalSplitter->setStyleSheet(splitterStyle);

        // Главный горизонтальный splitter
        mainHorizontalSplitter->addWidget(rightVerticalSplitter);
        //        mainHorizontalSplitter->setStretchFactor(0, 2);
        //        mainHorizontalSplitter->setStretchFactor(1, 1);

        // Добавляем главный splitter в layout
        mainLayout->addWidget(mainHorizontalSplitter, 1);
        progress = new UpdateProgressBar(this);
        //        progress->setTextVisible(true);
        //        progressBar->setFixedHeight(24);
        //        progressBar->setValue(50);

        //        QWidget* progressContainer = new QWidget();
        //        QVBoxLayout* progressLayout = new QVBoxLayout(progressContainer);
        //        progressLayout->setContentsMargins(0, 0, 0, 2);
        //        progressLayout->addWidget(progress);
        //        mainLayout->addWidget(progressContainer);
        //
        mainLayout->addWidget(progress);
        this->setCentralWidget(centralWidget);
        // #--------------------------------------------------------------------------------------
        // Logics
        // #--------------------------------------------------------------------------------------
        //        connect(openRequest, &QAction::triggered, this, &MainWindow::initRequestFromMenu);
        //        connect(openResult, &QAction::triggered, this, &MainWindow::initReportFromMenu);

        // connect(visWidget, &VisWidget::sendIntersectionResult, this, &MainWindow::setIntersectionInfo);

        // DataWidget
        connect(dataWidget, &DataWidget::sendPath, this, &MainWindow::loadJson); // Получаем путь из списка

        initializer = new Initializer();

        // Initializer <-> Main
        connect(initializer, &Initializer::sendRequestJson, dataWidget, &DataWidget::initializeRequest);
        connect(initializer, &Initializer::sendMessage, this, &MainWindow::getInitializerMessage);
        //        connect(initializer, &Initializer::sendRequest, this, &MainWindow::receiveRequest);
        //        connect(initializer, &Initializer::sendReport, this, &MainWindow::receiveReport);

        // Scene
        //        connect(initializer, &Initializer::sendLimits, scene, &SceneWidget::setLimits);
        //        connect(initializer, &Initializer::sendRequest, this, &MainWindow::setSpeedLabel);

        connect(initializer, &Initializer::requestLoaded, this, &MainWindow::receiveRequest);
        connect(initializer, &Initializer::reportLoaded, this, &MainWindow::receiveReport);

        // ProgressBar <-> Scene
        connect(scene, &SceneWidget::sendFullTime, progress, &UpdateProgressBar::setTotalTime);
        connect(scene, &SceneWidget::sendCurrentTime, progress, &UpdateProgressBar::setCurrentTime);
        connect(progress, &UpdateProgressBar::timeChanged, scene, &SceneWidget::moveFromProgress);

        // SubWidget
        // Submarine <-> Scene
        connect(sub, &SubWidget::resetButtomPushed, scene, &SceneWidget ::resetTarget);
        connect(sub, &SubWidget::sendSpeedChanged, scene, &SceneWidget::setTargetSpeed);

        // [ Set ]
        connect(scene, &SceneWidget::sendDrawing, sub, &SubWidget::changeButtonColor);
        connect(sub, &SubWidget::checkBottomChanged, scene, &SceneWidget::changeDrawing);

        // [ Reset ]
        connect(sub, &SubWidget::sendReset, scene, &SceneWidget::resetTarget);
        connect(sub, &SubWidget::sendSpeedChanged, this, &MainWindow::addInformation);

        // Manage
        connect(manage, &ManageWidget::sendPlayButtonClicked, this, &MainWindow::start);
        connect(manage, &ManageWidget::sendPauseButtonClicked, this, &MainWindow::pause);
        connect(manage, &ManageWidget::sendMinusButtonClicked, this, &MainWindow::downSpeed);
        connect(manage, &ManageWidget::sendPlusButtonClicked, this, &MainWindow::upSpeed);
        connect(manage, &ManageWidget::clickVisionButton, scene, &SceneWidget::changeShowRoutesPoints);

        connect(manage, &ManageWidget::pushReset, scene, &SceneWidget::reset);

        connect(scene, &SceneWidget::sceneReseted, this, &MainWindow::sceneReset);

        // Table
        //connect(scene, &SceneWidget::sendTargetCurrentPositionSpeed, table, &CustomTable::updateSubmarine);
        connect(scene, &SceneWidget::sendIndexCurrentPositionSpeed, table, &CustomTable::updateOrAddRow);
        this->setEnabled(false);

        this->setWindowTitle("Visualization");
    }

signals:

    // Отсылаем значения для инициализации в определенной последовательности
    //    void sendValidateRequest(Request);

    //    void sendValidateReport(Report);
    //
    //    void speedUp(const double&);
    //
    //    void speedDown(const int&);

private slots:
    void setSpeedLabel(const Request& request)
    {
        sub->setSpeedInput(request.target.currentVelocity);
    }

    void getInitializerMessage(const QString& message, const MessageType type) const
    {
        infoWidget->addMessage(message, type);
        // qDebug() << message;
    }

    // Отправка данных request в Scene
    //    void receiveRequest(const Request& request)
    void receiveRequest()
    {
        if (initializer->getRequest().isLoaded())
        {
            requestLoaded = true;

            checkLoad();

            scene->reloadRequest(initializer->getRequest());
            //            emit sendValidateRequest(request);
            //// Инициализация дерева request.json
            dataWidget->clear();

            dataWidget->initializeRequest(initializer->getRequestJson());

            infoWidget->addMessage("Request был загружен полностью.", MessageType::Success);
        }
        else
        {
            infoWidget->addMessage("Request был загружен не полностью.", MessageType::Warning);
        }
    }

    void sceneReset()
    {
        // scene->reset();

        // initializer->reset();

        reportLoaded = false;
        requestLoaded = false;

        setEnabled(false);

        progress->reset();
        table->reset();

        infoWidget->addMessage("Был Произведен сброс.", MessageType::Warning);
        infoWidget->addMessage("Требуется загрузка  данных(json).", MessageType::Info);
    }

    void addInformation(int speed)
    {
        infoWidget->addMessage(QString::number(speed) + " м/c новая скорость ПЛ.", MessageType::Info);
    }

    // Отправка данных report в Scene
    void receiveReport()
    {
        if (!requestLoaded)
        {
            infoWidget->addMessage("Загрузите Request. Нельзя построить маршруты.", MessageType::Error);
            return;
        }

        if (initializer->getRequest().isLoaded())
        {
            reportLoaded = true;

            checkLoad();

            scene->reloadReport(initializer->getReport());

            // Вывод сообщений
            for (const auto& message : initializer->getMessages())
            {
                infoWidget->addMessage(QString("%1").arg(message.code) + ":" + message.type + ": " + message.text);
            }

            infoWidget->addMessage("Report был загружен полностью.", MessageType::Success);
        }
        else
        {
            infoWidget->addMessage("Report был загружен не полностью.", MessageType::Warning);
        }
    }

public:
    void setEnabled(bool value)
    {
        manage->setEnabled(value);

        sub->setEnabled(value);

        table->setEnabled(value);

        // scene->setEnabled(value);

        progress->setEnabled(value);
    }

    // Получаем строку к файлу json из меню
    //    QString getPathFromMenu(const QString& title)
    //    {
    //        auto fileName = QFileDialog::getOpenFileName(this, title, "", "JSON Files (*.json)");
    //
    //        if (fileName.isEmpty())
    //        {
    //            throw std::runtime_error("Не удалось открыть файл. Операция отменена пользователем.");
    //        }
    //
    //        return fileName;
    //    }

    // TODO:: соединить  DatabaseInitializer reloadRequest(request) и virtual bool reloadRequest(const Request& , const bool&)

private slots:

    // Слот для загрузки из пути в базу данных
    void loadJson(const QString& path)
    {
        // Получить по пути Json
        QJsonObject obj = Operations::jsonFromFile(path);

        // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
        initializer->loadFromJson(obj);
    }

    //void setIntersectionInfo(const QString& str)
    //{
    //    infoWidget->addMessage(str, InformationWidget::MessageType::Success);
    //}
public:
    void calculate()
    {
        // Нажатие Calc:
        // вызов ActorType getSchemeType() из scene
        // SearchTask(request);
        // Вызов SearchTask.calculate(ActorType);
        // Обработать полученный json.
        // Выгрузить в scene.
        // Получить из сцены название
    }

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        switch (event->key())
        {
        case Qt::Key_Q:
            scene->change();
            break;
        case Qt::Key_Equal:
            this->speedReset();
            break;
        case Qt::Key_Plus:
            this->upSpeed();
            break;
        case Qt::Key_Minus:
            this->downSpeed();
            break;
        case Qt::Key_S:
            this->start(true);
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

    //    void initReportFromMenu()
    //    {
    //        QString path;
    //
    //        try
    //        {
    //            path = getPathFromMenu("Открыть report.json файл(Входные данные).");
    //        }
    //        catch (std::runtime_error& ex)
    //        {
    //            infoWidget->addMessage(ex.what(), MessageType::Error);
    //
    //            return;
    //        }
    //
    //        this->loadJson(path);
    //    }
    //
    //    void initRequestFromMenu() // TODO:
    //    {
    //        QString path;
    //
    //        try
    //        {
    //            path = getPathFromMenu("Открыть request.json файл(Входные данные).");
    //        }
    //        catch (std::runtime_error& ex)
    //        {
    //            infoWidget->addMessage(ex.what(), MessageType::Error);
    //
    //            return;
    //        }
    //
    //        this->loadJson(path);
    //    }

private:
    void setup()
    {
        try
        {
            //        QJsonObject obj = Operations::jsonFromFile("d:\\test\\request.json");
            //            QJsonObject obj = Operations::jsonFromFile("/home/harikeshi/ajson/request.json");
            //            QJsonObject obj = Operations::jsonFromFile("e:\\visualization\\jsons\\request.json");
            QJsonObject obj = Operations::jsonFromFile("d:\\dev\\visualization\\jsons\\request.json");
            // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
            initializer->loadFromJson(obj);

            requestLoaded = true;

            //        obj = Operations::jsonFromFile("d:\\test\\result.json");
            //             obj = Operations::jsonFromFile("/home/harikeshi/ajson/result.json");
            //            obj = Operations::jsonFromFile("e:\\visualization\\jsons\\result.json");
            obj = Operations::jsonFromFile("d:\\dev\\visualization\\jsons\\result.json");
            // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
            initializer->loadFromJson(obj);

            reportLoaded = true;
        }
        catch (...)
        {
        }

        checkLoad();
    }

    void checkLoad()
    {
        if (reportLoaded & reportLoaded)
            setEnabled(true);
    }

    void drawing()
    {
        scene->setDrawing(true);
    }

    // Команды
    void start(bool checked)
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
            if (checked)
                scene->start();
            else
            {
                scene->stop();
                progress->clear();
            }
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
            // manage->setPauseButtomImage(scene->pause());
        }
    }

    void upSpeed()
    {
        // Вызываем на сцене
        scene->upSpeed(1);
    }

    void downSpeed()
    {
        scene->downSpeed(2);
    }

    void speedReset()
    {
        scene->resetSpeed();
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
    }

private:
    // TODO: Указатель на абстрактную сцену
    SceneWidget* scene;

    // Правая панель
    UpdateProgressBar* progress;
    DataWidget* dataWidget;
    CustomTable* table;
    ManageWidget* manage;
    SubWidget* sub;

    InformationWidget* infoWidget;

    // Работа с данными
    Initializer* initializer;

    bool requestLoaded = false;
    bool reportLoaded = false;
};
