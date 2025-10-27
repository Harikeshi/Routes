#pragma once

#ifdef _WIN32
#define MYLIB_API __declspec(dllimport)
#endif

#include <QAction>
#include <QMainWindow>
#include <QMenuBar>
#include <QSplitter>
#include <QStackedLayout>
#include <QWidget>
#include <QtWidgets>

#include "./Database/DatabaseManager.hpp"
#include "./Widgets/CustomTable.hpp"
#include "./Widgets/DataWidget.hpp"
#include "./Widgets/InformationWidget.hpp"
#include "./Widgets/ManageWidget.hpp"
#include "./Widgets/MatrixViewWidget.hpp"
#include "./Widgets/SceneWidget.hpp"
#include "./Widgets/SubWidget.hpp"
#include "./Widgets/UpdateProgress.hpp"

#include "./Initializer.hpp"
#include "./MessageType.hpp"

#include "Task/Exceptions/AlgorithmException.hpp"
#include "Task/SearchTask.hpp"

class MainWindow : public QMainWindow
{
    using SceneWidget = Widgets::SceneWidget;
    using DataWidget = Widgets::DataWidget;
    using CustomTable = Widgets::CustomTable;
    using MatrixViewWidget = Widgets::MatrixViewWidget;
    using ManageWidget = Widgets::ManageWidget;
    using SubWidget = Widgets::SubWidget;
    using InformationWidget = Widgets::InformationWidget;
    using UpdateProgressBar = Widgets::UpdateProgressBar;

    using Request = Models::Request;
    using Report = Models::Report;

    Q_OBJECT

    SearchTask task;

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        //! MainLayout - Основная компоновка
        auto* centralWidget = new QWidget(this);

        auto* mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        //! Главный горизонтальный splitter (разделитель между Scene и правой панелью)
        auto* mainHorizontalSplitter = new QSplitter(Qt::Horizontal, centralWidget);

        //! SceneWidget
        scene = new SceneWidget(this);
        scene->setFocusPolicy(Qt::StrongFocus);
        scene->resize(800, 800);
        scene->setMouseTracking(false); // Отключить трекинг мыши
        mainHorizontalSplitter->addWidget(scene);

        //! Вертикальная компоновка справа
        auto* rightVerticalSplitter = new QSplitter(Qt::Vertical, centralWidget);

        //! Виджеты для правой вертикальной панели
        dataWidget = new DataWidget(this); // TODO: this

        //! Вкладки под первым справа
        QTabWidget* tabs = new QTabWidget(this);

        // Первый виджет
        table = new CustomTable(this);

        // Второй виджет
        matrix = new Widgets::MatrixViewWidget(this);

        // Добавляем вкладки
        tabs->addTab(table, "Таблица БЭНК");
        tabs->addTab(matrix, "Гистограмма");

        rightVerticalSplitter->addWidget(dataWidget);
        rightVerticalSplitter->addWidget(tabs);

        //! Горизонтальный splitter нижний
        auto* innerHorizontalSplitter = new QSplitter(Qt::Horizontal, centralWidget);
        manage = new ManageWidget(this);

        sub = new SubWidget(this);

        innerHorizontalSplitter->addWidget(manage);
        innerHorizontalSplitter->addWidget(sub);
        rightVerticalSplitter->addWidget(innerHorizontalSplitter);

        infoWidget = new InformationWidget(this);
        rightVerticalSplitter->addWidget(infoWidget);

        //! Стиль для разделителей
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

        //! Главный горизонтальный splitter
        mainHorizontalSplitter->addWidget(rightVerticalSplitter);

        //! Добавляем главный splitter в layout
        mainLayout->addWidget(mainHorizontalSplitter, 1);
        progress = new UpdateProgressBar(this);

        mainLayout->addWidget(progress);
        this->setCentralWidget(centralWidget);

        //! Signals-Slots
        this->initConnections();

        //! Установка дефолтный схемы схема(0)
        this->datamanager->setScheme(Scene::ActorTypeName.find(static_cast<Scene::ActorType>(0))->second);

        //! Инициализация ReportsView
        this->initReportsListView();

        //! Отключить виджеты
        this->setEnabled(false);

