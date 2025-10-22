#pragma once

/*! Core */
#include <Task/Exceptions/ValidationException.hpp>

#include <nlohmann/json.hpp>

namespace Operations {
/**
    * @brief Функция проверки наличия поля.
    *
    * @param j Проверяемый объект json;
    * @param key Поле, проверяемого объекта json.
*/
inline void validateContains(const nlohmann::json& j, const std::string& key)
{
    // Проверка: содержит поле.
    if (j.find(key) == j.end())
    {
        // code, type, message
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::MissingField, "Поле " + key + " отсутствует.");
    }
}

template<typename T>
inline void validateDigit(const nlohmann::json& j, const T min, const T max)
{
    if (j.is_null())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::IsNull, " не содержит значение.");
    }

    // Проверка: является числом
    if (!j.is_number())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::NotDigit, " не содержит число.");
    }

    // Проверка: лежит в интервале.
    if (j.get<T>() < min || j.get<T>() > max)
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::InvalidValue, " содержит число вне диапазона от " + std::to_string(min) + " до " + std::to_string(max) + ".");
    }
}

/**
    * @brief Функция проверки числового поля.
    *
    * @tparam T Тип проверяемого значения; TODO: у нас только double
    * @param j Проверяемый объект json;
    * @param key Поле объекта json;
    * @param min Минимальное допустимое значение;
    * @param max Максимальное допустимое значение.
*/
template<typename T>
inline void validateValueField(const nlohmann::json& j, const std::string& key, T min, T max)
{
    validateContains(j, key);

    // Проверка: является числом.
    try
    {
        validateDigit(j.at(key), min, max);
    }
    catch (Exceptions::ValidationFailure& ex)
    {
        throw Exceptions::ValidationFailure(ex.getCode(), "Поле " + key + ex.getMessage());
    }
}

/**
    * @brief Функция проверки массива.
    * Проверяет, что такое поле существует, является массивом и не пустое.
    * Для уточнения параметров массива расширить метод.
    * @param j Проверяемый объект json;
    * @param key Поле объекта json.
    */
inline void validateArrayField(const nlohmann::json& j, const std::string& key)
{
    validateContains(j, key);

    // Проверка: является массивом.
    if (!j.at(key).is_array())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::NotArray, "Поле " + key + " не является массивом.");
    }

    // Проверка: пустое значение.
    if (j.at(key).empty())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::EmptyArray, "Массив " + key + " не должен быть пустым.");
    }
}

/**
    * @brief Функция проверки *не именованной* точки(пары).
    * @param j Проверяемый объект json;
    * @param min Минимальное допустимое значение;
    * @param max Максимальное допустимое значение.
*/
inline void validateDigitPair(const nlohmann::json& j, const double min, const double max)
{
    if (j.is_null())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::IsNull, " не содержит значение.");
    }

    // Проверка: является массивом.
    if (!j.is_array())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::NotArray, " не является массивом.");
    }

    // Проверка: пустое значение.
    if (j.empty())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::EmptyArray, " не должен быть пустой.");
    }

    if (j.size() != 2)
    {
        // Не верный размер массива
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::BadLength, " должна содержать два значения.");
    }

    validateDigit(j[0], min, max);
    validateDigit(j[1], min, max);
}

/**
    * @brief Функция проверки Point2D.
    *  Проверка *именованной* точки
    * @param j Проверяемый объект json;
    * @param key Поле объекта json;
    * @param min Минимальное допустимое значение;
    * @param max Максимальное допустимое значение.
*/
inline void validatePoint2D(const nlohmann::json& j, const std::string& key, const double min, const double max)
{
    // Поле существует
    validateContains(j, key);

    // Является парой
    try
    {
        validateDigitPair(j[key], min, max);
    }
    catch (Exceptions::ValidationFailure& ex)
    {
        throw Exceptions::ValidationFailure(ex.getCode(), "Точка " + key + ex.getMessage());
    }
}

/**
    * @brief Функция проверки кольца неименованных точек.
    * именованное кольцо: есть название поля "key" => {"key": [[,]]} => доступ json[key]
    * не именованное кольцо: нет названия поля "" => [[,]] => доступ json
    * @param j Проверяемый объект json;
    * @param min Минимальное допустимое значение;
    * @param max Максимальное допустимое значение.
*/
inline void validatePointsRing(const nlohmann::json& json, const double min, const double max, const std::string& key = "")
{
    nlohmann::json value = json;

    if (!key.empty())
    {
        validateArrayField(json, key);

        value = json[key];
    }

    if (value.is_null())
    {
        throw Exceptions::ValidationFailure(Exceptions::ValidationEnumFailure::IsNull, "отсутствуют элементы(null).");
    }

    size_t index{0};

    try
    {
        // Каждая точка кольца
        for (const auto& point : value)
        {
            validateDigitPair(point, min, max);
            ++index;
        }
    }
    catch (Exceptions::ValidationFailure& ex)
    {
        throw Exceptions::ValidationFailure(ex.getCode(), (key.empty() ? "" : "В " + key) + "точка с индексом [" + std::to_string(index) + "] " + ex.getMessage());
    }
}

/**
    * @brief Функция проверки поискового региона("search_region").
    * @param j Проверяемый объект json(==request);
    * @param min Минимальное допустимое значение;
    * @param max Максимальное допустимое значение.
*/
inline void validateRegion(const nlohmann::json& json, const double min, const double max)
{
    // Проверка наличия полей
    validateContains(json, "search_region");

    // Проверка, что "borders" является массивом внутри "search_region"
    validateArrayField(json["search_region"], "borders");

    size_t index{0};
    try
    {
        for (const auto& ring : json["search_region"]["borders"])
        {
            validatePointsRing(ring, min, max);
            ++index;
        }
    }
    catch (Exceptions::ValidationFailure& ex)
    {
        throw Exceptions::ValidationFailure(ex.getCode(), "В \"borders\" в кольце с индексом [" + std::to_string(index) + "] " + ex.getMessage());
    }

    // Проверка именованной точки
    validatePoint2D(json["search_region"], "entry_point", min, max);
    validatePoint2D(json["search_region"], "exit_point", min, max);
}
} // namespace Operations
