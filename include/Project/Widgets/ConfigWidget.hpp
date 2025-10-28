#pragma once

#include <QLabel>
#include <QLineEdit>

#include <QWidget>
#include <QHBoxLayout>

class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        QLabel* l0 = new QLabel("l0:");
        QLineEdit* b0 = new QLineEdit("b0");
        connect(b0, &QLineEdit::textChanged, [this, b0]() {
            if (b0->text() != "")
                emit chengeEditLine0(b0->text().toDouble());
        });
        layout->addWidget(l0);
        b0->setMaxLength(10);
        layout->addWidget(b0);

        QLabel* l1 = new QLabel("l1:");
        QLineEdit* b1 = new QLineEdit("b1");
        connect(b1, &QLineEdit::textChanged, [this, b1]() {
            if (b1->text() != "")
                emit chengeEditLine1(b1->text().toDouble());
        });
        layout->addWidget(l1);
        b1->setMaxLength(10);
        layout->addWidget(b1);

        QLabel* l2 = new QLabel("l2:");
        QLineEdit* b2 = new QLineEdit("b2");
        connect(b2, &QLineEdit::textChanged, [this, b2]() {
            if (b2->text() != "")
                emit chengeEditLine2(b2->text().toDouble());
        });
        layout->addWidget(l2);
        b2->setMaxLength(10);
        layout->addWidget(b2);

        QLabel* l3 = new QLabel("l3:");
        QLineEdit* b3 = new QLineEdit("b3");
        connect(b3, &QLineEdit::textChanged, [this, b3]() {
            if (b3->text() != "")
                emit chengeEditLine3(b3->text().toDouble());
        });
        layout->addWidget(l3);
        b3->setMaxLength(10);
        layout->addWidget(b3);
    }

signals:
    void chengeEditLine0(double);
    void chengeEditLine1(double);
    void chengeEditLine2(double);
    void chengeEditLine3(double);
};
