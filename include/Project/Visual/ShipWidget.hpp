// #pragma once

// #include <QPainter>
// #include <QPointF>
// // TODO: Виджет корабля включает в себя радиус ГАС и так далее
// // TODO: Добавть к маршруту или сделать отдельным?? Все упирается в радиус ГАС
// // TODO: Радиус ГАС устанавливаем на корабль по время создания
// // Создание во время инициализации Периметра или Выходных данных??

// namespace Visual {
// class ShipWidget
// {
// public:
//     void draw(QPainter& painter, const QPointF& position)
//     {
//         // TODO: Это будет отдельный виджет
//         const QPointF screenPos = position; // Центр

//         color.setAlpha(80); // Прозрачность 0-255
//         painter.setBrush(color);

//         painter.drawEllipse(screenPos, radiusHAS, radiusHAS);

//         // Внутренний радиус

//         color.setAlpha(255); // Прозрачность 0-255
//         painter.setBrush(color);
//         painter.drawEllipse(screenPos, 0.1 * radiusHAS, 0.1 * radiusHAS);
//     }

//     double getRadius() const
//     {
//         return radiusHAS;
//     }

//     void setColor(const QColor& col)
//     {
//         color = col;
//     }

// private:
//     double radiusHAS;
//     QColor color;
// };
// } // namespace Visual
