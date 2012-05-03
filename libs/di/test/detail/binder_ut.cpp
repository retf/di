//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include "boost/di/named.hpp"
#include "boost/di/detail/binder.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

template<
    typename TExpected
  , typename TGiven = TExpected
  , typename TContext0 = mpl_::na
  , typename TContext1 = mpl_::na
  , typename TContext2 = mpl_::na
>
struct dependency
{
    typedef is_same<mpl::_1, TExpected> bind;
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename Expected, typename Given>
    struct rebind
    {
        typedef dependency<
            Expected
          , Given
          , TContext0
          , TContext1
          , TContext2
        > type;
    };
};

template<
    typename TExpected
  , typename TGiven = TExpected
  , typename TContext0 = mpl_::na
  , typename TContext1 = mpl_::na
  , typename TContext2 = mpl_::na
>
struct dependency_base_of
{
    typedef is_base_of<mpl::_1, TExpected> bind;
    typedef mpl::vector<TContext0, TContext1, TContext2> context;
    typedef TExpected expected;
    typedef TGiven given;

    template<typename Expected, typename Given>
    struct rebind
    {
        typedef dependency<
            Expected
          , Given
          , TContext0
          , TContext1
          , TContext2
        > type;
    };
};

BOOST_AUTO_TEST_CASE(binder_empty)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                int
              , mpl::vector0<>
              , mpl::vector0<>
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_one)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                int
              , mpl::vector0<>
              , mpl::vector<
                    dependency<int>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_found)
{
    BOOST_CHECK((
        is_same<
            dependency<float>
          , binder<
                float
              , mpl::vector0<>
              , mpl::vector<
                    dependency<int>
                  , dependency<float>
                  , dependency<double>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_found_many)
{
    BOOST_CHECK((
        is_same<
            dependency<float>
          , binder<
                float
              , mpl::vector0<>
              , mpl::vector<
                    dependency<int>
                  , dependency<float>
                  , dependency<float>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_not_found)
{
    BOOST_CHECK((
        is_same<
            dependency<double>
          , binder<
                double
              , mpl::vector0<>
              , mpl::vector<
                    dependency<int>
                  , dependency<float>
                  , dependency<float>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_if)
{
    BOOST_CHECK((
        is_same<
            dependency<if0, c0if0>
          , binder<
                if0
              , mpl::vector0<>
              , mpl::vector<
                    dependency<if0, c0if0>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<a, b> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_if)
{
    BOOST_CHECK((
        is_same<
            dependency<if0, c0if0>
          , binder<
                if0
              , mpl::vector<c1>
              , mpl::vector<
                    dependency<if0, c0if0>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_many)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    dependency<int>
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<a, b> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_many_end)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    dependency<int>
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int, int, mpl::vector<a, b> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_not_found)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                int
              , mpl::vector<a>
              , mpl::vector<
                    dependency<int, int, mpl::vector<b> >
                  , dependency<int, int, mpl::vector<a, b> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_other_types)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<a, b> >
          , binder<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<a, b> >
                  , dependency<float, float, mpl::vector<a, b> >
                  , dependency<double>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<a, b, c> >
          , binder<
                int
              , mpl::vector<a, b, c>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a, b, c> >
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int>
                  , dependency<int, int, mpl::vector<b, c> >
                  , dependency<int, int, mpl::vector<b, a, c> >
                  , dependency<int, int, mpl::vector<b, c, c> >
                  , dependency<int, int, mpl::vector<a, a, a> >
                  , dependency<int, int, mpl::vector<c> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_empty_call_stack)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                int
              , mpl::vector<>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a, b, c> >
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int>
                  , dependency<int, int, mpl::vector<b, c> >
                  , dependency<int, int, mpl::vector<b, a, c> >
                  , dependency<int, int, mpl::vector<b, c, c> >
                  , dependency<int, int, mpl::vector<a, a, a> >
                  , dependency<int, int, mpl::vector<c> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_diff_call_stack)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<b> >
          , binder<
                int
              , mpl::vector<c, a, b>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a, b, c> >
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int>
                  , dependency<int, int, mpl::vector<b, c> >
                  , dependency<int, int, mpl::vector<b, a, c> >
                  , dependency<int, int, mpl::vector<b, c, c> >
                  , dependency<int, int, mpl::vector<a, a, a> >
                  , dependency<int, int, mpl::vector<c> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_short_call_stack)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, mpl::vector<c> >
          , binder<
                int
              , mpl::vector<c>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a, b, c> >
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int>
                  , dependency<int, int, mpl::vector<b, c> >
                  , dependency<int, int, mpl::vector<b, a, c> >
                  , dependency<int, int, mpl::vector<b, c, c> >
                  , dependency<int, int, mpl::vector<a, a, a> >
                  , dependency<int, int, mpl::vector<c> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_context_long_with_order_to_long_call_stack)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                int
              , mpl::vector<a, b, c, d>
              , mpl::vector<
                    dependency<int, int, mpl::vector<a, b, c> >
                  , dependency<int, int, mpl::vector<a> >
                  , dependency<int, int, mpl::vector<b> >
                  , dependency<int>
                  , dependency<int, int, mpl::vector<b, c> >
                  , dependency<int, int, mpl::vector<b, a, c> >
                  , dependency<int, int, mpl::vector<b, c, c> >
                  , dependency<int, int, mpl::vector<a, a, a> >
                  , dependency<int, int, mpl::vector<c> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_base_of_fail)
{
    BOOST_CHECK((
        is_same<
            dependency<i>
          , binder<
                i
              , mpl::vector0<>
              , mpl::vector<
                    dependency_base_of<a>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_base_of_successful)
{
    BOOST_CHECK((
        is_same<
            dependency_base_of<impl>
          , binder<
                i
              , mpl::vector0<>
              , mpl::vector<
                    dependency_base_of<impl>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_complex_type)
{
    BOOST_CHECK((
        is_same<
            dependency<int>
          , binder<
                shared_ptr<int>
              , mpl::vector0<>
              , mpl::vector<
                    dependency<int>
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_named_type)
{
    BOOST_CHECK((
        is_same<
            dependency< named<int, mpl::_1>, int>
          , binder<
                named< shared_ptr<int>, mpl::_1>
              , mpl::vector0<>
              , mpl::vector0<>
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(binder_call_call_stack)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, c1, mpl::vector<c2> >
          , binder<
                int
              , mpl::vector<c2>
              , mpl::vector<
                    dependency<int, int, c1, mpl::vector<c2> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c1>
  , mpl::vector<c2, c3>
> binder_multiple_calls_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(binder_multiple_calls, TContext, binder_multiple_calls_t)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, c1, mpl::vector<c2, c3> >
          , typename binder<
                int
              , TContext
              , mpl::vector<
                    dependency<int, int, c1, mpl::vector<c2, c3> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c1>
  , mpl::vector<c1, c2, c3, c4>
  , mpl::vector<c2, c3, c4>
  , mpl::vector<c3, c4>
  , mpl::vector<c5>
> binder_multiple_calls_many_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(binder_multiple_calls_many, TContext, binder_multiple_calls_many_t)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int, c1, c5, mpl::vector<c3, c4> >
          , typename binder<
                int
              , TContext
              , mpl::vector<
                    dependency<int, int, c1, c5, mpl::vector<c3, c4> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c3>
  , mpl::vector<c6>
  , mpl::vector<c3, c2>
> binder_multiple_calls_not_found_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(binder_multiple_calls_not_found, TContext, binder_multiple_calls_not_found_t)
{
    BOOST_CHECK((
        is_same<
            dependency<int, int>
          , typename binder<
                int
              , TContext
              , mpl::vector<
                    dependency<int, int, c1, c5, mpl::vector<c3, c4> >
                >
              , dependency<mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

} // namespace detail
} // namespace di
} // namespace boost
