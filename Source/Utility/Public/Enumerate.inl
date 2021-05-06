#pragma once
#include <type_traits> // is_base_of_v
#include <utility>     // iterator_traits
#include <tuple>       // tie, tuple_cat, get

namespace tuple_goop
{
	// from https://stackoverflow.com/a/26540297
	template <class... T, std::size_t... i>
	constexpr auto to_tuple_ref_helper(std::tuple<T...>& t, std::index_sequence<i...>) noexcept
	{
		return std::tuple<T&...> {std::get<i>(t)...};
	}

	template <class... T>
	constexpr auto to_tuple_ref(std::tuple<T...>& t) noexcept
	{
		return to_tuple_ref_helper(t, std::make_index_sequence<sizeof...(T)>());
	}

	template <class T>
	constexpr auto to_tuple_ref(T& t) noexcept
	{
		return std::tuple<T&>{t};
	}

	template <class... Ts>
	constexpr auto flattening_tie(Ts &...args) noexcept
	{
		return std::tuple_cat(to_tuple_ref(args)...);
	}
} // namespace tuple goop

namespace extended
{
	///////////////////////////////////////////////////////////////////////////
	// iterator detection goo

	template<class Iter>
	constexpr bool is_output_iterator_v =
		std::is_base_of_v<
		std::output_iterator_tag,
		std::iterator_traits<Iter>::iterator_category
		>;

	template<class Iter>
	constexpr bool is_input_iterator_v =
		std::is_base_of_v<
		std::input_iterator_tag,
		typename std::iterator_traits<Iter>::iterator_category
		>;

	template<class Iter>
	constexpr bool is_forward_iterator_v =
		std::is_base_of_v<
		std::forward_iterator_tag,
		typename std::iterator_traits<Iter>::iterator_category
		>;

	template<class Iter>
	constexpr bool is_bidirectional_iterator_v =
		std::is_base_of_v<
		std::bidirectional_iterator_tag,
		typename std::iterator_traits<Iter>::iterator_category
		>;

	template<class Iter>
	constexpr bool is_random_access_iterator_v =
		std::is_base_of_v<
		std::random_access_iterator_tag,
		typename std::iterator_traits<Iter>::iterator_category
		>;

	template<typename InIt, typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
	class[[nodiscard]] enum_iterator
	{
		///////////////////////////////////////////////////////////////////////
		// iterator declarations

	public:
		// forward declaration needed for enumerator
		using Traits = typename std::iterator_traits<InIt>;
		using difference_type = typename Traits::difference_type;

		// forward declaration of members needed for value_type
	private:
		difference_type i;
		InIt iter;

		template<typename First, typename Second>
		static constexpr auto dereference_as_tuple(difference_type const& i, std::pair<First, Second> & pair) noexcept
		{
			return std::tie(i, pair.first, pair.second);
		}

		template<typename... Args>
		static constexpr auto dereference_as_tuple(difference_type const& i, std::tuple<Args...> & tuple) noexcept
		{
			return tuple_goop::flattening_tie(i, tuple);
		}

		template<typename T>
		static constexpr auto dereference_as_tuple(difference_type const& i, T & val) noexcept
		{
			return std::tie<difference_type const&>(i, val);
		}

	public:
		using iterator_category = typename Traits::iterator_category;
		using value_type = decltype(dereference_as_tuple(i, *iter));
		using pointer = value_type * ;
		using reference = value_type & ;

		static_assert(is_input_iterator_v<InIt>,
			"Enumerate expects an input iterator");

		///////////////////////////////////////////////////////////////////////
		// ctors and dtors

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr enum_iterator(difference_type i = 0, InIt const& iter = {})
			noexcept(std::is_nothrow_copy_constructible_v<InIt>)
			: i{i}
			, iter{iter}
		{}

		~enum_iterator() noexcept = default;

		constexpr enum_iterator(enum_iterator&&) noexcept = default;
		constexpr enum_iterator(enum_iterator const&) noexcept = default;

		constexpr enum_iterator& operator=(enum_iterator&&) noexcept = default;
		constexpr enum_iterator& operator=(enum_iterator const&) noexcept = default;

