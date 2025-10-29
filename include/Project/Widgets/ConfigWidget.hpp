#pragma once

#include <QLabel>
#include <QLineEdit>

#include <QHBoxLayout>
#include <QWidget>

#include "../Models/Parameters.hpp"

namespace Widgets {
class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        int length = 5;

        QHBoxLayout* layout1 = new QHBoxLayout(this);

        QLabel* p0 = new QLabel("P0:");
        b0 = new QLineEdit("0");
        connect(b0, &QLineEdit::textChanged, [this]() {
            if (b0->text() != "")
                emit chengedEditLineP0(b0->text().toDouble());
        });
        p0->setToolTip("Минимальное траверсное расстояние");
        layout1->addWidget(p0);

        b0->setMaxLength(length);
        layout1->addWidget(b0);

        QLabel* p1 = new QLabel("P1:");
        b1 = new QLineEdit("0");
        connect(b1, &QLineEdit::textChanged, [this]() {
            if (b1->text() != "")
                emit chengedEditLineP1(b1->text().toDouble());
        });
        p1->setToolTip("Минимальная длина галса");
        layout1->addWidget(p1);
        b1->setMaxLength(length);
        layout1->addWidget(b1);

        QLabel* a2 = new QLabel("A0:");
        b2 = new QLineEdit("0");
        connect(b2, &QLineEdit::textChanged, [this]() {
            if (b2->text() != "")
                emit chengedEditLineA0(b2->text().toDouble());
        });
        a2->setToolTip("Левая граница длины галса");
        layout1->addWidget(a2);
        b2->setMaxLength(length);
        layout1->addWidget(b2);

        QLabel* a3 = new QLabel("A1:");
        b3 = new QLineEdit("0");
        connect(b3, &QLineEdit::textChanged, [this]() {
            if (b3->text() != "")
                emit chengedEditLineA1(b3->text().toDouble());
        });
        a3->setToolTip("Минимальный угол поворота");
        layout1->addWidget(a3);
        b3->setMaxLength(length);
        layout1->addWidget(b3);

        QLabel* a4 = new QLabel("A2:");
        b4 = new QLineEdit("0");
        connect(b4, &QLineEdit::textChanged, [this]() {
            if (b4->text() != "")
                emit chengedEditLineA2(b4->text().toDouble());
        });
        a4->setToolTip("Максимальный угол поворота");
        layout1->addWidget(a4);
        b4->setMaxLength(length);
        layout1->addWidget(b4);

        QLabel* a5 = new QLabel("A3:");
        b5 = new QLineEdit("0");
        connect(b5, &QLineEdit::textChanged, [this]() {
            if (b5->text() != "")
                emit chengedEditLineA3(b5->text().toDouble());
        });
        a5->setToolTip("Параметр наклона линейной функции распределения длины галса");
        layout1->addWidget(a5);
        b5->setMaxLength(length);
        layout1->addWidget(b5);
    }

    void initialize(const Models::Parameters& parameters)
    {
        b0->setText(QString("%1").arg(parameters.traversaMin));
        b1->setText(QString("%1").arg(parameters.tackDistMin));
        b2->setText(QString("%1").arg(parameters.tackDistLeft));
        b3->setText(QString("%1").arg(parameters.turnAngleDegMin));
        b4->setText(QString("%1").arg(parameters.turnAngleDegMax));
        b5->setText(QString("%1").arg(parameters.distributionCoefficient));

        update();
    }

signals:
    void chengedEditLineP0(double);
    void chengedEditLineP1(double);
    void chengedEditLineA0(double);
    void chengedEditLineA1(double);
    void chengedEditLineA2(double);
    void chengedEditLineA3(double);

private:
    QLineEdit* b0;
    QLineEdit* b1;
    QLineEdit* b2;
    QLineEdit* b3;
    QLineEdit* b4;
    QLineEdit* b5;
};
} // namespace Widgets
