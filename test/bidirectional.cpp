// Copyright (C) 2019 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#define BOOST_STL_INTERFACES_DISABLE_CMCSTL2
#include <boost/stl_interfaces/iterator_interface.hpp>

#include "ill_formed.hpp"

#include <gtest/gtest.h>

#include <array>
#include <numeric>
#include <list>
#include <type_traits>


struct basic_bidirectional_iter : boost::stl_interfaces::iterator_interface<
                                      basic_bidirectional_iter,
                                      std::bidirectional_iterator_tag,
                                      int>
{
    basic_bidirectional_iter() : it_(nullptr) {}
    basic_bidirectional_iter(int * it) : it_(it) {}

    int & operator*() const { return *it_; }
    basic_bidirectional_iter & operator++()
    {
        ++it_;
        return *this;
    }
    basic_bidirectional_iter & operator--()
    {
        --it_;
        return *this;
    }
    friend bool operator==(
        basic_bidirectional_iter lhs, basic_bidirectional_iter rhs) noexcept
    {
        return lhs.it_ == rhs.it_;
    }

    using base_type = boost::stl_interfaces::iterator_interface<
        basic_bidirectional_iter,
        std::bidirectional_iterator_tag,
        int>;
    using base_type::operator++;
    using base_type::operator--;

private:
    int * it_;
};

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    basic_bidirectional_iter, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    basic_bidirectional_iter,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int,
    int &,
    int *,
    std::ptrdiff_t)

static_assert(
    !boost::stl_interfaces::v1::v1_dtl::
        plus_eq<basic_bidirectional_iter, std::ptrdiff_t>::value,
    "");

struct basic_adapted_bidirectional_ptr_iter
    : boost::stl_interfaces::iterator_interface<
          basic_adapted_bidirectional_ptr_iter,
          std::bidirectional_iterator_tag,
          int>
{
    basic_adapted_bidirectional_ptr_iter() : it_(nullptr) {}
    basic_adapted_bidirectional_ptr_iter(int * it) : it_(it) {}

private:
    friend boost::stl_interfaces::access;
    int *& base_reference() noexcept { return it_; }
    int * base_reference() const noexcept { return it_; }

    int * it_;
};

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    basic_adapted_bidirectional_ptr_iter, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    basic_adapted_bidirectional_ptr_iter,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int,
    int &,
    int *,
    std::ptrdiff_t)

struct basic_adapted_bidirectional_list_iter
    : boost::stl_interfaces::iterator_interface<
          basic_adapted_bidirectional_list_iter,
          std::bidirectional_iterator_tag,
          int>
{
    basic_adapted_bidirectional_list_iter() : it_() {}
    basic_adapted_bidirectional_list_iter(std::list<int>::iterator it) : it_(it) {}

private:
    friend boost::stl_interfaces::access;
    std::list<int>::iterator & base_reference() noexcept { return it_; }
    std::list<int>::iterator base_reference() const noexcept { return it_; }

    std::list<int>::iterator it_;
};

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    basic_adapted_bidirectional_list_iter, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    basic_adapted_bidirectional_list_iter,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int,
    int &,
    int *,
    std::ptrdiff_t)

template<typename ValueType>
struct adapted_bidirectional_ptr_iter
    : boost::stl_interfaces::iterator_interface<
          adapted_bidirectional_ptr_iter<ValueType>,
          std::bidirectional_iterator_tag,
          ValueType>
{
    adapted_bidirectional_ptr_iter() : it_(nullptr) {}
    adapted_bidirectional_ptr_iter(ValueType * it) : it_(it) {}

    template<typename ValueType2>
    adapted_bidirectional_ptr_iter(
        adapted_bidirectional_ptr_iter<ValueType2> other) :
        it_(other.it_)
    {}

    template<typename ValueType2>
    friend struct adapted_bidirectional_ptr_iter;

private:
    friend boost::stl_interfaces::access;
    ValueType *& base_reference() noexcept { return it_; }
    ValueType * base_reference() const noexcept { return it_; }

    ValueType * it_;
};

static_assert(
    !boost::stl_interfaces::v1_dtl::ra_iter<
        adapted_bidirectional_ptr_iter<int>>::value,
    "");
