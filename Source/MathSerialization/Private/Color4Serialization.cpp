#include "Color4Serialization.hpp"

typeRT toTypeRT(const color4& p_color4)
{
    typeRT col_4("color4");
    col_4.setTypeName("color4");
    col_4.insertMember(typeRT("r", p_color4.getX()));
    col_4.insertMember(typeRT("g", p_color4.getY()));
    col_4.insertMember(typeRT("b", p_color4.getZ()));
    col_4.insertMember(typeRT("a", p_color4.getW()));
    return col_4;
}

color4 toColor4(typeRT& p_type)
{
    color4 l_result;
    l_result.setX(p_type.member("r").getFloat());
    l_result.setY(p_type.member("g").getFloat());
    l_result.setZ(p_type.member("b").getFloat());
    l_result.setW(p_type.member("a").getFloat());
    return l_result;
}

