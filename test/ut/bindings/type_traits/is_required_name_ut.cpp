//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/bindings/type_traits/is_required_name.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type.hpp>

#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

struct simple_name { };

template<typename T>
struct fake_data {
    typedef T type;
};

BOOST_AUTO_TEST_CASE(no_name) {
    BOOST_CHECK((!is_required_name<void>::apply<fake_data<int>>::type::value));
    BOOST_CHECK((!is_required_name<int>::apply<fake_data<int>>::type::value));
}

BOOST_AUTO_TEST_CASE(name_not_match) {
    BOOST_CHECK((!is_required_name<simple_name>::apply<fake_data<named<int>>>::type::value));
}

BOOST_AUTO_TEST_CASE(name_match) {
    BOOST_CHECK((is_required_name<simple_name>::apply<fake_data<named<int, simple_name>>>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<fake_data<named<char, simple_name>>>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<fake_data<const named<int, simple_name>&>>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<fake_data<named<int, simple_name>*>>::type::value));
    BOOST_CHECK((is_required_name<simple_name>::apply<fake_data<named<int, simple_name>*>>::type::value));
}

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

