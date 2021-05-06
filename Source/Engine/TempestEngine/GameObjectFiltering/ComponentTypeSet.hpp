/*!***************************************************************************************
\file       ComponentTypeSet.hpp
\author     Aaron Damyen
\date       5/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the componentTypeSet class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include <ComponentType.hpp>

//========3rd Party Includes============================================================//
#include <memory>
#include <unordered_map>
#include <vector>
#include <array>
#include <numeric>

namespace GameObjectFiltering
{

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////
/*!***************************************************************************************
\par class: componentTypeSet
\brief   This is the componentTypeSet class used to filter game objects by the types of components
         they contain
*****************************************************************************************/
class componentTypeSet
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	componentTypeSet(const componentType & p_type);
	constexpr componentTypeSet() noexcept = default;
	componentTypeSet(const componentTypeSet&) noexcept = default;
	componentTypeSet& operator=(const componentTypeSet&) noexcept = default;
	~componentTypeSet() = default;

	/*!***************************************************************************************
	\brief  Enables the set entry for the specified component type.
	\param p_type - The type of the component to be added to the set.
	*****************************************************************************************/
	void setType(componentType p_type);

	void reset();

	/*!***************************************************************************************
	\brief  Determines the inequality between this set and another.
	\param p_other - The other set to determine equality with.
	\return bool - Returns true if, for both sets, any element in one set is not present in the other
	*****************************************************************************************/
	bool operator!=(const componentTypeSet & p_other) const;

	/*!***************************************************************************************
	\brief  Determines the equality between this set and another.
	\param p_other - The other set to determine equality with.
	\return bool - Returns true if, for both sets, all entries in one are contained in the other.
	*****************************************************************************************/
	bool operator==(const componentTypeSet & p_other) const;
	/*!***************************************************************************************
	\brief  Determines if this set is a proper subset of the other set.
	\param p_other - The other set to compare.
	\return bool - Returns true if all entries in this set are contained in the other set and
	               the two sets are not equal.
	*****************************************************************************************/
	bool operator<(const componentTypeSet & p_other) const;
	/*!***************************************************************************************
	\brief  Determines if this set is a subset of the other set.
	\param p_other - The other set to compare.
	\return bool - Returns true if all entries in this set are contained in the other set.
	*****************************************************************************************/
	bool operator<=(const componentTypeSet & p_other) const;


	/*!***************************************************************************************
	\brief  concatenates two subsets into a superset
	\param p_other - The set to concatenate
	\return componentTypeSet - the new component set
	*****************************************************************************************/
	componentTypeSet operator|(const componentTypeSet & p_other) const;

	/*!***************************************************************************************
	\brief  concatenates two subsets into a superset
	\param p_other - The set to concatenate
	\return componentTypeSet ref - this
	*****************************************************************************************/
	componentTypeSet& operator|=(const componentTypeSet & p_other);

	/*!***************************************************************************************
	\brief  Determines if this set contains no entries.
	\return bool - Returns true if there are no entries in this set, false otherwise.
	*****************************************************************************************/
	bool isEmpty() const;

	/*!***************************************************************************************
	\brief  Calculates the hash value of this set.
	\return std::size_t - Returns the hash value of this set.
	\relates std::hash<GameObjectFiltering::componentTypeSet>
	*****************************************************************************************/
	std::size_t hashValue() const;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves a reference to an empty set.
	\return componentTypeSet - The empty set.
	*****************************************************************************************/
	static componentTypeSet Empty();

	static const componentType typeForIndex(unsigned p_combined_index);

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	static constexpr unsigned int s_blockCount = 2; //!< Number of size_t to use for bitset
    public: using bitSet = std::array<size_t, s_blockCount>;
    private: bitSet bits = { 0 };  //!< The bits for indicating members of a component type

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Finds the index of a component type within the dynamically manages list of types.
	\param p_type - The type of the component to find in the list.
	\return unsigned int - The location (combined index) of the specified type.
	*****************************************************************************************/
	static unsigned int indexOf(componentType p_type);
	static std::vector<unsigned int>&  indicesOf(componentType p_type, std::vector<unsigned int> & p_result);
	static std::vector<unsigned int> indicesOf(componentType p_type);


	/*!***************************************************************************************
	\brief  Calculates the element index and bit index given a combined index
	\param p_combined_index - the combined index to break apart
	\param p_element_index - out - the calculated element index containing the combined index
	\param p_bit_index - out - the calculated bit index containing the combined index
	*****************************************************************************************/
	static void breakdownIndex(unsigned int p_combined_index, unsigned int & p_element_index, unsigned int & p_bit_index);

	/*!***************************************************************************************
	\brief  Calculates the element index and bit index given a combined index
	\param p_combined_index - the combined index to break apart
	\return pair unsigned&unsigned - the element index into the array followed by the bit index into that element
	*****************************************************************************************/
	static constexpr std::pair<unsigned, unsigned> breakdownIndex(unsigned int p_combined_index) noexcept;

	/*!***************************************************************************************
	\brief helper: Creates a componentTypeSet from an existing bitset
	\param p_bitset the bitset to use\
	*****************************************************************************************/
	componentTypeSet(bitSet p_bitset) noexcept;


	/*!***************************************************************************************
	\brief helper: Creates a bitset from a componentType
	\param p_type - the type to hash and store
	\return bitset - the hash of the type
	*****************************************************************************************/
	static bitSet createBitSet(componentType p_type);

	static unsigned int nextCombinedIndex; //!< The next available component type (combined) index
	static std::unordered_map<componentType, unsigned int /*combined index*/> indexMap; //!< map of component types to their assigned index

};