        this->setWindowTitle("Visualization");
    }

    ~MainWindow() = default;

    void initConnections()
    {
        //! Logics

        //! DataWidget
        connect(dataWidget, &DataWidget::sendPath, this, &MainWindow::loadJson); // Получаем путь из списка
        connect(dataWidget, &DataWidget::sendReportRequestIds, this, &MainWindow::setRequestReportFromIds);

        connect(datamanager, &Database::DatabaseManager::sendReportsModel, dataWidget, &DataWidget::updateReports);
        connect(datamanager, &Database::DatabaseManager::sendReportRow, dataWidget, &DataWidget::updateReports_);

        connect(&Initializer::instance(), &Initializer::sendMessage, this, &MainWindow::processMessage);
        connect(&Initializer::instance(), &Initializer::sendError, this, &MainWindow::processError);

        connect(&Initializer::instance(), &Initializer::changedRequest, this, &MainWindow::receiveRequest);
        connect(&Initializer::instance(), &Initializer::changedReport, this, &MainWindow::receiveReport);

        // connect(&Initializer::instance(), &Initializer::changedRequest, this, &MainWindow::requestChange);
        // connect(&Initializer::instance(), &Initializer::changedReport, this, &MainWindow::reportChange);

        //connect(&Initializer::instance(), &Initializer::changedRequest, datamanager, &Database::DatabaseManager::saveRequest);
        //connect(&Initializer::instance(), &Initializer::changedReport, datamanager, &Database::DatabaseManager::saveReport);

        //! ProgressBar <-> Scene
        connect(scene, &SceneWidget::sendFullTime, progress, &UpdateProgressBar::setTotalTime);
        connect(scene, &SceneWidget::sendCurrentTime, progress, &UpdateProgressBar::setCurrentTime);
        connect(progress, &UpdateProgressBar::timeChanged, scene, &SceneWidget::moveFromProgress);

        //! SubWidget
        //! Submarine <-> Scene
        connect(sub, &SubWidget::resetButtomPushed, scene, &SceneWidget::resetTarget);
        connect(sub, &SubWidget::sendSpeedChanged, scene, &SceneWidget::setTargetSpeed);

        connect(scene, &SceneWidget::sendTargetSpeed, sub, &SubWidget::setSpeedInput);

        //! [ Set ]
        connect(scene, &SceneWidget::sendDrawing, sub, &SubWidget::changeButtonColor);
        connect(sub, &SubWidget::checkBottomChanged, scene, &SceneWidget::changeDrawing);

        //! [ Reset ]
        connect(sub, &SubWidget::sendReset, scene, &SceneWidget::resetTarget);

        //! Manage
        connect(manage, &ManageWidget::sendPlayButtonClicked, this, &MainWindow::start);
        connect(manage, &ManageWidget::sendPauseButtonClicked, this, &MainWindow::pause);
        connect(manage, &ManageWidget::sendMinusButtonClicked, this, &MainWindow::downSpeed);
        connect(manage, &ManageWidget::sendPlusButtonClicked, this, &MainWindow::upSpeed);
        connect(manage, &ManageWidget::clickVisionButton, scene, &SceneWidget::changeShowWidthPath);
        connect(manage, &ManageWidget::clickedCalculate, this, &MainWindow::clickedCalc);
        connect(manage, &ManageWidget::pushReset, scene, &SceneWidget::reset);

        connect(scene, &SceneWidget::sceneReseted, this, &MainWindow::sceneReset);

        //! Table
        connect(scene, &SceneWidget::sendIndexCurrentPositionSpeed, table, &CustomTable::updateOrAddRow);
        connect(scene, &SceneWidget::schemeChanged, datamanager, &Database::DatabaseManager::setScheme);

        //! DataWidget
        connect(dataWidget, &DataWidget::sendRequestFromWidget, this, &MainWindow::setRequestFromDataWidget);
        connect(dataWidget, &DataWidget::sendDeleteReportRequestIds, this, &MainWindow::deleteRequestReportFromTable); // delete
        connect(dataWidget, &DataWidget::sendSaveReportRequestIds, this, &MainWindow::saveRequestReportToDisk);        // save
        connect(dataWidget, &DataWidget::sendAddReportToDbSignal, this, &MainWindow::addRequestReportToDb);            // add

        //! MatrixViewer
        connect(matrix, &MatrixViewWidget::sendMessage, this, &MainWindow::getMessage);
    }

