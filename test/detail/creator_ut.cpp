//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/detail/creator.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/is_same.hpp>

#include "fake_binder.hpp"

namespace boost {
namespace di {
namespace detail {

template<typename T, T value>
struct fake_dependency
{
    typedef fake_dependency type;
    typedef T given;
    typedef T expected;
    typedef mpl::vector0<> context;
    typedef void scope;
    typedef is_same<mpl::_1, T> bind;
    typedef T result_type;

    T create() {
        return value;
    }
};

template<typename T>
struct entries
    : T
{ };

BOOST_AUTO_TEST_CASE(creator_simple) {
    const int i = 42;

    typedef fake_dependency<int, i> dependency_type;
    entries<dependency_type> entries_;

    BOOST_CHECK_EQUAL(i, (
        creator<
            fake_binder<dependency_type>
        >::execute<int, mpl::vector0<>>(entries_)
    ));
}

} // namespace detail
} // namespace di
} // namespace boost
