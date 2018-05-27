#pragma once

#include <optional>

#include <endocpp/applicative.hpp>
#include <endocpp/functor.hpp>

namespace endo {

struct std_optional_tag
{
	template<class T>
	using compose = std::optional<T>;
};
// TODO detail
struct std_optional_impl;

template<class T>
struct higher_order_type_traits<std::optional<T>>
{
	using tag = std_optional_tag;
	using parameters = parameter_pack<T>;
};

// TODO detail
template<class T>
struct std_optional_traits
{
	using implementation = std_optional_impl;
	using value_type = typename std::optional<T>::value_type;
};

template<class T>
struct pointed_traits<std::optional<T>> : std_optional_traits<T>
{
};
template<class T>
struct functor_traits<std::optional<T>> : std_optional_traits<T>
{
};
template<class T>
struct applicative_traits<std::optional<T>> : std_optional_traits<T>
{
};

struct std_optional_impl
{
	// Applicative
	template<class Function, class... Args>
	static constexpr auto lift(empty_context_t, Function&& f, Args&&... args)
	{
		using R = decltype(std::forward<Function>(f)(*std::forward<Args>(args)...));

		return (... && args) ? std::optional<std::decay_t<R>>(
								   std::forward<Function>(f)(*std::forward<Args>(args)...))
							 : std::nullopt;
	}
	template<class Function, class... Args>
	static constexpr auto lift_impl(Function&& f, Args&&... args) // TODO
	{
		using R = decltype(std::forward<Function>(f)(*std::forward<Args>(args)...));

		return (... && args) ? std::optional<std::decay_t<R>>(
								   std::forward<Function>(f)(*std::forward<Args>(args)...))
							 : std::nullopt;
	}

	// Pointed
	template<class T>
	static constexpr auto point(empty_context_t, T&& arg)
	{
		return std::optional<std::decay_t<T>>(std::forward<T>(arg));
	}
	// Pointed
	template<class Function>
	static constexpr auto point_eval(empty_context_t, Function&& f)
	{
		return std::optional<detail::decay_result_t<Function>>(std::forward<Function>(f)());
	}

	// Applicative
	template<class OptFunction, class... Args>
	static constexpr auto ap(empty_context_t, OptFunction&& f, Args&&... args)
	{
		return f ? lift_impl(*std::forward<OptFunction>(f), std::forward<Args>(args)...)
				 : std::nullopt;
	}

	// Functor
	template<class Opt, class Function>
	static constexpr auto map(empty_context_t, Opt&& opt, Function&& f)
	{
		return lift_impl(std::forward<Function>(f), std::forward<Opt>(opt));
	}
};

} // namespace endo
