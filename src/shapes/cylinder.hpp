#pragma once

#include "shape.hpp"

namespace shapes
{

/**
 * @brief A cylinder of radius 1, height 2, centred at the origin
 * Constructed from two disks and a tube
 */
class cylinder : public shape
{
public:
    virtual ~cylinder() = default;

    std::unique_ptr<shape> clone() const override { return std::make_unique<cylinder>(*this); }

    bool hit(const ray_t& ray) const override;
    bool hit(const ray_t& ray, intersection& intersec) const override;

    boundary get_boundary() const override;
};

}