namespace utility
{
	// https://stackoverflow.com/questions/18017543/c11-variable-number-of-arguments-same-specific-type
	template<typename... T>
	struct all_same : std::false_type { };

	template<>
	struct all_same<> : std::true_type { };

	template<typename T>
	struct all_same<T> : std::true_type { };

	template<typename T, typename... Ts>
	struct all_same<T, T, Ts...> : all_same<T, Ts...> { };

	template<typename T, typename... Ts>
	constexpr auto all_same_v = all_same<T, Ts...>::value;

	//! \brief reduce operators for make_componentTypeSet
	struct reduceTypeSets
	{
		using result_value = componentTypeSet;
		using input_value = componentType;

		result_value operator()(result_value const& typeSet, result_value const& typeSet2) const
		{
			return typeSet |typeSet2;
		}

		result_value operator()(result_value const& typeSet, input_value const& type) const
		{
			return typeSet | result_value{ type };
		}

		result_value operator()(input_value const& type, result_value const& typeSet) const
		{
			return result_value{ type } | typeSet;
		}

		result_value operator()(input_value const& type, input_value const& type2) const
		{
			return result_value{ type } | result_value{ type2 };
		}
	};
} // namespace utility


/*!***************************************************************************************
\brief concatenates multiple componentTypes into a single constructed componentTypeSet
\param p_type - the first parater. All inputs must be of its type
\param p_types - a variadic number of additional componentType s.
\return componentTypeSet - a componentTypeSet of the union of all componentTypes
*****************************************************************************************/
template<typename... componentTypes>
componentTypeSet make_componentTypeSet(componentType p_type, componentTypes... p_types)
{
	static_assert(utility::all_same_v<componentType, componentTypes...>, "All inputs must be componentTypes");

    return componentTypeSet{p_type} | (componentTypeSet{p_types} | ...);

#if 0
	return std::reduce( l_types.cbegin(), l_types.cend(),
		componentTypeSet{ p_type },
		utility::reduceTypeSets{});
#endif
}

/*!***************************************************************************************
\brief constructs a componentTypeSet from a single componentType
\param p_type - the type of which the componentTypeSet should check against
\return componentTypeSet - a set which can be used to detect objects of p_type
*****************************************************************************************/
componentTypeSet make_componentTypeSet(componentType p_type);


/*!***************************************************************************************
\brief constructs an empty component typeset
\return componentTypeSet - default constructed componenttypeset that doesn't do much on its own
*****************************************************************************************/
componentTypeSet make_componentTypeSet();


} // namespace GameObjectFiltering


namespace std
{
////////==========================================================================////////
////////  Structure                                                               ////////
////////==========================================================================////////
/*!*************************************************************************************
\par struct: hash<componentTypeSet>
\brief   Implementation of the has<componentTypeSet> template specialization so the componentTypeSet
         class can be used as an STL container key value type.
***************************************************************************************/
template <>
struct hash<GameObjectFiltering::componentTypeSet>
{
	/*!***************************************************************************************
	\brief  Executes the hash value calculation.
	\param p_key - The set to calculate the hash value for.
	\return std::size_t - The calculated hash value.
	*****************************************************************************************/
	std::size_t operator()(const GameObjectFiltering::componentTypeSet & p_key) const
	{
		return p_key.hashValue();
	}
};

} // namespace std