private slots:

    /*!
     * Инициализация Reports при загрузке MainWindow.
     */
    void initReportsListView()
    {
        try
        {
            dataWidget->updateReports(datamanager->allReportRowsModel());
        }
        catch (std::exception& ex)
        {
            qDebug() << ex.what();
        }
    }

    /*!
     * Обработка сообщения.
     * @param message
     */
    void processMessage(const QString& message)
    {
        infoWidget->addMessage(message, MessageType::Info);
    }

    /*!
     * Обработка Ошибки.
     * @param error
     */
    void processError(const QString& error)
    {
        infoWidget->addMessage(error, MessageType::Error);
    }

    /*!
    * Действия при сбросе сцены.
    */
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

        infoWidget->addMessage("Требуется загрузка данных(json).", MessageType::Info);
    }

    void clickedCalc()
    {
        auto type = scene->getActorName();

        datamanager->setScheme(type);

        scene->setActor(type, Initializer::instance().getRequest());

        if (requestLoaded)
        {
            reportLoaded = false;
            try
            {
                // TODO: Можно сделать чтобы был перевод в nlohmann toNJson()
                task.setTask(Initializer::instance().getRequest().toNJson());

                auto report = task.computeRoute(static_cast<SearchScheme>(scene->getActorType()));

                Initializer::instance().loadFromJson(Operations::convertToQJsonObject(report));

                //TODO: message Произведен расчет
                infoWidget->addMessage(QString("Расчет %1 произведен успешно!").arg(scene->getActorName()),
                                       MessageType::Success);
            }
            catch (const std::exception& ex)
            {
                infoWidget->addMessage(ex.what(), MessageType::Error);
            }
            catch (...)
            {
                infoWidget->addMessage("Неизвестная ошибка.", MessageType::Error);
            }
        }
        else
        {
            infoWidget->addMessage("Загрузите Request. Нельзя построить маршруты.", MessageType::Error);
        }
    }

    /*!
     * Действия после инициализации request.
     * Действия при изменении request
     */
    void receiveRequest(const Request& request)
    {
        requestLoaded = true;
        reportLoaded = false;
        checkLoad(); // TODO: В новых реалиях(сброс при загрузке request) под вопросом

        //! Загрузка request, сброс загрузки report.
        scene->reloadRequest(request);

        //! Инициализация дерева request.json
        // TODO: Переделать дерево.
        dataWidget->clear();
        dataWidget->setRequest(request);

        infoWidget->addMessage("Request был загружен полностью.", MessageType::Success);

        matrix->loadRequest(request);

        Operations::jsonToFile(request.toJson(), "d:\\_request.json");
    }

    /*!
     * Методы работы с базой данных
     * Для загрузки из строки из базы Данных.
     * @param request_id
     * @param report_id
     */
    void setRequestReportFromIds(size_t report_id, size_t request_id)
    {
        auto request = datamanager->getRequest(request_id);
        auto report = datamanager->getReport(report_id);

        if (!report.scheme.isEmpty())
            scene->setActor(report.scheme, request);

        try
        {
            scene->reset();

            Initializer::instance().setRequest(request);

            receiveRequest(request);
        }
        catch (std::exception& ex)
        {
            infoWidget->addMessage(ex.what(), MessageType::Error);
        }

        if (report._routes.isEmpty())
        {
            infoWidget->addMessage("Report не содержит пути.", MessageType::Error);
            return;
        }

        try
        {
            Initializer::instance().setReport(report);

            receiveReport(report);
        }
        catch (std::exception& ex)
        {
            infoWidget->addMessage(ex.what(), MessageType::Error);
        }
    }

    void deleteRequestReportFromTable(size_t report_id, size_t request_id)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            tr("Подтверждение"),
            tr("Точно хотите удалить запись?"),
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            datamanager->deleteReport(report_id);
            dataWidget->deleteReport(report_id);
            // обновить таблицу
        }
        else
        {
            // отмена
        }
    }

    void saveRequestReportToDisk(size_t report_id, size_t request_id)
    {
        const QString dirPath = QFileDialog::getSaveFileName(
            this, tr("Выберите папку для сохранения"), QDir::homePath());

        if (dirPath.isEmpty())
        {
            return;
        }

        auto json_report = datamanager->getReport(report_id).toNJson().dump(4);
        saveFile(dirPath + "_report.json", json_report);

        auto json_request = datamanager->getRequest(request_id).toNJson().dump(4);
        saveFile(dirPath + "_request.json", json_request);
    }

    void addRequestReportToDb()
    {
        // TODO: Проверить id в базе, Чтобы не сохранять одинаковые или оставить на усмотрение пользователя;
        if (reportLoaded == true)
        {
            auto request_id = datamanager->saveRequest(Initializer::instance().getRequest());

            auto report_id = datamanager->saveReport(Initializer::instance().getReport());

            infoWidget->addMessage(QString("Report #%1 сохранен в базу данных.").arg(report_id), MessageType::Success);
        }
        else
        {
            infoWidget->addMessage("Report не загружен.", MessageType::Warning);
        }

        table->update();
    }

