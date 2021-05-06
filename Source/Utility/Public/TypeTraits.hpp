#pragma once

/// @cond
template<typename T>
T __void_if_no_value(T);
[[maybe_unused]] void __void_if_no_value();
/// @endcond

/**
 * @brief Returns the address of a given reference.
 *
 * @return The address of t.
 */
template<typename T>
T* __as_voidptr_if_no_value(T& t) { return &t; };

/**
 * @brief Returns the given address.
 *
 * @return t
 */
template<typename T>
T* __as_voidptr_if_no_value(T* t) { return t; };

/**
 * @brief Returns NULL.
 *
 * @return NULL.
 */
template<typename = void>
[[maybe_unused]] void* __as_voidptr_if_no_value() { return nullptr; }

