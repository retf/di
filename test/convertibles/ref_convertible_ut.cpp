//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/convertibles/ref.hpp"

#include <boost/test/unit_test.hpp>
#include <memory>
#include <boost/type.hpp>
#include <boost/ref.hpp>

namespace boost {
namespace di {
namespace convertibles {

BOOST_AUTO_TEST_CASE(copy_ctor) {
    int i = 42;
    ref<int> ref_i((reference_wrapper<int>(i)));
    ref<int> object_ref(ref_i);

    BOOST_CHECK_EQUAL(ref_i(type<int&>()), object_ref(type<int&>()));
}

BOOST_AUTO_TEST_CASE(to_ref) {
    int i = 42;
    int& i_ref = i;
    ref<int> object_ref((reference_wrapper<int>(i_ref)));
    BOOST_CHECK_EQUAL(i, object_ref(type<int&>()));
}

BOOST_AUTO_TEST_CASE(to_named) {
    int i = 42;
    int& i_ref = i;
    ref<int> object_ref((reference_wrapper<int>(i_ref)));
    named<int> named_ref(object_ref(type<int&>()));
    BOOST_CHECK_EQUAL(i, named_ref);
}

BOOST_AUTO_TEST_CASE(to_const_ref) {
    const int i = 42;
    const int& i_ref = i;
    ref<const int> object_ref((reference_wrapper<const int>(i_ref)));
    BOOST_CHECK_EQUAL(i, object_ref(type<const int&>()));
}

BOOST_AUTO_TEST_CASE(to_const_named_ref) {
    const int i = 42;
    const int& i_ref = i;
    ref<const int> object_ref((reference_wrapper<const int>(i_ref)));
    named<const int> named_ref(object_ref(type<const int&>()));
    BOOST_CHECK_EQUAL(i, named_ref);
}

} // namespace convertibles
} // namespace di
} // namespace boost
