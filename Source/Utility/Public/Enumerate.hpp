#pragma once
#include "Enumerate.inl" // implementation for the enumerate iterator thing

/*!
\brief
	based on Python's enumerate iterator
	works with any type with a forward iterator
\param container
	the container to enumerate
\return
	range, compatible with STL algorithms and range-based forloops
	has an i that increments with the container's iterator
*/
template<class T>
[[nodiscard]]
constexpr extended::enumerated<T> enumerate(T& container) noexcept
{
	return extended::enumerated<T>{ container };
}

/*!
\brief
	const container overload of "enumerate"
	works with any type with an input iterator
\param container
	the container to enumerate
\return
	range, compatible with STL algorithms and range-based forloops
	has an i that increments with the container's iterator
*/
template<class T>
[[nodiscard]]
constexpr extended::const_enumerated<T> enumerate(T const& container) noexcept
{
	return extended::const_enumerated<T>{ container };
}
