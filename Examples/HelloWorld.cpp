//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <QDeps/QDeps.hpp>

class C1 { };
class C2 { };

class LValue
{
public:
    QDEPS_CTOR(LValue, C1, C2)
    { }
};

class SharedPtr
{
public:
    QDEPS_CTOR(SharedPtr, boost::shared_ptr<C1>, boost::shared_ptr<C2>)
    { }
};

struct Module : QDeps::Front::Base::Module<> { };

int main()
{
    QDeps::Utility::Injector<Module> injector;
    injector.create<LValue>();
    injector.create<SharedPtr>();

    return 0;
}
