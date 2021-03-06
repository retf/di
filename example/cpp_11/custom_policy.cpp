//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_policy_cpp_11
//````C++11```
//<-
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/has_xxx.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct example {
    example(std::shared_ptr<int>, std::unique_ptr<double>, boost::shared_ptr<char>) { }
};

/*<define policy class>*/
class allow_only_smart_ptr_policy {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

    template<typename TDependency>
    struct is_creation_by_smart_ptr
        : has_element_type<
              typename boost::remove_reference<typename TDependency::type>::type
          >
    { };

public:
    /*<<define `assert_policy` requirement - version when type is smart pointer>>*/
    template<typename TDependency>
    typename boost::enable_if<is_creation_by_smart_ptr<TDependency>>::type
    assert_policy() const { }

    /*<<define `assert_policy` requirement - version when type is not smart pointer>>*/
    template<typename TDependency>
    typename boost::disable_if<is_creation_by_smart_ptr<TDependency>>::type
    assert_policy() const {
        BOOST_MPL_ASSERT_MSG(
            false
          , CREATION_NOT_BY_SMART_PTR_IS_DISALLOWED
          , (TDependency)
        );
    }
};

int main() {
    /*<<create shared_ptr `example` with `allow_only_smart_ptr_policy` policy>>*/
    di::make_injector().create<std::shared_ptr<example>>(allow_only_smart_ptr_policy());

    /*<<create lvalue `example` with `allow_only_smart_ptr_policy` policy - compile error>>*/
    //di::make_injector().create<example>(allow_only_smart_ptr_policy());

    return 0;
}

//]

