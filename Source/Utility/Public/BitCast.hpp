#pragma once
#include <cstring>
#include <type_traits>
// 
template<typename To, typename From>
To bit_cast(From toCast)
{
	static_assert(sizeof(To) == sizeof(From), "can only cast types of the same size");
	To casted;
	std::memcpy(&casted, &toCast, sizeof(To));
	return casted;
}