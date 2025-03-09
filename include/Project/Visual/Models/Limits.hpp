#pragma once

namespace Visual {
namespace Models {
struct Limits
{
    double minX;
    double minY;
    double maxX;
    double maxY;

    double getMaxBorderLength() const
    {
        return std::max(std::fabs(maxX - minX), std::fabs(maxY - minY));
    }
};
} // namespace Models
} // namespace Visual
