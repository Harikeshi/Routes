#pragma once

#include <QJsonObject>
#include <QJsonArray>

namespace Data {
inline QJsonObject setupReport()
{
    QJsonObject rootObject;

    // Создаем массив messages
    QJsonArray messagesArray;

    // Создаем объект сообщения
    QJsonObject messageObject;
    messageObject["code"] = 0;
    messageObject["text"] = "Success!";
    messageObject["type"] = "Info";

    // Добавляем сообщение в массив
    messagesArray.append(messageObject);

    // Добавляем массив messages в корневой объект
    rootObject["messages"] = messagesArray;

    // Создаем массив routes
    QJsonArray routesArray;

    // Создаем объект route
    QJsonObject routeObject;

    // Создаем массив points
    QJsonArray pointsArray;

    // Создаем первую точку (массив координат)
    QJsonArray point1;
    point1.append(27.61964416703702);
    point1.append(12.69099186621467);

    // Создаем вторую точку (массив координат)
    QJsonArray point2;
    point2.append(25.0);
    point2.append(6.0);

    // Добавляем точки в массив points
    pointsArray.append(point1);
    pointsArray.append(point2);

    // Добавляем массив points в объект route
    routeObject["points"] = pointsArray;

    // Создаем массив velocities
    QJsonArray velocitiesArray;
    velocitiesArray.append(5);

    // Добавляем массив velocities в объект route
    routeObject["velocities"] = velocitiesArray;

    // Добавляем объект route в массив routes
    routesArray.append(routeObject);

    // Добавляем массив routes в корневой объект
    rootObject["routes"] = routesArray;

    return rootObject;
}

inline QJsonObject setupRequest()
{
    QJsonObject json;
    json["time"] = 50;

    QJsonObject formTarget;
    QJsonArray detectionPoint = {0, 0};
    formTarget["detection_point"] = detectionPoint;
    formTarget["root-mean-square_error"] = 99999;
    QJsonArray courses = {360, 360};
    formTarget["courses"] = courses;

    QJsonObject velocities;
    velocities["current"] = 99.99;
    velocities["max"] = 99.99;
    velocities["min_noise-reduced"] = 99.99;
    velocities["max_noise-reduced"] = 99.99;
    formTarget["velocities"] = velocities;

    formTarget["obsolescence_time"] = 999999;
    formTarget["avoidance_distance"] = 999999;
    json["form_target"] = formTarget;

    QJsonObject searchRegion;
    QJsonArray borders;
    QJsonArray border1 = {
        QJsonArray{0, 0}, QJsonArray{0, 20}, QJsonArray{20, 20}, QJsonArray{20, 4}, QJsonArray{25, 20}, QJsonArray{25, 0}};
    QJsonArray border2 = {
        QJsonArray{7, 8}, QJsonArray{7, 12}, QJsonArray{13, 12}, QJsonArray{13, 8}};
    borders.append(border1);
    borders.append(border2);
    searchRegion["borders"] = borders;

    QJsonArray entryPoint = {0, 6};
    searchRegion["entry_point"] = entryPoint;
    QJsonArray exitPoint = {25, 6};
    searchRegion["exit_point"] = exitPoint;
    json["search_region"] = searchRegion;

    QJsonArray borderLine = {
        QJsonArray{0, 0}, QJsonArray{1, 1}, QJsonArray{2, 2}};
    json["border_line"] = borderLine;

    QJsonObject shipsParams;
    //shipsParams["count"] = 99;
    shipsParams["detection_range"] = 3;
    shipsParams["max_velocity"] = 18;
    shipsParams["search_velocity"] = 12;
    shipsParams["turning_radius"] = 3;
    shipsParams["min_lenght_section"] = 2;
    json["ships_parameters"] = shipsParams;

    return json;
}
} // namespace Data