/*!***************************************************************************************
\file       ComponentTypeSet.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ComponentTypeSet class member functions.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "ComponentTypeSet.hpp"
//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <cassert>
#include <algorithm>
#include <limits>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations ========================================================//

/*!***************************************************************************************
\brief helper: concatenates two bitsets using element-wise bitwise-or
\param lhs - the first parameter
\param rhs - the second parameter
\return bitset - the union of the hash of both inputs
*****************************************************************************************/
auto concat(GameObjectFiltering::componentTypeSet::bitSet const& lhs,
            GameObjectFiltering::componentTypeSet::bitSet const& rhs) noexcept
{
	// element-wise bitwise-or the two input arrays into the output
	GameObjectFiltering::componentTypeSet::bitSet newSet;
	std::transform(
		lhs.cbegin(), lhs.cend(),
		rhs.begin(),
		newSet.begin(),
		[](const size_t lhs, const size_t rhs) constexpr noexcept
			-> size_t
	{
		return lhs | rhs;
	});

	return newSet;
}


namespace GameObjectFiltering
{
	/*!************************************************************************
	\brief- Returns true if and only if the right-hand side (rhs)
	contains all of the bits set in the left-hand side (lhs);
	the the lhs is a subset of the rhs
	\param lhs - the supposed subset
	\param rhs - the supposed superset
	\return the lhs is indeed a subset of the rhs
	**************************************************************************/
	constexpr bool bitwiseIsSubsetOf(size_t lhs, size_t rhs) noexcept;

	template<typename BinOp>
	bool doCompare(componentTypeSet::bitSet const& lhs, componentTypeSet::bitSet const& rhs, BinOp comparison);

	//======== Static Data Definitions ===================================================//
	unsigned int componentTypeSet::nextCombinedIndex = 0;
	std::unordered_map<componentType, unsigned int /*index*/> componentTypeSet::indexMap;

	//======== Constructors & Destructor ===================================================//

	//======== Getters & Setters ===========================================================//
	std::size_t componentTypeSet::hashValue() const
	{
		// https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
		const auto performHash =
		[](const size_t seed, const size_t bitset) constexpr noexcept -> size_t
		{
			return seed ^ (bitset + 0x9e3779b9 + (seed << 6) + (seed >> 2));
		};

		return std::accumulate(bits.cbegin() + 1, bits.cend(), bits.front(), performHash);
	}

	componentTypeSet componentTypeSet::Empty()
	{
		return componentTypeSet{};
	}

	const componentType componentTypeSet::typeForIndex(const unsigned p_combined_index)
	{
		const auto reverseLookup =
		[p_combined_index](const auto key_valPair) constexpr noexcept
			-> bool
		{
			return key_valPair.second == p_combined_index;
		};

		if (const auto found = std::find_if(indexMap.cbegin(), indexMap.cend(), reverseLookup);
			found != indexMap.cend())
		{
			return found->first;
		}

		return componentType::forAbstract<void>();
	}

	componentTypeSet componentTypeSet::operator|(const componentTypeSet & p_other) const
	{
		return componentTypeSet(concat(bits, p_other.bits));
	}

	componentTypeSet& componentTypeSet::operator|=(const componentTypeSet & p_other)
	{
		bits = concat(bits, p_other.bits);
		return *this;
	}

	bool componentTypeSet::isEmpty() const
	{
		const auto found = std::find_if(bits.cbegin(), bits.cend(),
			[](const size_t bits) constexpr noexcept -> bool { return bits != 0; }
		);
		return found == bits.cend();
	}

	componentTypeSet::componentTypeSet(const componentType & p_type)
		: bits{ createBitSet(p_type) }
	{
	}

	void componentTypeSet::setType(const componentType p_type)
	{
		bits = concat(bits, createBitSet(p_type));
	}

	void componentTypeSet::reset()
	{
		*this = componentTypeSet();
	}


	//======== Overrides ===================================================================//
	bool componentTypeSet::operator!=(const componentTypeSet & p_other) const
	{
		return bits != p_other.bits;
	}

	bool componentTypeSet::operator==(const componentTypeSet & p_other) const
	{
		return bits == p_other.bits;
	}


