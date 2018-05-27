#pragma once

#include <algorithm>
#include <vector>

#include <endocpp/applicative.hpp>
#include <endocpp/functor.hpp>

namespace endo {

struct std_vector_tag
{
	template<class... Args>
	using compose = std::vector<Args...>;
};
// TOOD detail
struct std_vector_impl;

template<class T, class Alloc>
struct higher_order_type_traits<std::vector<T, Alloc>>
{
	using tag = std_vector_tag;
	using parameters = parameter_pack<T, Alloc>;
};

// TODO detail
template<class T, class Alloc>
struct std_vector_traits
{
	using implementation = std_vector_impl;
	using value_type = typename std::vector<T, Alloc>::value_type;
};

template<class T, class Alloc>
struct functor_traits<std::vector<T, Alloc>> : std_vector_traits<T, Alloc>
{
};
template<class T, class Alloc>
struct pointed_traits<std::vector<T, Alloc>> : std_vector_traits<T, Alloc>
{
};
template<class T, class Alloc>
struct applicative_traits<std::vector<T, Alloc>> : std_vector_traits<T, Alloc>
{
};

struct std_vector_impl
{
	// Functor
	template<class T, class Alloc, class Function>
	static auto map(empty_context_t, const std::vector<T, Alloc>& v, Function f)
	{
		using U = detail::decay_result_t<Function, T>;
		std::cout << "const ref" << std::endl;
		std::vector<U> out;
		out.reserve(v.size());
		for (const auto& x : v)
		{
			out.push_back(f(x));
		}
		return out;
	}
	// Functor
	template<class Allocator, class T, class Alloc, class Function>
	static auto map(Allocator alloc, const std::vector<T, Alloc>& v, Function f)
	{
		using U = detail::decay_result_t<Function, T>;
		static_assert(detail::is_allocator<Allocator>::value, "requires an allocator");
		static_assert(std::is_same<detail::alloc_value_t<Allocator>, U>::value,
					  "requires an allocator with value type U");

		std::cout << "const ref custom alloc" << std::endl;
		std::vector<U, Allocator> out(alloc);
		out.reserve(v.size());
		for (const auto& x : v)
		{
			out.push_back(f(x));
		}
		return out;
	}

	/*template<
		class T, class Alloc, class Function, class ArrowContext,
		std::enable_if_t<
			!detail::is_same_assignable<Function, std::add_rvalue_reference_t<T>>::value, int>
	= 0> static auto map(std::vector<T, Alloc>&& v, Function f, ArrowContext)
	{
		using U = detail::decay_result_t<Function, T>;
		std::cout << "Not Same" << std::endl;
		std::vector<U> out;
		out.reserve(v.size());
		for (auto&& x : v)
		{
			out.push_back(f(std::move(x)));
		}
		return out;
	}

	template<
		class T, class Alloc, class Function, class ArrowContext,
		std::enable_if_t<
			detail::is_same_assignable<Function, std::add_rvalue_reference_t<T>>::value, int> =
	0> static auto map(std::vector<T, Alloc>&& v, Function f, ArrowContext)
	{
		std::cout << "Same" << std::endl;
		for (auto&& x : v)
		{
			x = f(std::move(x)); // requires assignable
		}
		return v;
	}*/

	// Applicative
	template<class ArrowContext, class Function, class T, class Alloc>
	static auto lift(ArrowContext&&, Function f, const std::vector<T, Alloc>& v)
	{

		std::cout << "const ref" << std::endl;
		std::vector<std::decay_t<std::result_of_t<Function(T)>>> out;
		out.reserve(v.size());
		for (const auto& x : v)
		{
			out.push_back(f(x));
		}
		return out;
	}

	// Pointed
	template<class T>
	static auto point(empty_context_t, T&& arg)
	{
		std::vector<std::decay_t<T>> v;
		v.push_back(std::forward<T>(arg));
		return v;
	}
	// Pointed
	template<class Function>
	static auto point_eval(empty_context_t, Function&& f)
	{
		std::vector<detail::decay_result_t<Function>> v;
		v.push_back(std::forward<Function>(f)());
		return v;
	}
};

} // namespace endo
