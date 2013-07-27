//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/binder.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "boost/di/named.hpp"
#include "fake_dependency.hpp"
#include "fake_dependency_base_of.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace detail {

struct fake_scope { };

template<
    template<typename> class = std::allocator
>
struct fake_scope_allocator { };

BOOST_AUTO_TEST_CASE(empty) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                int
              , mpl::vector0<>
              , mpl::vector0<>
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(one) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                int
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, float>
          , binder_impl<
                float
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, float>
                  , fake_dependency<fake_scope, double>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(found_allocator) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope_allocator<>, float>
          , binder_impl<
                float
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope_allocator<>, int>
                  , fake_dependency<fake_scope_allocator<>, float>
                  , fake_dependency<fake_scope_allocator<>, double>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope_allocator<>, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(found_many) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, float>
          , binder_impl<
                float
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, float>
                  , fake_dependency<fake_scope, float>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(not_found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, double>
          , binder_impl<
                double
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, float>
                  , fake_dependency<fake_scope, float>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(if_base) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, if0, c0if0>
          , binder_impl<
                if0
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, if0, c0if0>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
          , binder_impl<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_if) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, if0, c0if0>
          , binder_impl<
                if0
              , mpl::vector<c1>
              , mpl::vector<
                    fake_dependency<fake_scope, if0, c0if0>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_many) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
          , binder_impl<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_many_end) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
          , binder_impl<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_not_found) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                int
              , mpl::vector<a>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_other_types) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
          , binder_impl<
                int
              , mpl::vector<a, b>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, b> >
                  , fake_dependency<fake_scope, float, float, mpl::vector<a, b> >
                  , fake_dependency<fake_scope, double>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
          , binder_impl<
                int
              , mpl::vector<a, b, c>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, a, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, a, a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<c> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_empty_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                int
              , mpl::vector<>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, a, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, a, a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<c> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_diff_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<b> >
          , binder_impl<
                int
              , mpl::vector<c, a, b>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, a, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, a, a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<c> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_short_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, mpl::vector<c> >
          , binder_impl<
                int
              , mpl::vector<c>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, a, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, a, a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<c> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(context_long_with_order_to_long_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                int
              , mpl::vector<a, b, c, d>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, mpl::vector<a, b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b> >
                  , fake_dependency<fake_scope, int>
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, a, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<b, c, c> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<a, a, a> >
                  , fake_dependency<fake_scope, int, int, mpl::vector<c> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(base_of_fail) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, i>
          , binder_impl<
                i
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency_base_of<fake_scope, a>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(base_of_successful) {
    BOOST_CHECK((
        is_same<
            fake_dependency_base_of<fake_scope, impl>
          , binder_impl<
                i
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency_base_of<fake_scope, impl>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(complex_type) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int>
          , binder_impl<
                shared_ptr<int>
              , mpl::vector0<>
              , mpl::vector<
                    fake_dependency<fake_scope, int>
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(named_type) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, named<int, mpl::_1>, int>
          , binder_impl<
                named<shared_ptr<int>, mpl::_1>
              , mpl::vector0<>
              , mpl::vector0<>
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

BOOST_AUTO_TEST_CASE(call_call_stack) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, c1, mpl::vector<c2> >
          , binder_impl<
                int
              , mpl::vector<c2>
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, c1, mpl::vector<c2> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c1>
  , mpl::vector<c2, c3>
> multiple_calls_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls, TContext, multiple_calls_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, c1, mpl::vector<c2, c3> >
          , typename binder_impl<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, c1, mpl::vector<c2, c3> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
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
> multiple_calls_many_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_many, TContext, multiple_calls_many_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int, c1, c5, mpl::vector<c3, c4> >
          , typename binder_impl<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, c1, c5, mpl::vector<c3, c4> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

typedef mpl::vector<
    mpl::vector<c3>
  , mpl::vector<c6>
  , mpl::vector<c3, c2>
> multiple_calls_not_found_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(multiple_calls_not_found, TContext, multiple_calls_not_found_t) {
    BOOST_CHECK((
        is_same<
            fake_dependency<fake_scope, int, int>
          , typename binder_impl<
                int
              , TContext
              , mpl::vector<
                    fake_dependency<fake_scope, int, int, c1, c5, mpl::vector<c3, c4> >
                >
              , mpl::vector0<>
              , fake_dependency<fake_scope, mpl::_1, mpl::_2>
            >::type
        >::value
    ));
}

} // namespace detail
} // namespace di
} // namespace boost
