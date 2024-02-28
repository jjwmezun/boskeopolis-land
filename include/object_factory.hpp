#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <cstdint>
#include "object.hpp"
#include <optional>

namespace BSL::ObjectFactory
{
    std::optional<ObjectTemplate> get( uint_fast16_t n );
    void init();
}

#endif // OBJECT_FACTORY_H