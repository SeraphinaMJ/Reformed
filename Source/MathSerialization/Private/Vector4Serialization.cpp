#include "Vector4Serialization.hpp"

typeRT toTypeRT(const vector4& p_vector4)
{
    typeRT vec_4("vector4");
    vec_4.setTypeName("vector4");
    vec_4.insertMember(typeRT("x", p_vector4.getX()));
    vec_4.insertMember(typeRT("y", p_vector4.getY()));
    vec_4.insertMember(typeRT("z", p_vector4.getZ()));
    vec_4.insertMember(typeRT("w", p_vector4.getW()));
    return vec_4;
}

vector4 toVector4(typeRT& p_type)
{
    vector4 l_result;
    l_result.setX(p_type.member("x").getFloat());
    l_result.setY(p_type.member("y").getFloat());
    l_result.setZ(p_type.member("z").getFloat());
    l_result.setW(p_type.member("w").getFloat());
    return l_result;
}