	bool componentTypeSet::operator<(const componentTypeSet& p_other) const
	{
		// lhs is a subset of rhs, but rhs is not a subset of lhs
		const auto isStrongSubset =
		[](const size_t lhs, const size_t rhs) constexpr noexcept
			-> bool
		{
			return (lhs != rhs) & bitwiseIsSubsetOf(lhs, rhs);
		};

		return doCompare(bits, p_other.bits, isStrongSubset);
	}

	bool componentTypeSet::operator<=(const componentTypeSet& p_other) const
	{
		return doCompare(bits, p_other.bits, bitwiseIsSubsetOf);
	}

	//======== Functionality ===============================================================//

	//======== Helper Functions ============================================================//
	template<typename Op>
	static void iterateComponentType(componentType p_type, Op op)
	{
		// infinite looping...
		// because: cannot check if current p_type is a valid p_type, can only check if parent exists
		for (;;)
		{
			// perform the operation on the current type
			op(p_type);

			// base case: current type doesn't have a parent
			if (!p_type.hasParent())
				return;

			// continue on the parent type
			p_type = p_type.getParentType();
		}
	}

	constexpr bool bitwiseIsSubsetOf(const size_t lhs, const size_t rhs) noexcept
	{
		return !(lhs & ~rhs);
	}

	template<typename BinOp>
	bool doCompare(componentTypeSet::bitSet const& lhs, componentTypeSet::bitSet const& rhs, BinOp comparison)
	{
		return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), comparison);
	}

	unsigned int componentTypeSet::indexOf(componentType p_type)
	{
		// find if the type is already in the map
		if (const auto l_iter = indexMap.find(p_type); l_iter != indexMap.cend())
		{
			return l_iter->second; // return the existing index
		}

		// add to the map, and return the new index
		assert(nextCombinedIndex < s_blockCount * std::numeric_limits<size_t>::digits
			&& "Out of bit indices! Increment s_blockCount in ComponentTypeSet.hpp");
		return indexMap.emplace(std::move(p_type), nextCombinedIndex++).first->second;
	}

	std::vector<unsigned int>& componentTypeSet::indicesOf(const componentType p_type, std::vector<unsigned>& p_result)
	{
		// emplace back all indices of the p_type and its ancestors
		iterateComponentType(p_type, [&p_result](const componentType p_type)
		{
			p_result.emplace_back(indexOf(p_type));
		});
		return p_result;
	}

	std::vector<unsigned int> componentTypeSet::indicesOf(const componentType p_type)
	{
		std::vector<unsigned int> indices;
		// educated guess that components don't have more than 4 levels of inheritance.
		indices.reserve(4);
		(void)indicesOf(p_type, indices);
		return indices;
	}

	constexpr std::pair<unsigned, unsigned> componentTypeSet::breakdownIndex(const unsigned int p_combined_index) noexcept
	{
		constexpr unsigned int s_bitCountPerElement = std::numeric_limits<size_t>::digits;
		return std::make_pair(
			p_combined_index / s_bitCountPerElement, // element index
			p_combined_index % s_bitCountPerElement  // bit index
		);
	}

	void componentTypeSet::breakdownIndex(const unsigned int p_combined_index, unsigned int & p_element_index, unsigned int & p_bit_index)
	{
		constexpr unsigned int s_bitCountPerElement = std::numeric_limits<size_t>::digits;
		p_element_index = p_combined_index / s_bitCountPerElement;
		p_bit_index = p_combined_index % s_bitCountPerElement;
	}

	componentTypeSet::bitSet componentTypeSet::createBitSet(const componentType p_type)
	{
		bitSet set = { 0 };

		// set each bit of the type and its parent's type in the bitSet
		iterateComponentType(p_type, [&set](const componentType p_type)
		{
			const auto[l_element_index, l_bit_index] = breakdownIndex(indexOf(p_type));
			set[l_element_index] |= size_t(1) << l_bit_index;
		});

		return set;
	}

	componentTypeSet make_componentTypeSet(componentType p_type)
	{
		return componentTypeSet(p_type);
	}

	componentTypeSet make_componentTypeSet()
	{
		return componentTypeSet::Empty();
	}

	componentTypeSet::componentTypeSet(bitSet p_bitset) noexcept
		: bits{ p_bitset }
	{
	}
} // namespace GameObjectFiltering


