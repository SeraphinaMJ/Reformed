/*!***************************************************************************************
\file       ZipTests.cpp
\author     Tyler Robbins and Patrick Cook
\date       10/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implements the Python zip() class in C++.
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <tuple>

/*!***************************************************************************************
\brief  Helper function. Converts the given parameter into a tuple type of just the type
        of the result of that parameter's begin() method.
\details This function is to remain unimplemented, as it is only meant to be called in
         unevaluated contexts.
\tparam List1 The C++ container type.
\return Nothing.
*****************************************************************************************/
template<typename List1>
auto createZipTupleType() -> std::tuple<decltype(*std::begin(std::declval<List1>()))>;

/*!***************************************************************************************
\brief  Helper function. Converts the given parameters into a tuple type of just the types
        of the results of those parameters' begin() methods.
\details This function is to remain unimplemented, as it is only meant to be called in
         unevaluated contexts.
\tparam Lists The C++ container types.
\return Nothing.
*****************************************************************************************/
template<typename... Lists>
using ZipTupleType = decltype(std::tuple_cat(createZipTupleType<Lists>()...));

//-----------------------------------------------------------------------------------------------------------------------

/*!***************************************************************************************
\brief  Helper function. Converts the given parameter into a tuple type of just those
        types' end iterators.
\details This function is to remain unimplemented, as it is only meant to be called in
         unevaluated contexts. Will work on any type for which std::end() may be called on
         it.
\tparam List1 The C++ container type.
\return Nothing.
*****************************************************************************************/
template<typename List1>
auto createEndIteratorType() -> std::tuple<decltype(std::end(std::declval<List1>()))>;

/*!***************************************************************************************
\brief  Helper function. Converts the given parameters into a tuple type of just those
        types' end iterators.
\details This function is to remain unimplemented, as it is only meant to be called in
         unevaluated contexts.
\tparam Lists The C++ container types.
\return Nothing.
*****************************************************************************************/
template<typename... Lists>
using EndIteratorTypes = decltype(std::tuple_cat(createEndIteratorType<Lists>()...));

/*!***************************************************************************************
\par class: Zipper
\tparam  Lists All C++ container types to be zipped together.
\brief   Container-like class which zips together multiple lists. Do not construct this
         manually, use the zip() function instead.
*****************************************************************************************/
template<typename... Lists>
struct Zipper {
    class iterator;

    
    using const_iterator = iterator; //!< The constant iterator alias.

    //! The type of each value in this container
    using value_type = ZipTupleType<Lists...>; 

    //! The same as value_type
    using reference = value_type;

    //! const value_type
    using const_reference = const value_type;

    //! std::ptrdiff_t
    using difference_type = std::ptrdiff_t;

    //! std::size_t
    using size_type = std::size_t;

    protected:

        /**
         * @brief Implementation of the begin() method.
         *
         * @tparam Indices All indices for each member of Lists
         * @param l The tuple of lists.
         * @param UNUSED Unused.
         *
         * @return An iterator to the start of this container.
         */
        template<std::size_t... Indices>
        iterator begin_impl(const std::tuple<Lists...>& l, std::index_sequence<Indices...> /*UNUSED*/) const {
            return iterator(std::make_tuple(std::get<Indices>(l).begin() ...));
        }

        /**
         * @brief Implementation of the end() method.
         *
         * @tparam Indices All indices for each member of Lists
         * @param l The tuple of lists.
         * @param UNUSED Unused.
         *
         * @return An iterator to the end of this container.
         */
        template<std::size_t... Indices>
        iterator end_impl(const std::tuple<Lists...>& l, std::index_sequence<Indices...> /*UNUSED*/) const {
            return iterator(std::make_tuple(std::get<Indices>(l).end() ...));
        }

        /**
         * @brief Implementation of the size() method.
         *
         * @tparam Indices All indices for each member of Lists
         * @param UNUSED Unused.
         *
         * @return The number of elements that one can get through iterating 
         *         over this container.
         */
        template<std::size_t... Indices>
        size_type size_impl(std::index_sequence<Indices...> /*UNUSED*/) const {
            using expand_type = std::size_t[];
            expand_type _{static_cast<std::size_t>(std::size(std::get<Indices>(m_lists)))...};
            return std::min_element(std::begin(_), std::end(_));
        }

    public:
        /**
         * @brief Constructs an empty Zipper
         */
        Zipper() { }

        /**
         * @brief Constructs a Zipper with all lists provided.
         * @param lists All lists to construct this Zipper with.
         */
        Zipper(Lists&&... lists): m_lists(std::tie(lists...))
        { }

