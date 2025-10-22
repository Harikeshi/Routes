#include <Task/Outputs/SchemeEfficiency.hpp>

namespace Outputs {
SchemeEfficiency::SchemeEfficiency(size_t name, double efficiency, double course)
    : scheme{name}, efficiency{efficiency}, approachCourse{course}
{
}

nlohmann::json SchemeEfficiency::toJson() const
{
    nlohmann::json result;

    result["scheme"] = scheme;
    result["efficiency"] = efficiency;
    result["approach_course"] = approachCourse.get();

    return result;
}
} // namespace Outputs
