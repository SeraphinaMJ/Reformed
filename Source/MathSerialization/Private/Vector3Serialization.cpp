#include "Vector3Serialization.hpp"

typeRT toTypeRT(const vector3& p_vector3)
{
    typeRT vec_3("vector3");
    vec_3.setTypeName("vector3");
    vec_3.insertMember(typeRT("x", p_vector3.getX()));
    vec_3.insertMember(typeRT("y", p_vector3.getY()));
    vec_3.insertMember(typeRT("z", p_vector3.getZ()));
    return vec_3;
}

vector3 toVector3(typeRT& p_type)
{
    vector3 l_result;
    l_result.setX(p_type.member("x").getFloat());
    l_result.setY(p_type.member("y").getFloat());
    l_result.setZ(p_type.member("z").getFloat());
    return l_result;
}
