
#include "Vector2Serialization.hpp"

typeRT toTypeRT(const vector2& p_vector2)
{
    typeRT vec_2("vector2");
    vec_2.setTypeName("vector2");
    vec_2.insertMember(typeRT("x", p_vector2.getX()));
    vec_2.insertMember(typeRT("y", p_vector2.getY()));
    return vec_2;
}

vector2 toVector2(typeRT& p_type)
{
    vector2 l_result;
    l_result.setX(p_type.member("x").getFloat());
    l_result.setY(p_type.member("y").getFloat());
    return l_result;
}
