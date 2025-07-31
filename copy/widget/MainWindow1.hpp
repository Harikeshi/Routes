#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QComboBox>
#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "Scene1.hpp"
#include "Scene2.hpp"

class MainWindow1 : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow1(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        setupUI();
        setupScenes();
    }

private:
    QStackedWidget* stackedWidget;
    QComboBox* comboBox;

    void setupUI()
    {
        comboBox = new QComboBox(this);
        comboBox->addItem("Scheme 1");
        comboBox->addItem("Scheme 2");
        comboBox->setFixedWidth(200);

        stackedWidget = new QStackedWidget(this);

        connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), stackedWidget, &QStackedWidget::setCurrentIndex);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(comboBox, 0, Qt::AlignHCenter);
        layout->addWidget(stackedWidget);

        QWidget* centralWidget = new QWidget(this);
        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
        resize(400, 300);
    }

    void setupScenes()
    {
        stackedWidget->addWidget(new Scene1(this));
        stackedWidget->addWidget(new Scene2(this));
    }
};

#endif // MAINWINDOW_H