private:
    void saveFile(const QString& filePath, const std::string& txt)
    {
        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось открыть файл для записи: \n%1").arg(file.errorString()));
            return;
        }
        QTextStream out_report(&file);
        out_report.setCodec("UTF-8");
        out_report << QString::fromStdString(txt);
        out_report.flush();

        if (file.error() != QFile::NoError)
        {
            QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка записи в файл: \n%1").arg(file.errorString()));
            return;
        }

        QMessageBox::information(this, tr("Готово"), tr("Файл успешно сохранен: \n%1").arg(filePath));

        file.close();
    }

public:
    void setRequestFromDataWidget(const Models::Request& request)
    {
        requestLoaded = true;

        QString message;
        MessageType type = MessageType::Success;

        scene->reloadRequest(request);

        Initializer::instance().saveRequest(request);

        infoWidget->addMessage(message, type);
    }

    // Отправка сообщения в InformationWidget
    void getMessage(const QString& message, size_t type)
    {
        infoWidget->addMessage(message, static_cast<MessageType>(type));
    }

    /*!
     * Действия при изменении Report.
     */
    void receiveReport(const Report& report)
    {
        if (!requestLoaded)
        {
            infoWidget->addMessage("Загрузите Входные данные. Нельзя построить маршруты.", MessageType::Error);
            return;
        }

        reportLoaded = true;

        checkLoad();

        scene->reloadReport(report);

        // Вывод сообщений
        for (const auto& message : report._messages)
        {
            infoWidget->addMessage(QString("%1").arg(message.code) + ":" + message.type + ": " + message.text);
        }

        infoWidget->addMessage("Report был загружен полностью.", MessageType::Success);

        //! Загрузка в Гистограмму
        matrix->loadReport(report);

        // Сохранить на диск
        Operations::jsonToFile(report.toJson(), "d:\\_report.json");
    }

public:
    /*!
     * Активируем виджеты после загрузки report.
     * @param value
     */
    void setEnabled(bool value)
    {
        // manage->setEnabled(value);

        sub->setEnabled(value);

        table->setEnabled(value);

        // scene->setEnabled(value);

        progress->setEnabled(value);
    }

    // TODO:: соединить  DatabaseInitializer reloadRequest(request) и virtual bool reloadRequest(const Request& , const bool&)

private slots:
    // Слот для загрузки из пути в базу данных
    void loadJson(const QString& path)
    {
        // Получить по пути Json
        QJsonObject obj = Operations::jsonFromFile(path);

        // TODO: может просто из инициализатора приходить request или report и используется там, где подписано?
        Initializer::instance().loadFromJson(obj);
    }

protected:
    void keyPressEvent(QKeyEvent* event) override
    {
        switch (event->key())
        {
        case Qt::Key_U:
            break;
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
            dataWidget->updateReports(datamanager->allReportRowsModel());
            scene->lines();
            break;
        case Qt::Key_B:
            scene->full();
            break;
        case Qt::Key_P:
            this->pause();
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

private:
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
            {
                infoWidget->addMessage("Request не загружен.", MessageType::Error);
            }
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

private:
    // TODO: Указатель на абстрактную сцену
    SceneWidget* scene;

    // Правая панель
    UpdateProgressBar* progress;
    DataWidget* dataWidget;
    CustomTable* table;
    MatrixViewWidget* matrix;
    ManageWidget* manage;
    SubWidget* sub;

    InformationWidget* infoWidget;

    //!
    Database::DatabaseManager* datamanager = new Database::DatabaseManager(this);

    bool requestLoaded = false;
    bool reportLoaded = false;

    // bool reportChanged = false;
    // bool requestChanged = false;
};
