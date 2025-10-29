#include <Task/SearchTask.hpp>

#include <Task/Exceptions/AlgorithmException.hpp>
#include <Task/Exceptions/ValidationException.hpp>
//#include <Task/Operations/UnitConverter.hpp>
#include <Task/Outputs/Message.hpp>
#include <Task/SchemeManager.hpp>

#ifndef NDEBUG
#include <Task/Operations/JsonOperations.hpp> //! Для сохранения json
#endif

SearchTask::SearchTask()
    : task{}, schemeManager{std::make_shared<SchemeManager>()}
{
}

SearchTask::SearchTask(const nlohmann::json& json)
    : schemeManager{std::make_shared<SchemeManager>()}
{
    // TODO: Валидация ввода в конструкторе? В таком случае можно исключить валидацию внутри Input, но в таком случае требуется проверка необязательных полей.
#ifndef NDEBUG
    Operations::saveJsonToHome(json, "request.json");
#endif

    task = json; // Нельзя инициализировать в конструкторе task{json}, приводится task к массиву.
}

void SearchTask::setTask(const nlohmann::json& json)
{
    task = json;
}

nlohmann::json SearchTask::computeRoute(const SearchScheme scheme)
{
    auto input = schemeManager->createInput(scheme);

    nlohmann::json result;

    result["routes"] = nlohmann::json::array();
    result["messages"] = nlohmann::json::array();

    if (input == nullptr)
    {
        // return
    }

    // Ошибки валидации(Exceptions::ValidationException)
    try
    {
        input->fromJson(task);
    }
    catch (const Exceptions::ValidationFailure& ex)
    {
        result["messages"].push_back(Outputs::Message{ex.getCode(), "Error", ex.what()}.toJson()); // TODO: Ошибка валидации

#ifndef NDEBUG
        Operations::saveJsonToHome(result, "report.json");
#endif
        return result;
    }
    /// @note здесь можно инициализировать параметры и задать для схемы и входных данных

    // 1. Алгоритм возвращает ошибку (Exceptions::AlgorithmException)
    // 2. Алгоритм возвращает результат
    try
    {
        auto concreteScheme = schemeManager->createScheme(scheme, *input);
        Outputs::Route route = concreteScheme->calculate();

        result["routes"].push_back(route.toJson());
        result["messages"].push_back(Outputs::Message{}.toJson());
    }
    catch (Exceptions::AlgorithmFailure& ex)
    {
        result["messages"].push_back(Outputs::Message{ex.getCode(), "Error", ex.what()}.toJson());
    }

#ifndef NDEBUG
    Operations::saveJsonToHome(result, "report.json");
#endif

    return result;
}

nlohmann::json SearchTask::chooseScheme(const SearchType type)
{
    // 1. Формируем весь список, по конкретному типу;
    // 2. Вычисляем максимальную вероятность из них; // TODO: как в этом смысл?
    // 3. Формируем json
    const auto& schemes = schemeManager->getSchemes(type);

    nlohmann::json result;

    result["schemes_efficiencies"] = nlohmann::json::array();
    result["messages"] = nlohmann::json::array();

    // TODO: Результаты должны быть отсортированы
    for (auto scheme : schemes)
    {
        // TODO: возможно нужна проверка, если вдруг схемы зарегистрированы не верно
        auto input = schemeManager->createInput(scheme);

        try
        {
            input->fromJson(task);
        }
        catch (const Exceptions::ValidationFailure& ex)
        {
            result["schemes_efficiencies"].push_back(Outputs::SchemeEfficiency{static_cast<size_t>(scheme)}.toJson());
            result["messages"].push_back(Outputs::Message{ex.getCode(), "Error", ex.what()}.toJson()); // TODO: Ошибка валидации

            continue;
        }

        try
        {
            auto concreteScheme = schemeManager->createScheme(scheme, *input);
            Outputs::SchemeEfficiency output = concreteScheme->probability();

            output.scheme = scheme; // TODO: Можно перевести в Алгоритм, но тогда алгоритм должен знать свой номер,
                                    // по сути алгоритм должен возвращать число и угол

            result["schemes_efficiencies"].push_back(output.toJson());
            result["messages"].push_back(Outputs::Message{}.toJson());
        }
        catch (const std::runtime_error& ex) // TODO: Ошибка из Алгоритма, еще не разработаны
        {
            result["schemes_efficiencies"].push_back(Outputs::SchemeEfficiency{}.toJson());
            result["messages"].push_back(Outputs::Message{}.toJson()); // TODO: переписать ошибку из Алгоритма
        }
    }
    // TODO: Если применять сортировку, то надо соответственно и сортировать Сообщения.
    // std::sort(result["schemes_efficiencies"].begin(), result["schemes_efficiencies"].end(), [](const nlohmann::json& a, const nlohmann::json& b) { return a["efficiency"].get<double>() < b["efficiency"].get<double>(); });

    auto elementWithMostEffectiveScheme = std::max_element(result["schemes_efficiencies"].begin(), result["schemes_efficiencies"].end(), [](const nlohmann::json& a, const nlohmann::json& b) { return a["efficiency"].get<double>() < b["efficiency"].get<double>(); });

    result["mostEffectiveScheme"] = (*elementWithMostEffectiveScheme)["scheme"];

#ifndef NDEBUG
    Operations::saveJsonToHome(result, "scheme_report.json");
#endif

    return result;
}
