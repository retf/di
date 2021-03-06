//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_allocator_cpp_11
//````C++11```
//<-
#include <cassert>
#include <memory>
#include <utility>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() { }; };
struct implementation : interface { };
//->

/*<define `example` class as usual>*/
struct example {
    explicit example(long l, std::unique_ptr<interface> up)
    {
        assert(l == 42l);
        assert(dynamic_cast<implementation*>(up.get()));
    }
};

/*<define `custom allocator` with allocate emethod>*/
class custom_allocator {
public:
    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* allocate(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }

    /*<to support `bind_int<interface>()`, `bind_string<"s">()` allocate needs support types with `value` or when `mpl::string`>*/
};

int main() {
    /*<<make injector with simple configuration>>*/
    auto injector = di::make_injector(
        di::bind<long>::to(42l)
      , di::bind<interface, implementation>()
    );

    /*<<create `example` using `custom_allocator`>>*/
    injector.allocate<example>(custom_allocator());
}

//]

