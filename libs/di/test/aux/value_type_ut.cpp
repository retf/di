//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/is_same.hpp>
#include "boost/di/named.hpp"
#include "boost/di/aux/value_type.hpp"
#include "data.hpp"

namespace boost {
namespace di {
namespace aux {

struct c
{
    typedef int value_type;
};

BOOST_AUTO_TEST_CASE(value_type_basic)
{
    BOOST_CHECK((is_same<int, typename value_type<int>::type>::value));
    BOOST_CHECK((is_same<int, typename value_type<shared_ptr<int> >::type>::value));
    BOOST_CHECK((is_same<int, typename value_type<named<int> >::type>::value));
    BOOST_CHECK((is_same<int, typename value_type<c>::type>::value));
    BOOST_CHECK((is_same<std::string, typename value_type<std::string>::type>::value));
}

} // namespace aux
} // namespace di
} // namespace boost
