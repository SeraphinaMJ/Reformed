
#include "QuaternionSerialization.hpp"

typeRT toTypeRT(const quaternion& p_quaternion)
{
    typeRT l_quaternion("quaternion");
    l_quaternion.setTypeName("quaternion");
    l_quaternion.insertMember(typeRT("x", p_quaternion.getX()));
    l_quaternion.insertMember(typeRT("y", p_quaternion.getY()));
    l_quaternion.insertMember(typeRT("z", p_quaternion.getZ()));
    l_quaternion.insertMember(typeRT("w", p_quaternion.getW()));
    return l_quaternion;
}

quaternion toQuaternion(typeRT& p_type)
{
    quaternion l_result;
    l_result.setX(p_type.member("x").getFloat());
    l_result.setY(p_type.member("y").getFloat());
    l_result.setZ(p_type.member("z").getFloat());
    l_result.setW(p_type.member("w").getFloat());
    return l_result;
}