        /**
         * @brief Copies another Zipper.
         */
        Zipper(const Zipper<Lists...>& other) = default;

        /**
         * @brief Moves another Zipper.
         */
        Zipper(Zipper<Lists...>&& other): m_lists(std::move(other.m_lists))
        { }

        /**
         * @brief Destructs this Zipper.
         */
        ~Zipper() = default;

        /**
         * @brief Assigns the lists in another Zipper to here by copy.
         * @param other The other Zipper.
         * @return A reference to this Zipper after assignment.
         */
        Zipper<Lists...>& operator=(const Zipper<Lists...>& other) {
            m_lists = other.m_lists;
            return *this;
        }

        /**
         * @brief Assigns the lists in another Zipper to here by move.
         * @param other The other Zipper.
         * @return A reference to this Zipper after assignment.
         */
        Zipper<Lists...>& operator=(Zipper<Lists...>&& other) {
            m_lists = std::move(other.m_lists);
            return *this;
        }

        /**
         * @brief The iterator for a Zipper.
         */
        class iterator {
            public:
                /**
                 * @brief Constructs a Zipper::iterator.
                 *
                 * @param iterators A tuple of each iterator for Lists.
                 */
                iterator(EndIteratorTypes<Lists...> iterators): m_iterators(iterators)
                { }

                /**
                 * @brief Dereferences this iterator.
                 *
                 * @return A tuple of the element from each list at their
                 *         equivalent position.
                 */
                reference operator*() const {
                    return dereference<reference>(m_iterators);
                }

                /**
                 * @brief Increments this iterator.
                 *
                 * @return This iterator after incrementing.
                 */
                auto& operator++() {
                    increment(m_iterators);
                    return *this;
                }

                /**
                 * @brief Increments this iterator.
                 *
                 * @return A copy of this iterator before incrementing.
                 */
                typename Zipper<Lists...>::iterator operator++(int) {
                    auto copy = *this;
                    increment(m_iterators);
                    return copy;
                }

                /**
                 * @brief Compares this iterator to another.
                 *
                 * @param other The other iterator.
                 *
                 * @return true if this iterator is not equal to other, false
                 *         otherwise.
                 */
                bool operator!=(const Zipper<Lists...>::iterator& other) const {
                    return !isEqual(m_iterators, other.m_iterators);
                }

                /**
                 * @brief Compares this iterator to another.
                 *
                 * @param other The other iterator.
                 *
                 * @return true if this iterator is equal to other, false
                 *         otherwise.
                 */
                bool operator==(const Zipper<Lists...>::iterator& other) const {
                    return isEqual(m_iterators, other.m_iterators);
                }

            private:
                /**
                 * @brief Implements isEqual method. Checks if each value is
                 *        equal between the two given tuples.
                 *
                 * @tparam T The tuple type to check.
                 * @tparam Indices All indices for each member of Lists
                 * @param a The first tuple.
                 * @param b The second tuple.
                 * @param UNUSED Unused.
                 *
                 * @return true if a is equal to b, false otherwise.
                 */
                template<typename T, std::size_t... Indices>
                bool isEqual_impl(const T& a, const T& b, std::index_sequence<Indices...> /*UNUSED*/) const {
                    return ((std::get<Indices>(a) == std::get<Indices>(b)) | ...);
                }

                /**
                 * @brief Checks if two EndIteratorTypes<Lists...> are equal.
                 *
                 * @param a The first EndIteratorTypes<Lists...>
                 * @param b The first EndIteratorTypes<Lists...>
                 *
                 * @return true if they are equal, false otherwise.
                 */
                bool isEqual(const EndIteratorTypes<Lists...>& a, const EndIteratorTypes<Lists...>& b) const {
                    return isEqual_impl(a, b, std::make_index_sequence<sizeof...(Lists)>());
                }

                /**
                 * @brief Implements increment method. Increments each stored
                 *        internal iterator.
                 *
                 * @tparam T The tuple type to increment.
                 * @tparam Indices All indices for each member of Lists.
                 * @param a The tuple to increment.
                 * @param UNUSED Unused.
                 */
                template<typename T, std::size_t... Indices>
                void increment_impl(T& a, std::index_sequence<Indices...> /*UNUSED*/) {
                    (void)std::make_tuple((++std::get<Indices>(a)) ...);
                }

                /**
                 * @brief Increments the given EndIteratorTypes<Lists...>
                 *
                 * @param a The EndIteratorTypes<Lists...>
                 */
                void increment(EndIteratorTypes<Lists...>& a) {
                    increment_impl(a, std::make_index_sequence<sizeof...(Lists)>());
                }

