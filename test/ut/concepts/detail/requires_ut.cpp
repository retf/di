//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/detail/requires.hpp"

#include <boost/test/unit_test.hpp>

#include "common/fakes/fake_scope.hpp"
#include "common/fakes/fake_concept.hpp"

namespace boost {
namespace di {
namespace concepts {
namespace detail {

BOOST_AUTO_TEST_CASE(empty_factors) {
    BOOST_CHECK_EQUAL(1, (requires<>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
    BOOST_CHECK_EQUAL(1, (requires<>::apply<int, mpl::vector<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(simple_factor_pass) {
    BOOST_CHECK_EQUAL(10, (requires<fake_concept<10>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(simple_factor_fail) {
    BOOST_CHECK_EQUAL(0, (requires<fake_concept<0>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(many_factors_pass) {
    BOOST_CHECK_EQUAL(100, (requires<fake_concept<1>, fake_concept<1>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
    BOOST_CHECK_EQUAL(2000000, (requires<fake_concept<1>, fake_concept<1>, fake_concept<2>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(many_factors_fail) {
    BOOST_CHECK_EQUAL(0, (requires<fake_concept<1>, fake_concept<0>, fake_concept<1>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
    BOOST_CHECK_EQUAL(0, (requires<fake_concept<1>, fake_concept<1>, fake_concept<0>, fake_concept<2>>::apply<void, mpl::vector<>, fake_scope<>>::type::value));
}

BOOST_AUTO_TEST_CASE(many_factors_pass_custom_multiplication_factor) {
    BOOST_CHECK_EQUAL(8, (requires<fake_concept<1>, fake_concept<1>, fake_concept<1>>::apply<void, mpl::vector<>, fake_scope<>, mpl::int_<2>>::type::value));
}

} // namespace detail
} // namespace concepts
} // namespace di
} // namespace boost
