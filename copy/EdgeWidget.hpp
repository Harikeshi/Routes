#pragma once

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

namespace Widgets {
// Пример распределения виджета вверху и внизу
class EdgeWidgets : public QWidget
{
public:
    EdgeWidgets(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        // Главный вертикальный layout
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0); // Убираем отступы по краям
        mainLayout->setSpacing(0);                  // Убираем промежутки

        // 1. Верхняя часть (растягивается)
        QWidget* topWidget = new QWidget(this);
        QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
        topLayout->setContentsMargins(0, 10, 10, 0); // Отступы: слева, сверху, справа, снизу

        // Добавляем растягивающееся пространство слева
        topLayout->addStretch();

        // Виджет в правом верхнем углу
        QPushButton* topRightBtn = new QPushButton("Верх", topWidget);
        topRightBtn->setFixedSize(80, 30);
        topLayout->addWidget(topRightBtn);

        mainLayout->addWidget(topWidget);

        // 2. Центральная часть (растягивается)
        QWidget* centerWidget = new QWidget(this);
        centerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        mainLayout->addWidget(centerWidget, 1);

        // 3. Нижняя часть (растягивается)
        QWidget* bottomWidget = new QWidget(this);
        QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
        bottomLayout->setContentsMargins(0, 0, 10, 10);

        // Добавляем растягивающееся пространство слева
        bottomLayout->addStretch();

        // Виджет в правом нижнем углу
        QPushButton* bottomRightBtn = new QPushButton("Низ", bottomWidget);
        bottomRightBtn->setFixedSize(80, 30);
        bottomLayout->addWidget(bottomRightBtn);

        mainLayout->addWidget(bottomWidget);

        setWindowTitle("Прикрепленные виджеты");
        resize(400, 300);
    }
};
} // namespace Widgets