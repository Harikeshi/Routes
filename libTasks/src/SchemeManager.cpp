#include <Task/SchemeManager.hpp>

SchemeManager::SchemeManager()
{
    /**
         * @brief Регистрация типов схем
         *
         */
    types[SearchType::InRegion] = {SearchScheme::Zigzag, SearchScheme::Shift, SearchScheme::DeterminedShift, SearchScheme::StraightTackRegion};
    types[SearchType::ByCall] = {SearchScheme::SectorSpiral, SearchScheme::StraightTackCall};
    types[SearchType::AtBorder] = {SearchScheme::LinearPatrolling, SearchScheme::OnStop};

    types[SearchType::Restoring] = {SearchScheme::PetljaK, SearchScheme::ProryvK};

    /**
         * @brief Регистрация в фабриках inputFactory и schemeFactory по принципу: registerSchemeAndInput<Scheme, SchemeInput>(SearchScheme)
         *
         */
    registerSchemeAndInput<Schemes::Search::InRegion::Zigzag, Schemes::Search::InRegion::Input>(SearchScheme::Zigzag);
    registerSchemeAndInput<Schemes::Search::InRegion::Shift, Schemes::Search::InRegion::Input>(SearchScheme::Shift);
    registerSchemeAndInput<Schemes::Search::InRegion::DeterminedShift, Schemes::Search::InRegion::Input>(SearchScheme::DeterminedShift);
    registerSchemeAndInput<Schemes::Search::InRegion::StraightTack, Schemes::Search::InRegion::Input>(SearchScheme::StraightTackRegion);

    //!
    //    registerSchemeAndInput<Schemes::Search::InRegion::_Zigzag, Schemes::Search::InRegion::Input>(SearchScheme::_Zigzag);

    registerSchemeAndInput<Schemes::Search::ByCall::SectorSpiral, Schemes::Search::ByCall::Input>(SearchScheme::SectorSpiral);
    registerSchemeAndInput<Schemes::Search::ByCall::StraightTack, Schemes::Search::ByCall::Input>(SearchScheme::StraightTackCall);

    registerSchemeAndInput<Schemes::Search::AtBorder::LinearPatrolling, Schemes::Search::AtBorder::Input>(SearchScheme::LinearPatrolling);
    registerSchemeAndInput<Schemes::Search::AtBorder::CrossPatrolling, Schemes::Search::AtBorder::Input>(SearchScheme::CrossPatrolling);
    registerSchemeAndInput<Schemes::Search::AtBorder::OnStop, Schemes::Search::AtBorder::Input>(SearchScheme::OnStop);

    registerSchemeAndInput<Schemes::Restore::PetljaK, Schemes::Restore::Input>(SearchScheme::PetljaK);
    registerSchemeAndInput<Schemes::Restore::ProryvK, Schemes::Restore::Input>(SearchScheme::ProryvK);
}

std::unique_ptr<Abstractions::Input> SchemeManager::createInput(const SearchScheme scheme)
{
    auto it = inputFactory.find(scheme);
    if (it != inputFactory.end())
    {
        return it->second();
    }

    // TODO: throw
    return nullptr;
}

std::unique_ptr<Abstractions::Scheme> SchemeManager::createScheme(const SearchScheme scheme, const Abstractions::Input& input)
{
    auto it = schemeFactory.find(scheme);

    if (it != schemeFactory.end())
    {
        return it->second(input);
    }

    // TODO: throw
    return nullptr;
}

const std::vector<SearchScheme>& SchemeManager::getSchemes(const SearchType type) const
{
    return types.at(type);
}

template<typename SchemeClass, typename InputClass>
void SchemeManager::registerSchemeAndInput(const SearchScheme scheme)
{
    inputFactory[scheme] = []() {
        return std::make_unique<InputClass>();
    };

    schemeFactory[scheme] = [](const Abstractions::Input& input) {
        return std::make_unique<SchemeClass>(dynamic_cast<const InputClass&>(input));
    };
}
