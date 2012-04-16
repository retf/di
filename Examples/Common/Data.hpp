//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_EXAMPLES_COMMON_DATA_HPP
#define QDEPS_EXAMPLES_COMMON_DATA_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/string.hpp>
#include <QDeps/QDeps.hpp>

struct If0
{
    virtual ~If0() { }
    virtual void dummy() = 0;
};

struct CIf0 : If0
{
    virtual void dummy() { }
};

struct CIf01 : If0
{
    virtual void dummy() { }
};

struct CIf02 : If0
{
    virtual void dummy() { }
};

struct CIf03 : If0
{
    QDEPS_CTOR(CIf03, int i, double d)
        : i(i), d(d)
    { }

    virtual void dummy() { }

    int i;
    double d;
};

struct C0
{
    //trivial ctor
};

struct C1
{
    explicit C1(int = 0) { }
};

struct C2
{
    QDEPS_CTOR(C2, int i, double d, char c)
        : i(i), d(d), c(c)
    { }

    int i;
    double d;
    char c;
};

struct C3
{
    QDEPS_CTOR(explicit C3, int i = 0)
        : i(i)
    { }

    int i;
};

struct C4
{
    QDEPS_CTOR(C4, boost::shared_ptr<C3> c3, QDeps::Utility::Named<int, boost::mpl::string<'1'> > i1, QDeps::Utility::Named<int, boost::mpl::string<'2'> > i2)
        : c3(c3), i1(i1), i2(i2)
    { }

    boost::shared_ptr<C3> c3;
    int i1;
    int i2;
};

struct C5
{
    QDEPS_CTOR(C5, boost::shared_ptr<If0> if0, boost::shared_ptr<C2> c2, boost::shared_ptr<C1> c1)
        : if0(if0), c2(c2), c1(c1)
    { }

    boost::shared_ptr<If0> if0;
    boost::shared_ptr<C2> c2;
    boost::shared_ptr<C1> c1;
};

struct C6
{
    QDEPS_CTOR(C6, boost::shared_ptr<C3> c3, const boost::shared_ptr<C4>& c4, C5 c5)
        : c3(c3), c4(c4), c5(c5)
    { }

    boost::shared_ptr<C3> c3;
    boost::shared_ptr<C4> c4;
    C5 c5;
};

struct C7
{
    QDEPS_CTOR(C7, boost::shared_ptr<If0> if0, boost::shared_ptr<C6> c6)
        : if0(if0), c6(c6)
    { }

    boost::shared_ptr<If0> if0;
    boost::shared_ptr<C6> c6;
};

struct C8
{
    QDEPS_CTOR(C8, boost::shared_ptr<C7> c7, C0 c0, boost::shared_ptr<C1> c1, int i)
        : c7(c7), c0(c0), c1(c1), i(i)
    { }

    boost::shared_ptr<C7> c7;
    C0 c0;
    boost::shared_ptr<C1> c1;
    int i;
};

struct Transaction
{
    Transaction(int i)
        : i(i)
    { }

    int i;
};

struct TransactionProvider : QDeps::Utility::Provider< boost::shared_ptr<Transaction> >
{
    QDEPS_CTOR(TransactionProvider, boost::shared_ptr<C3> c3)
        : c3(c3)
    { }

    virtual boost::shared_ptr<Transaction> get() const
    {
        return boost::make_shared<Transaction>(c3->i);
    }

    boost::shared_ptr<C3> c3;
};

struct TransactionUsage
{
    QDEPS_CTOR(TransactionUsage, boost::shared_ptr< QDeps::Utility::Provider< boost::shared_ptr<Transaction> > > p)
        : p(p)
    { }

    boost::shared_ptr< QDeps::Utility::Provider< boost::shared_ptr<Transaction> > > p;
};


#endif
