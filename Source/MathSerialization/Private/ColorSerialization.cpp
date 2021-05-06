#include "ColorSerialization.hpp"

#include "Vector4Serialization.hpp"
#include <Vector4.hpp>

typeRT toTypeRT(const color& p_color)
{
    return toTypeRT(static_cast<vector4>(p_color));
}

color toColor(typeRT& p_typeRT)
{
    vector4 l_vec4 = toVector4(p_typeRT);
    color l_result(l_vec4.getX(), l_vec4.getY(), l_vec4.getZ(), l_vec4.getW());
    return l_result;
}