		///////////////////////////////////////////////////////////////////////
		// input iterator operators

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr enum_iterator& operator++()
		{
			++iter;
			++i;
			return *this;
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr enum_iterator* operator->() noexcept
		{
			return this;
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr enum_iterator const* operator->() const noexcept
		{
			return this;
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr value_type operator*() const
		{
			return dereference_as_tuple(i, *iter);
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr bool operator!=(enum_iterator const& rhs) const noexcept
		{
			return iter != rhs.iter;
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr bool operator==(enum_iterator const& rhs) const noexcept
		{
			return iter == rhs.iter;
		}

		///////////////////////////////////////////////////////////////////////
		// forward iterator operators

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr enum_iterator operator++(int)
		{
			enum_iterator temp{ *this };
			++(*this);
			return temp;
		}

		///////////////////////////////////////////////////////////////////////
		// bidirectional-iterator operators

		template<typename enabled = std::enable_if_t<is_bidirectional_iterator_v<InIt>>>
		constexpr enum_iterator operator--(int)
		{
			enum_iterator temp{ *this };
			--(*this);
			return temp;
		}

		template<typename enabled = std::enable_if_t<is_bidirectional_iterator_v<InIt>>>
		constexpr enum_iterator& operator--()
		{
			--iter; --i;
			return *this;
		}

		///////////////////////////////////////////////////////////////////////
		// random-access iterator operators

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr enum_iterator operator+(difference_type const& off) const
		{
			return enum_iterator{ i + off, std::next(iter, +off) };
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr enum_iterator& operator+=(difference_type const& off)
		{
			return *this = *this + off;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr enum_iterator operator-(difference_type const& off) const
		{
			return enum_iterator{ i - off,  std::prev(iter, off) };
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr enum_iterator& operator-=(difference_type const& off)
		{
			return *this = *this - off;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr difference_type operator-(enum_iterator const& rhs) const
		{
			return iter - rhs.iter;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr value_type operator[](difference_type const& off) const
		{
			return dereference_as_tuple(i + off, iter[off]);
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr bool operator>(enum_iterator const& rhs) const noexcept
		{
			return iter > rhs.iter;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr bool operator>=(enum_iterator const& rhs) const noexcept
		{
			return iter >= rhs.iter;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr bool operator<=(enum_iterator const& rhs) const noexcept
		{
			return iter <= rhs.iter;
		}

		template<typename enabled = std::enable_if_t<is_random_access_iterator_v<InIt>>>
		constexpr bool operator<(enum_iterator const& rhs) const noexcept
		{
			return iter < rhs.iter;
		}

		///////////////////////////////////////////////////////////////////////
		// iterator-specific methods

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr difference_type const& _count() const noexcept
		{
			return i;
		}

		template<typename enabled = std::enable_if_t<is_input_iterator_v<InIt>>>
		constexpr InIt _underlying_iter() const noexcept
		{
			return iter;
		}
	};

	template<class T>
	class[[nodiscard]] enumerated
	{
	private:
		T& container;

	public:
		using iterator = enum_iterator<decltype(std::begin(container))>;

		///////////////////////////////////////////////////////////////////////
		// ctors and dtors
		constexpr enumerated(T& c) noexcept : container{ c } {}
		constexpr enumerated(enumerated&&) noexcept = default;
		constexpr enumerated(enumerated const&) noexcept = default;

		~enumerated() noexcept = default;

		constexpr enumerated& operator=(enumerated&&) noexcept = default;
		constexpr enumerated& operator=(enumerated const&) noexcept = default;

		///////////////////////////////////////////////////////////////////////
		// range

		[[nodiscard]]
		constexpr iterator begin() const noexcept
		{
			return iterator{ 0, std::begin(container) };
		}

		[[nodiscard]]
		constexpr iterator end() const noexcept
		{
			using std::cbegin; using std::cend;
			using std::end; using std::distance;

			if constexpr (is_bidirectional_iterator_v<iterator>)
			{
				return iterator{
					distance(cbegin(container), cend(container)),
					end(container)
				};
			}
			else
			{
				return iterator{ 0, end(container) };
			}
		}
	};

	// Unless there is a more elegant way, a duplicate class is needed for const references

	template<class T>
	class[[nodiscard]] const_enumerated
	{
	private:
		T const& container;
	public:
		using iterator = enum_iterator<decltype(std::cbegin(container))>;

		///////////////////////////////////////////////////////////////////////
		// ctors and dtors

		constexpr const_enumerated(T const& c) noexcept : container{ c } {}
		constexpr const_enumerated(const_enumerated&&) noexcept = default;
		constexpr const_enumerated(const_enumerated const&) noexcept = default;

		~const_enumerated() noexcept = default;

		constexpr const_enumerated& operator=(const_enumerated&&) noexcept = default;
		constexpr const_enumerated& operator=(const_enumerated const&) noexcept = default;

		///////////////////////////////////////////////////////////////////////
		// range

		[[nodiscard]]
		constexpr iterator begin() const noexcept
		{
			return iterator{ 0, std::cbegin(container) };
		}

		[[nodiscard]]
		constexpr iterator end() const noexcept
		{
			using std::cbegin; using std::cend; using std::distance;

			if constexpr (is_bidirectional_iterator_v<iterator>)
			{
				return iterator{
					distance(cbegin(container), cend(container)),
					cend(container)
				};
			}
			else
			{
				return iterator{ 0, cend(container) };
			}
		}
	};


} // namespace extended

namespace std
{
	///////////////////////////////////////////////////////////////////////////
	// template speciliaztions

	template<typename Iter>
	[[nodiscard]]
	constexpr typename iterator_traits<Iter>::difference_type
		distance(
			extended::enum_iterator<Iter> begin,
			extended::enum_iterator<Iter> const& end
		) noexcept
	{
		if constexpr (extended::is_random_access_iterator_v<Iter>)
		{
			return end - begin;
		}
		if constexpr (extended::is_bidirectional_iterator_v<Iter>)
		{
			return end._count() - begin._count();
		}
		else
		{
			return distance(begin._underlying_iter(), end._underlying_iter());
		}
	}

	template<typename Iter1, typename Iter2>
	void iter_swap(extended::enum_iterator<Iter1> const& lhs, extended::enum_iterator<Iter2> const& rhs)
	{
		iter_swap(lhs._underlying_iter(), rhs._underlying_iter());
	}

	template<typename Iter1, typename Iter2>
	void iter_swap(extended::enum_iterator<Iter1> const& lhs, Iter2 const& rhs)
	{
		iter_swap(lhs._underlying_iter(), rhs);
	}

	template<typename Iter1, typename Iter2>
	void iter_swap(Iter1 const& lhs, extended::enum_iterator<Iter2> const& rhs)
	{
		iter_swap(lhs, rhs._underlying_iter());
	}

	template<typename Iter>
	void swap(extended::enum_iterator<Iter>&& lhs, extended::enum_iterator<Iter>&& rhs)
	{
		extended::enum_iterator temp = std::move(lhs);
		lhs = std::move(rhs);
		rhs = std::move(lhs);
	}
} // namespace std
