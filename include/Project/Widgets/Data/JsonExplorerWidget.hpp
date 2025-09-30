#pragma once

#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>

namespace Widgets {
/*!
 * Класс выбора Json файла.
 */
class JsonFileExplorer final : public QWidget
{
    Q_OBJECT
public:
    JsonFileExplorer(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // Основные элементы интерфейса
        createWidgets();
        setupLayout();
        setupConnections();

        // Начальный каталог
        navigateTo(QDir::homePath());
    }
signals:
    void sendPath(QString);

private:
    void createWidgets()
    {
        // Адресная строка
        pathEdit = new QLineEdit(this);
        upButton = new QPushButton("↑", this);
        upButton->setFixedWidth(30);

        // Модель и список файлов
        model = new QFileSystemModel(this);
        model->setNameFilters({"*.json"});
        model->setNameFilterDisables(false);
        model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

        listView = new QListView(this);
        listView->setModel(model);
    }

    void setupLayout()
    {
        QHBoxLayout* pathLayout = new QHBoxLayout();
        pathLayout->addWidget(upButton);
        pathLayout->addWidget(pathEdit);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(pathLayout);
        mainLayout->addWidget(listView);
        setLayout(mainLayout);
    }

    void setupConnections()
    {
        connect(upButton, &QPushButton::clicked, this, &JsonFileExplorer::navigateUp);
        connect(pathEdit, &QLineEdit::returnPressed, this, &JsonFileExplorer::onPathEntered);
        connect(listView, &QListView::doubleClicked, this, &JsonFileExplorer::onItemDoubleClicked);
    }

private slots:
    void navigateUp()
    {
        QDir dir(currentPath);
        if (dir.cdUp())
        {
            navigateTo(dir.path());
        }
    }

    void onPathEntered()
    {
        QString newPath = pathEdit->text();
        if (QFileInfo::exists(newPath))
        {
            navigateTo(newPath);
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Указанный путь не существует");
            pathEdit->setText(currentPath);
        }
    }

    void onItemDoubleClicked(const QModelIndex& index)
    {
        QString path = model->filePath(index);

        if (model->isDir(index))
        {
            try
            {
                navigateTo(path);
            }
            catch (...)
            {
                qDebug() << "Exception from json_path_explorer!";
            }
        }
        else
        {
            emit sendPath(path);
        }
    }

private:
    void navigateTo(const QString& path)
    {
        currentPath = QDir::cleanPath(path);
        model->setRootPath("");
        listView->setRootIndex(model->index(currentPath));
        pathEdit->setText(currentPath);

        //
        setWindowTitle("JSON Explorer - " + currentPath);
    }

    // Элементы интерфейса
    QLineEdit* pathEdit;
    QPushButton* upButton;
    QFileSystemModel* model;
    QListView* listView;

    QString currentPath;
};
} // namespace Widgets