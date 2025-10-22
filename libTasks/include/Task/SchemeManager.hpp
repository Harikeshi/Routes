#pragma once

#include <Task/Schemes/Restore/PetljaK.hpp>
#include <Task/Schemes/Restore/ProryvK.hpp>
#include <Task/Schemes/Search/AtBorder/CrossPatrolling.hpp>
#include <Task/Schemes/Search/AtBorder/LinearPatrolling.hpp>
#include <Task/Schemes/Search/AtBorder/OnStop.hpp>
#include <Task/Schemes/Search/ByCall/SectorSpiral.hpp>
#include <Task/Schemes/Search/ByCall/StraightTack.hpp>
#include <Task/Schemes/Search/InRegion/DeterminedShift.hpp>
#include <Task/Schemes/Search/InRegion/Shift.hpp>
#include <Task/Schemes/Search/InRegion/StraightTack.hpp>
#include <Task/Schemes/Search/InRegion/Zigzag.hpp>
//#include <Task/Schemes/Search/InRegion/_Zigzag.hpp>

#include <Task/SearchScheme.hpp>

/*! std */
#include <functional>
#include <map>
#include <memory>

/**
 * @brief Класс для создания объектов Input и Scheme и управления Типами. 
 * 
 */
class SchemeManager
{
    std::map<SearchScheme, std::function<std::unique_ptr<Abstractions::Input>()>> inputFactory;

    std::map<SearchScheme, std::function<std::unique_ptr<Abstractions::Scheme>(const Abstractions::Input&)>> schemeFactory;

    std::map<SearchType, std::vector<SearchScheme>> types;

public:
    /**
 * @brief Конструктор для регистрации фабрик.
 * 
 */
    SchemeManager();

    /**
     * @brief Метод создания Input
     * 
     * @param scheme Конкретная Scheme для которой создается Input
     * @return std::unique_ptr<Abstractions::Input> указатель на Input 
     */
    std::unique_ptr<Abstractions::Input> createInput(SearchScheme scheme);

    /**
     * @brief Метод создания Scheme
     * 
     * @param scheme Конкретная Scheme, которую надо создать
     * @param input Input соответствующей Scheme
     * @return std::unique_ptr<Abstractions::Scheme> указатель на Scheme 
     */
    std::unique_ptr<Abstractions::Scheme> createScheme(SearchScheme scheme, const Abstractions::Input& input);

    /**
 * @brief Метод создания списка схем
 * 
 * @param type Конкретный тип
 * @return const std::vector<SearchScheme>& список схем 
 */
    const std::vector<SearchScheme>& getSchemes(SearchType type) const;

    ~SchemeManager() = default;

private:
    /**
 * @brief Метод регистрации Input и Scheme
 * 
 * @tparam SchemeClass имя типа регистрируемой схемы
 * @tparam InputClass имя типа Input соответствующей схемы
 * @param scheme Конкретная схема
 */
    template<typename SchemeClass, typename InputClass>
    void registerSchemeAndInput(SearchScheme scheme);
};