                /**
                 * @brief Implements the dereference method. Dereferences each
                 *        stored internal iterator.
                 *
                 * @tparam T The tuple type to return
                 * @tparam IterTuple The tuple type of iterators to dereference
                 * @tparam Indices All indices for each member of Lists.
                 * @param a The tuple to dereference.
                 * @param UNUSED Unused.
                 *
                 * @return A std::tie() of the result of dereferencing each
                 *         iterator in a.
                 */
                template<typename T, typename IterTuple, std::size_t... Indices>
                T dereference_impl(const IterTuple& a, std::index_sequence<Indices...>) const {
                    return std::tie(*std::get<Indices>(a) ...);
                }

                /**
                 * @brief Dereferences the given EndIteratorTypes<Lists...>
                 *
                 * @tparam T The tuple type to return.
                 * @param a The EndIteratorTypes<Lists...>
                 *
                 * @return The result of dereference_impl
                 */
                template<typename T>
                T dereference(const EndIteratorTypes<Lists...>& a) const {
                    return dereference_impl<T>(a, std::make_index_sequence<sizeof...(Lists)>());
                }

                //! A tuple of iterators, one for each type in Lists...
                EndIteratorTypes<Lists...> m_iterators;
        };

        /**
         * @brief Returns an iterator to the front of this Zipper
         *
         * @return An iterator to the front of this Zipper.
         */
        iterator begin() {
            return begin_impl(m_lists, std::make_index_sequence<sizeof...(Lists)>());
        }

        /**
         * @brief Returns an iterator to the front of this Zipper
         *
         * @return An iterator to the front of this Zipper.
         */
        const_iterator begin() const {
            return begin_impl(m_lists, std::make_index_sequence<sizeof...(Lists)>());
        }

        /**
         * @brief Returns an iterator to the front of this Zipper
         *
         * @return An iterator to the front of this Zipper.
         */
        const_iterator cbegin() const {
            return begin();
        }

        /**
         * @brief Returns an iterator to the end of this Zipper
         *
         * @return An iterator to the end of this Zipper.
         */
        iterator end() {
            return end_impl(m_lists, std::make_index_sequence<sizeof...(Lists)>());
        }

        /**
         * @brief Returns an iterator to the end of this Zipper
         *
         * @return An iterator to the end of this Zipper.
         */
        const_iterator end() const {
            return end_impl(m_lists, std::make_index_sequence<sizeof...(Lists)>());
        }

        /**
         * @brief Returns an iterator to the end of this Zipper
         *
         * @return An iterator to the end of this Zipper.
         */
        const_iterator cend() const {
            return end();
        }

        /**
         * @brief Returns the size of this Zipper
         *
         * @return The size of this Zipper
         */
        size_type size() const {
            return size_impl(std::make_index_sequence<sizeof...(Lists)>());
        }

        /**
         * @brief Returns the maximum size of this Zipper
         *
         * @return The maximum size of this Zipper
         */
        size_type max_size() const {
            return size();
        }

        /**
         * @brief Returns if this Zipper is empty.
         *
         * @return true if this Zipper is empty, false otherwise.
         */
        bool empty() const {
            return begin() == end();
        }

        /**
         * @brief Swaps this Zipper for another.
         *
         * @param other The Zipper to swap with.
         */
        void swap(Zipper<Lists...>& other) {
            std::swap(m_lists, other.m_lists);
        }

        /**
         * @brief Compares this Zipper to another.
         *
         * @param other The Zipper to compare with
         *
         * @return true if this Zipper is equal to the other Zipper, false
         *         otherwise.
         */
        bool operator==(const Zipper<Lists...>& other) const {
            return m_lists == other.m_lists;
        }

        /**
         * @brief Compares this Zipper to another.
         *
         * @param other The Zipper to compare with
         *
         * @return true if this Zipper is not equal to the other Zipper, false
         *         otherwise.
         */
        bool operator!=(const Zipper<Lists...>& other) const {
            return m_lists != other.m_lists;
        }

    private:
        //! All Lists that this Zipper is referring to.
        std::tuple<Lists...> m_lists;
};

/**
 * @brief Combines multiple containers together into one container view.
 *
 * @tparam Lists A set of types that follow the Container named requirement.
 * @param lists All Containers to zip together.
 *
 * @return A Zipper object of all given Lists...
 */
template<typename... Lists>
auto zip(Lists&&... lists) -> Zipper<Lists...> {
    return Zipper<Lists...>(std::forward<Lists>(lists)...);
}

