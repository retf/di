//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_TRAITS_HPP

#include <boost/mpl/has_xxx.hpp>

#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(BOOST_DI_CTOR_UNIQUE_NAME)
BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(policy_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(externals)
BOOST_MPL_HAS_XXX_TRAIT_DEF(context)
BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)
BOOST_MPL_HAS_XXX_TRAIT_DEF(pool)
BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

} // namespace type_traits
} // namespace di
} // namespace boost

#endif
