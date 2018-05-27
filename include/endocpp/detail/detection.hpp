#pragma once

#include <type_traits>

namespace endo::detail {

// Implementation of the detection idiom, until we can use C++20
// http://en.cppreference.com/w/cpp/experimental/is_detected

struct nonesuch
{
	nonesuch() = delete;
	~nonesuch() = delete;
	nonesuch(nonesuch const&) = delete;
	void operator=(nonesuch const&) = delete;
};

template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
struct detector
{
	using value_t = std::false_type;
	using type = Default;
};

template<class Default, template<class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
{
	using value_t = std::true_type;
	using type = Op<Args...>;
};

template<template<class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template<template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template<class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

template<class A>
using alloc_value_t = typename std::allocator_traits<A>::value_type;
template<class A>
using allocate_t = decltype(std::declval<A>().allocate(1));

template<class A>
using is_allocator = detail::is_detected<allocate_t, A>;
template<class T, class A>
using is_allocator_of = std::conjunction<is_allocator<A>, std::is_same<alloc_value_t<A>, T>>;

//template<class T, class... Args>
//using first_type = T;

template<class F, class... Args>
using decay_result_t = std::decay_t<std::invoke_result_t<F, Args...>>;

template<class Function, class T>
using same_result = std::is_same<decay_result_t<Function, T>, T>;

template<class Function, class T>
using is_same_assignable =
	std::conjunction<same_result<Function, T>,
					 std::is_assignable<T, detail::decay_result_t<Function, T>>>;

} // namespace endo::detail
