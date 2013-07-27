//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include <map>
    #include <typeinfo>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/any.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/equal_to.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/insert.hpp>

    #include "boost/di/type_traits/has_traits.hpp"
    #include "boost/di/type_traits/value_type.hpp"
    #include "boost/di/detail/dependency.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"
    #include "boost/di/scopes/singleton.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/detail/module.hpp"    \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace detail {

    namespace aux {

    template<typename T>
    struct dependency_impl
        : dependency<
            scopes::singleton<>
          , typename T::element_type
          , typename type_traits::value_type<typename T::element_type>::type
          , typename T::context
        >
    { };

    } // namespace aux

    template<
        typename TDeps = mpl::vector0<>
      , typename TExternals = TDeps
      , template<
            typename
          , typename
        > class TBinder = binder
      , template<
            typename
          , typename = void
        > class TPool = pool
      , template<
            typename
        > class TCreator = creator
      , template<
            typename
        > class TVisitor = visitor
      , template<
            typename
        > class TDependency = aux::dependency_impl
    >
    class module
    {
        template<
            typename
          , typename
          , template<typename, typename> class
          , template<typename, typename> class
          , template<typename> class
          , template<typename> class
          , template<typename> class
        > friend class module;

        class type_info_cmp
        {
        public:
            bool operator ()(const std::type_info* a, const std::type_info* b) const {
                return a->before(*b);
            }
        };

        typedef std::map<const std::type_info*, boost::any, type_info_cmp> type_info_deps_t;

        template<typename TSeq>
        struct is_module
            : mpl::equal_to<
                  mpl::size<TSeq>
                , typename mpl::fold<
                      TSeq
                    , mpl::int_<0>
                    , mpl::if_<
                          type_traits::has_deps<mpl::_2>
                        , mpl::next<mpl::_1>
                        , mpl::_1
                      >
                  >::type
              >
        { };

        template<
            typename T
          , typename = void
        >
        struct deps_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct deps_impl<
            T
          , typename enable_if<
                type_traits::has_deps<T>
            >::type
        >
        {
            typedef typename T::deps type;
        };

        template<
            typename T
          , typename = void
        >
        struct externals_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct externals_impl<
            T
          , typename enable_if<
                type_traits::has_externals<T>
            >::type
        >
        {
            typedef mpl::vector<typename T::externals> type;
        };

    public:
        struct policies
            : mpl::fold<
                  TDeps
                , mpl::vector0<>
                , mpl::if_<
                      type_traits::has_policy_type<mpl::_2>
                    , mpl::push_back<mpl::_1, mpl::_2>
                    , mpl::_1
                  >
              >::type
        { };

        typedef TPool<
            typename mpl::fold<
                TExternals
              , mpl::vector0<>
              , mpl::copy<
                    externals_impl<mpl::_2>
                  , mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        > externals;

        struct deps
            : mpl::remove_if<
                  typename mpl::fold<
                      typename mpl::fold<
                          typename mpl::fold<
                              TDeps
                            , typename mpl::fold<
                                  typename externals::types
                                , mpl::vector0<>
                                , mpl::if_<
                                      type_traits::has_element_type<mpl::_2>
                                    , mpl::push_back<
                                          mpl::_1
                                        , TDependency<mpl::_2>
                                      >
                                    , mpl::_1
                                  >
                              >::type
                            , mpl::copy<
                                  deps_impl<mpl::_2>
                                , mpl::back_inserter<boost::mpl::_1>
                              >
                          >::type
                        , mpl::set0<>
                        , mpl::insert<mpl::_1, mpl::_2>
                      >::type
                    , mpl::vector0<>
                    , mpl::push_back<mpl::_1, mpl::_2>
                  >::type
                , mpl::or_<
                      type_traits::has_policy_type<mpl::_1>
                    , type_traits::has_element_type<mpl::_1>
                  >
              >::type
        { };

    private:
        template<typename T, typename TPolicy>
        struct verify_policies_impl
            : TPolicy::template verify<deps, TExternals, T>::type
        { };

        template<typename T>
        struct verify_policies
            : mpl::transform<
                  policies
                , verify_policies_impl<T, mpl::_1>
              >::type
        { };

        typedef TBinder<deps, TExternals> binder_type;

    public:
        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            typedef typename verify_policies<T>::type policies_type;

            return TCreator<binder_type>::template
                execute<T, mpl::vector0<> >(deps_, externals_, type_info_deps_);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            typedef typename verify_policies<T>::type policies_type;

            TVisitor<binder_type>::template
                execute<T, mpl::vector0<> >(visitor);
        }

        template<typename Scope, typename Action>
        void call(const Action& action) {
            call_impl<Scope, deps>(action);
        }

    private:
        template<
            typename Scope
          , typename Deps
          , typename Action
        >
        typename enable_if<mpl::empty<Deps> >::type
        call_impl(const Action&) { }

        template<
            typename Scope
          , typename Deps
          , typename Action
        >
        typename enable_if<
            mpl::and_<
                mpl::not_<mpl::empty<Deps> >
              , is_same<typename mpl::front<Deps>::type::scope, Scope>
            >
        >::type
        call_impl(const Action& action) {
            typedef typename mpl::front<Deps>::type type;
            static_cast<type&>(deps_).call(action);
            call_impl<Scope, typename mpl::pop_front<Deps>::type>(action);
        }

        externals externals_;
        TPool<deps> deps_;
        type_info_deps_t type_info_deps_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename enable_if<
            is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >
        >::type* = 0)
        : externals_(BOOST_PP_ENUM_BINARY_PARAMS(
              BOOST_PP_ITERATION()
            , args
            , .externals_ BOOST_PP_INTERCEPT
          ))
    { }

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename disable_if<
            is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> >
        >::type* = 0)
        : externals_(BOOST_DI_ARGS_FORWARD(args))
    { }

#endif