static_assert(
    !boost::stl_interfaces::v1_dtl::ra_iter<
        adapted_bidirectional_ptr_iter<int const>>::value,
    "");

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    adapted_bidirectional_ptr_iter<int>, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    adapted_bidirectional_ptr_iter<int>,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int,
    int &,
    int *,
    std::ptrdiff_t)
BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    adapted_bidirectional_ptr_iter<int const>, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    adapted_bidirectional_ptr_iter<int const>,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int const,
    int const &,
    int const *,
    std::ptrdiff_t)

template<typename ValueType>
struct bidirectional_iter : boost::stl_interfaces::iterator_interface<
                                bidirectional_iter<ValueType>,
                                std::bidirectional_iterator_tag,
                                ValueType>
{
    bidirectional_iter() : it_(nullptr) {}
    bidirectional_iter(ValueType * it) : it_(it) {}
    template<
        typename ValueType2,
        typename E = std::enable_if_t<std::is_convertible<
            typename ValueType2::value_type *,
            ValueType *>::value>>
    bidirectional_iter(ValueType2 it) : it_(it.it_)
    {}

    ValueType & operator*() const { return *it_; }
    bidirectional_iter & operator++()
    {
        ++it_;
        return *this;
    }
    bidirectional_iter & operator--()
    {
        --it_;
        return *this;
    }
    friend bool
    operator==(bidirectional_iter lhs, bidirectional_iter rhs) noexcept
    {
        return lhs.it_ == rhs.it_;
    }

    using base_type = boost::stl_interfaces::iterator_interface<
        bidirectional_iter<ValueType>,
        std::bidirectional_iterator_tag,
        ValueType>;
    using base_type::operator++;
    using base_type::operator--;

private:
    ValueType * it_;

    template<typename ValueType2>
    friend struct bidirectional_iter;
};

using bidirectional = bidirectional_iter<int>;
using const_bidirectional = bidirectional_iter<int const>;

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    bidirectional, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    bidirectional,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int,
    int &,
    int *,
    std::ptrdiff_t)

BOOST_STL_INTERFACES_STATIC_ASSERT_CONCEPT(
    const_bidirectional, std::bidirectional_iterator)
BOOST_STL_INTERFACES_STATIC_ASSERT_ITERATOR_TRAITS(
    const_bidirectional,
    std::bidirectional_iterator_tag,
    std::bidirectional_iterator_tag,
    int const,
    int const &,
    int const *,
    std::ptrdiff_t)


std::array<int, 10> ints = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};


TEST(bidirectional, basic_std_copy)
{
    basic_bidirectional_iter first(ints.data());
    basic_bidirectional_iter last(ints.data() + ints.size());

    {
        std::array<int, 10> ints_copy;
        std::copy(first, last, ints_copy.begin());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> ints_copy;
        std::copy(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            ints_copy.begin());
        std::reverse(ints_copy.begin(), ints_copy.end());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> iota_ints;
        basic_bidirectional_iter first(iota_ints.data());
        basic_bidirectional_iter last(iota_ints.data() + iota_ints.size());
        std::iota(first, last, 0);
        EXPECT_EQ(iota_ints, ints);
    }

    {
        std::array<int, 10> iota_ints;
        basic_bidirectional_iter first(iota_ints.data());
        basic_bidirectional_iter last(iota_ints.data() + iota_ints.size());
        std::iota(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            0);
        std::reverse(iota_ints.begin(), iota_ints.end());
        EXPECT_EQ(iota_ints, ints);
    }
}

TEST(bidirectional, basic_adapted_ptr_std_copy)
{
    basic_adapted_bidirectional_ptr_iter first(ints.data());
    basic_adapted_bidirectional_ptr_iter last(ints.data() + ints.size());

    {
        std::array<int, 10> ints_copy;
        std::copy(first, last, ints_copy.begin());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> ints_copy;
        std::copy(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            ints_copy.begin());
        std::reverse(ints_copy.begin(), ints_copy.end());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> iota_ints;
        basic_adapted_bidirectional_ptr_iter first(iota_ints.data());
        basic_adapted_bidirectional_ptr_iter last(
            iota_ints.data() + iota_ints.size());
        std::iota(first, last, 0);
        EXPECT_EQ(iota_ints, ints);
    }

    {
        std::array<int, 10> iota_ints;
        basic_adapted_bidirectional_ptr_iter first(iota_ints.data());
        basic_adapted_bidirectional_ptr_iter last(
            iota_ints.data() + iota_ints.size());
        std::iota(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            0);
        std::reverse(iota_ints.begin(), iota_ints.end());
        EXPECT_EQ(iota_ints, ints);
    }
}

