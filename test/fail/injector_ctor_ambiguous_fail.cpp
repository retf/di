//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct ctor
{
    BOOST_DI_INJECT(ctor, int, double) { }
    BOOST_DI_INJECT(ctor, double, int) { }
};

BOOST_AUTO_TEST_CASE(ctor_ambiguous_fail) {
    injector<>().create<ctor>();
}

} // namespace di
} // namespace boost

