#pragma once

#include <QObject>

#include "Project/Models/Request.hpp"

namespace Scene {
class Actor : public QObject
{
    using Request = Models::Request;

public:
    ~Actor() = default;
    Actor(QObject* parent = nullptr)
        : QObject(parent)
    {
    }
    /*!
    * Отрисовка Объектов.
    * @param painter
    */
    virtual void draw(QPainter& painter) = 0;

    /*!
     * Инициализация после загрузки request.
     * @param request
     */
    virtual void reload(const Request& request) = 0;

    /*!
     * Получить пределы важные для соответствующего Actor.
     * @return
     */
    virtual Entities::Limits getLimits() const = 0;

    /*!
     * Смена координат X<->Y.
     */
    virtual void swapCoordinates() = 0;

    /*!
     * Сброс Actor к нулевым значениям.
     */
    virtual void reset() = 0;
};
} // namespace Scene
