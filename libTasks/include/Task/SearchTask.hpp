#pragma once

// TODO: geos/vend/json.hpp Используется nlohmann v.3.9.1 из Core -> geos
#include <Task/SchemeManager.hpp>

#include <nlohmann/json.hpp>

/**
 * @brief Класс работы с поисковыми задачами (Search API)
 *  
 */
class SearchTask
{
    nlohmann::json task;                          //!< Входные данные. = nlohmann::json::object();
    std::shared_ptr<SchemeManager> schemeManager; //!< Объект manager.

public:
    /**
     * @brief Конструктор по-умолчанию.
    */
    explicit SearchTask();

    /**
     * @brief Конструктор.
     *
     * @param json Объект json, входные данные.
    */
    explicit SearchTask(const nlohmann::json& json);

public:
    /**
 * @brief Метод устанавливает новое значение task.
 * 
 * @param json Новые входные данные.
 */
    void setTask(const nlohmann::json& json);

    /**
 * @brief Метод расчета пути конкретной схемы.
 * 
 * @param scheme Определяет конкретную схему.
 * @return nlohmann::json Объект json.
 */
    nlohmann::json computeRoute(const SearchScheme scheme);

    /**
 * @brief Метод выбора схемы.
 * 
 * @param type Указывает конкретный тип;
 * @return nlohmann::json Объект json, содержит пути и сообщения о возможных ошибках.
 */
    nlohmann::json chooseScheme(const SearchType type);

    ~SearchTask() = default;
};