TEST(bidirectional, basic_adapted_list_std_copy)
{
    std::list<int> ints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    basic_adapted_bidirectional_list_iter first(ints.begin());
    basic_adapted_bidirectional_list_iter last(ints.end());

    {
        std::list<int> ints_copy;
        std::copy(first, last, std::back_inserter(ints_copy));
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::list<int> ints_copy;
        std::copy(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            std::back_inserter(ints_copy));
        std::reverse(ints_copy.begin(), ints_copy.end());
        EXPECT_EQ(ints_copy, ints);
    }
}

TEST(bidirectional, mutable_to_const_conversions)
{
    {
        bidirectional first(ints.data());
        bidirectional last(ints.data() + ints.size());
        const_bidirectional first_copy(first);
        const_bidirectional last_copy(last);
        std::equal(first, last, first_copy, last_copy);
    }

    {
        adapted_bidirectional_ptr_iter<int> first(ints.data());
        adapted_bidirectional_ptr_iter<int> last(ints.data() + ints.size());
        adapted_bidirectional_ptr_iter<int const> first_copy(
            (int const *)ints.data());
        adapted_bidirectional_ptr_iter<int const> last_copy(
            (int const *)ints.data() + ints.size());
        std::equal(first, last, first_copy, last_copy);
    }
}

TEST(bidirectional, mutable_to_const_comparisons)
{
    {
        bidirectional first(ints.data());
        bidirectional last(ints.data() + ints.size());
        const_bidirectional first_const(first);
        const_bidirectional last_const(last);

        EXPECT_EQ(first, first_const);
        EXPECT_EQ(first_const, first);
        EXPECT_NE(first, last_const);
        EXPECT_NE(last_const, first);
    }

    {
        adapted_bidirectional_ptr_iter<int> first(ints.data());
        adapted_bidirectional_ptr_iter<int> last(ints.data() + ints.size());
        adapted_bidirectional_ptr_iter<int const> first_const(
            (int const *)ints.data());
        adapted_bidirectional_ptr_iter<int const> last_const(
            (int const *)ints.data() + ints.size());

        static_assert(
            !boost::stl_interfaces::v1_dtl::ra_iter<
                adapted_bidirectional_ptr_iter<int>>::value,
            "");

        EXPECT_EQ(first, first_const);
        EXPECT_EQ(first_const, first);
        EXPECT_NE(first, last_const);
        EXPECT_NE(last_const, first);
    }
}

TEST(bidirectional, postincrement_preincrement)
{
    {
        bidirectional first(ints.data());
        bidirectional last(ints.data() + ints.size());
        while (first != last && !(first == last))
            first++;
    }

    {
        bidirectional first(ints.data());
        bidirectional last(ints.data() + ints.size());
        while (first != last && !(first == last))
            last--;
    }

    {
        basic_bidirectional_iter first(ints.data());
        basic_bidirectional_iter last(ints.data() + ints.size());
        while (first != last && !(first == last))
            first++;
    }

    {
        basic_bidirectional_iter first(ints.data());
        basic_bidirectional_iter last(ints.data() + ints.size());
        while (first != last && !(first == last))
            last--;
    }

    {
        basic_adapted_bidirectional_ptr_iter first(ints.data());
        basic_adapted_bidirectional_ptr_iter last(ints.data() + ints.size());
        while (first != last && !(first == last))
            first++;
    }

    {
        basic_adapted_bidirectional_ptr_iter first(ints.data());
        basic_adapted_bidirectional_ptr_iter last(ints.data() + ints.size());
        while (first != last && !(first == last))
            last--;
    }

    {
        std::list<int> ints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        basic_adapted_bidirectional_list_iter first(ints.begin());
        basic_adapted_bidirectional_list_iter last(ints.end());
        while (first != last && !(first == last))
            first++;
    }

    {
        std::list<int> ints = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        basic_adapted_bidirectional_list_iter first(ints.begin());
        basic_adapted_bidirectional_list_iter last(ints.end());
        while (first != last && !(first == last))
            last--;
    }
}

