#pragma once

#include <endocpp/detail/detection.hpp>
#include <endocpp/arrow_context.hpp>
#include <endocpp/higher_order_type.hpp>

namespace endo {

/// Functor specialization point
template<class T>
struct functor_traits;

namespace detail {
template<class T>
using functor_impl = typename functor_traits<std::decay_t<T>>::implementation;
}

/// Functor T value type
template<class T>
using functor_value_t = typename functor_traits<std::decay_t<T>>::value_type;

/// Detect if T is a Functor
template<class T>
using is_functor = detail::is_detected<detail::functor_impl, T>;

/// Detect if T is a Functor
template<class T>
inline constexpr bool is_functor_v = is_functor<T>::value;

/// Functor: map
template<class ArrowContext, class Functor, class Function>
constexpr decltype(auto) map(ArrowContext&& ctx, Functor&& ftor, Function&& f)
{
	static_assert(is_functor_v<Functor>, "map requires a Functor");
	return detail::functor_impl<Functor>::map(std::forward<ArrowContext>(ctx),
											  std::forward<Functor>(ftor),
											  std::forward<Function>(f));
};

/// Functor: map
template<class Functor, class Function>
constexpr decltype(auto) map(Functor&& ftor, Function&& f)
{
	static_assert(is_functor_v<Functor>, "map requires a Functor");
	return ::endo::map(empty_context, std::forward<Functor>(ftor), std::forward<Function>(f));
};

} // namespace endo
