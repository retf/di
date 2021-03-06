# C++ Dependency Injection
> "Don't call us, we'll call you", Hollywood principle

## Disclaimers
This is not an official Boost library yet and there is no guarantee it will ever be!

## Introduction
Boost.DI is C++03/C++11/C++14 header only library providing type safe, compile time, macro free constructor dependency injection.

### Hello World

```cpp
#include <boost/di.hpp>

namespace di = boost::di;

struct hello {
    hello(const std::shared_ptr<i>& sp
        , std::unique_ptr<int> up
        , boost::function<int()> f)
        : sp(sp)
    {
        assert(dynamic_cast<impl*>(sp.get()));
        assert(*up == 0);
        assert(f.empty());
    }

    std::shared_ptr<int> sp;
};

struct world {
    world(hello copy
        , boost::shared_ptr<i> sp
        , const std::string& str
        , int i)
    {
        assert(dynamic_cast<impl*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(str == "");
        assert(i == 0);
    }
};

struct app {
    app(hello, world);
    int run();
};

int main() {
    auto injector = di::make_injector(
        di::bind<i, impl>() // scope deduction -> di::shared<di::bind<i, impl>>
    );

    return injector.create<app>().run();
}
```

## Key features
* Type safe
* Compile time approach - no exceptions - if application compiles all dependencies will be be created accurately
* Macro free - by default no need to specify constructor traits or register anything (less intrusive)
* Scopes deduction - scopes are deduced based on type semantic
* Automatic conversion between std/boost smart pointers
* Compile time policies - ex. to detect circular dependencies or limit supported types only to specified
* Supports C++03/C++11/C++14 standards

## Other features
* Header only library
* Architecture independent (tested on x86/x86-64)
* Supports all POSIX and Windows operating systems (tested on Linux 3.8/Windows 7/8)
* Exception safe guaranty
* Thread safe guaranty
* Constructor injection (macro free)
* Compile time creation guaranty
* Dependencies life time management (scopes: deduce, external, unique, shared, session + custom scopes)
* Scope deduction (shared\_ptr -> shared, unique\_ptr, lvalue -> unique, ...)
* Supports copies, references, pointers, boost and std smart pointers / rvalue references(C++11)
* Named parameters (named\<int, my\_int\>)
* Runtime visitor throughout created objects (useful for generation dependency diagrams)

## More information
* [Documentation](http://krzysztof-jusiak.github.io/di/boost/libs/di/doc/html)
* [Todo/Issues] (https://github.com/krzysztof-jusiak/di/issues?state=open)
* [Boost Library Incubator - DI](http://rrsd.com/blincubator.com/bi_library/di-dependency-injection-2)

## Tested compilers (100% test code coverage)
* Linux (x86/x86-64)
   * Clang 3.2/3.3/3.4.2+ (clean with Clang Static Analyzer and Valgrind)
   * GCC 4.7.3/4.8.2/4.9.0+ (clean with Valgrind)
   * Intel C++ 14.0.3+ (clean with Valgrind)

* Windows (x86/x86-64)
   * MinGW 4.7.3/4.8.2+
   * Visual Studio 2013+ (clean with DrMemory)

* Darwin/Mac OS (x86-64)
   * Clang 503.0.40+ (Apple LLVM version 5.1)

## Continuous integration
[![Build Status](https://travis-ci.org/krzysztof-jusiak/di.png?branch=master)](https://travis-ci.org/krzysztof-jusiak/di)
[![Build Status](https://ci.appveyor.com/api/projects/status/1il4knxh7tq9o5ic)](https://ci.appveyor.com/project/krzysztof-jusiak/di)
[![Coverage Status](https://coveralls.io/repos/krzysztof-jusiak/di/badge.png?branch=master)](https://coveralls.io/r/krzysztof-jusiak/di?branch=master)

## Other Dependency Injection Frameworks for C++:
 * [https://bitbucket.org/cheez/dicpp](https://bitbucket.org/cheez/dicpp)
 * [https://code.google.com/p/infectorpp](https://code.google.com/p/infectorpp)
 * [https://github.com/phs/sauce](https://github.com/phs/sauce)
 * [https://code.google.com/p/wallaroo](https://code.google.com/p/wallaroo)
 * [https://code.google.com/p/hypodermic](https://code.google.com/p/hypodermic)
 * [https://github.com/admiyo/CppInject](https://github.com/admiyo/CppInject)
 * [http://qtioccontainer.sourceforge.net](http://qtioccontainer.sourceforge.net)
 * [https://code.google.com/p/autumnframework](https://code.google.com/p/autumnframework)
 * [http://sourceforge.net/projects/cpp-resolver](http://sourceforge.net/projects/cpp-resolver)
 * [https://code.google.com/p/pococapsule](https://code.google.com/p/pococapsule)
 * [https://code.google.com/p/spring-cpp](https://code.google.com/p/spring-cpp)
 * [http://www.cs.utah.edu/~arichard/inject.html](http://www.cs.utah.edu/~arichard/inject.html)
 * [http://bobah.net/d4d/source-code/misc/cpp-dependency-injection](http://bobah.net/d4d/source-code/misc/cpp-dependency-injection)
 * [https://code.google.com/p/ffead-cpp](https://code.google.com/p/ffead-cpp)
 * [http://codebros.github.io/DiLite](http://codebros.github.io/DiLite)
 * [https://code.google.com/p/kindi](https://code.google.com/p/kindi)

## License
Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