TEST(bidirectional, std_copy)
{
    bidirectional first(ints.data());
    bidirectional last(ints.data() + ints.size());

    {
        std::array<int, 10> ints_copy;
        std::copy(first, last, ints_copy.begin());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> ints_copy;
        std::copy(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            ints_copy.begin());
        std::reverse(ints_copy.begin(), ints_copy.end());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        std::array<int, 10> iota_ints;
        bidirectional first(iota_ints.data());
        bidirectional last(iota_ints.data() + iota_ints.size());
        std::iota(first, last, 0);
        EXPECT_EQ(iota_ints, ints);
    }

    {
        std::array<int, 10> iota_ints;
        bidirectional first(iota_ints.data());
        bidirectional last(iota_ints.data() + iota_ints.size());
        std::iota(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            0);
        std::reverse(iota_ints.begin(), iota_ints.end());
        EXPECT_EQ(iota_ints, ints);
    }
}

TEST(bidirectional, const_std_copy)
{
    const_bidirectional first(ints.data());
    const_bidirectional last(ints.data() + ints.size());

    {
        std::array<int, 10> ints_copy;
        std::copy(first, last, ints_copy.begin());
        EXPECT_EQ(ints_copy, ints);
    }

    {
        EXPECT_TRUE(std::binary_search(first, last, 3));
        EXPECT_TRUE(std::binary_search(
            std::make_reverse_iterator(last),
            std::make_reverse_iterator(first),
            3,
            std::greater<>{}));
    }
}


////////////////////
// view_interface //
////////////////////
#include "view_tests.hpp"

template<typename T>
using data_t = decltype(std::declval<T>().data());

static_assert(
    ill_formed<
        data_t,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous>>::value,
    "");
static_assert(
    ill_formed<
        data_t,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous> const>::value,
    "");

template<typename T>
using size_t_ = decltype(std::declval<T>().size());

static_assert(
    ill_formed<
        size_t_,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous>>::value,
    "");
static_assert(
    ill_formed<
        size_t_,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous> const>::value,
    "");

template<typename T>
using index_operator_t = decltype(std::declval<T>()[0]);

static_assert(
    ill_formed<
        index_operator_t,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous>>::value,
    "");
static_assert(
    ill_formed<
        index_operator_t,
        subrange<
            basic_bidirectional_iter,
            basic_bidirectional_iter,
            boost::stl_interfaces::v1::discontiguous> const>::value,
    "");

TEST(bidirectional, basic_subrange)
{
    basic_bidirectional_iter first(ints.data());
    basic_bidirectional_iter last(ints.data() + ints.size());

    auto r = range<boost::stl_interfaces::v1::discontiguous>(first, last);
    auto empty = range<boost::stl_interfaces::v1::discontiguous>(first, first);

    // range begin/end
    {
        std::array<int, 10> ints_copy;
        std::copy(r.begin(), r.end(), ints_copy.begin());
        EXPECT_EQ(ints_copy, ints);

        EXPECT_EQ(empty.begin(), empty.end());
    }

    // empty/op bool
    {
        EXPECT_FALSE(r.empty());
        EXPECT_TRUE(r);

        EXPECT_TRUE(empty.empty());
        EXPECT_FALSE(empty);

        auto const cr = r;
        EXPECT_FALSE(cr.empty());
        EXPECT_TRUE(cr);

        auto const cempty = empty;
        EXPECT_TRUE(cempty.empty());
        EXPECT_FALSE(cempty);
    }

    // front/back
    {
        EXPECT_EQ(r.front(), 0);
        EXPECT_EQ(r.back(), 9);

        auto const cr = r;
        EXPECT_EQ(cr.front(), 0);
        EXPECT_EQ(cr.back(), 9);
    }
}
