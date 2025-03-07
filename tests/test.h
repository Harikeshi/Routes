#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

// Структура для хранения координат точки
struct Point
{
    double x, y;

    Point(double x = 0, double y = 0)
        : x(x), y(y)
    {
    }
};

// Класс для представления объекта
class Object
{
public:
    int id;
    Point currentPosition;
    Point targetPosition;
    double speed;

    Object(int id, const Point& current, const Point& target, double speed)
        : id(id), currentPosition(current), targetPosition(target), speed(speed)
    {
    }

    // Метод для расчета новой позиции
    Point calculateNewPosition() const
    {
        double distance = std::sqrt(std::pow(targetPosition.x - currentPosition.x, 2) +
                                    std::pow(targetPosition.y - currentPosition.y, 2));
        double ratio = speed / distance;
        return {
            currentPosition.x + (targetPosition.x - currentPosition.x) * ratio,
            currentPosition.y + (targetPosition.y - currentPosition.y) * ratio};
    }
};

// Класс для управления перестроением объектов
class Formation
{
private:
    std::vector<Object> objects;

public:
    // Добавление объекта в перестроение
    void addObject(const Object& obj)
    {
        objects.push_back(obj);
    }

    // Расчет участков движения для всех объектов
    std::vector<std::pair<Object, Point>> calculateMovementSegments() const
    {
        std::vector<std::pair<Object, Point>> segments;
        for (const auto& obj : objects)
        {
            Point newPosition = obj.calculateNewPosition();
            segments.emplace_back(obj, newPosition);
        }
        return segments;
    }

    // Вывод информации о движении объектов
    void printMovementSegments() const
    {
        auto segments = calculateMovementSegments();
        for (const auto& [obj, newPosition] : segments)
        {
            std::cout << "Object " << obj.id << " moves from ("
                      << obj.currentPosition.x << ", " << obj.currentPosition.y
                      << ") to (" << newPosition.x << ", " << newPosition.y
                      << ") with speed " << obj.speed << std::endl;
        }
    }
};

int main()
{
    std::srand(std::time(0));

    // Создаем объекты со случайными начальными позициями и целевыми позициями
    Formation formation;

    formation.addObject(Object(1, {(double)(std::rand() % 100), (double)(std::rand() % 100)}, {10, 10}, 5 + std::rand() % 6));
    formation.addObject(Object(2, {(double)(std::rand() % 100), (double)(std::rand() % 100)}, {20, 10}, 5 + std::rand() % 6));
    formation.addObject(Object(3, {(double)(std::rand() % 100), (double)(std::rand() % 100)}, {30, 10}, 5 + std::rand() % 6));
    formation.addObject(Object(4, {(double)(std::rand() % 100), (double)(std::rand() % 100)}, {40, 10}, 5 + std::rand() % 6));
    formation.addObject(Object(5, {(double)(std::rand() % 100), (double)(std::rand() % 100)}, {50, 10}, 5 + std::rand() % 6));

    // Выводим информацию о движении объектов
    formation.printMovementSegments();

    return 0;
}
