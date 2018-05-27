#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>
//#include <memory_resource>

#include <endocpp/functor.hpp>
#include <endocpp/impl_std_optional.hpp>
#include <endocpp/impl_std_vector.hpp>

#define AS_LAMBDA(fn)                                                                         \
	[](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }

template<class T>
class TT;

int main(int, char**)
{
	try
	{
		/*std::pmr::unsynchronized_pool_resource rsc;
		std::pmr::polymorphic_allocator pmralloc(&rsc);
		const std::vector<int> inp = {1, 2, 3};
		auto pmrvec = endo::map(vec, [](auto i) { return i * 2; }, pmralloc);

		std::transform(pmrvec.begin(), pmrvec.end(),
					   std::ostream_iterator<std::string>(std::cout, " "),
					   [](const auto& i) { return std::to_string(i); });
		std::cout << std::endl;
		*/

		static_assert(endo::is_functor_v<std::optional<int>>, "");
		static_assert(endo::is_applicative_v<std::optional<int>>, "");
		static_assert(!endo::is_arrow_context_v<std::optional<int>>, "");
		static_assert(endo::is_arrow_context_v<endo::empty_context_t>, "");

		using vec_tag = endo::type_tag<std::vector<int>>;
		using vec_param_pack = endo::type_parameters<std::vector<int>>;
		static_assert(std::is_same<vec_param_pack::template compose_type<vec_tag>,
								   std::vector<int>>::value);

		static_assert(endo::is_type_tag_v<endo::std_vector_tag, int>);

		std::vector<int> vec = {1, 2, 3};
		static_assert(endo::is_functor_v<decltype(vec)>, "");
		static_assert(std::is_same<endo::compose_type<endo::std_vector_tag, int>,
								   std::vector<int>>::value);

		static_assert(std::is_same<endo::compose_type<endo::type_tag<decltype(vec)>, double>,
								   std::vector<double>>::value);

		static_assert(!endo::detail::is_allocator<int>::value, "");
		static_assert(endo::detail::is_allocator<std::allocator<int>>::value, "");

		const auto to_str = AS_LAMBDA(std::to_string);

		// TODO arrow context detection
		// std::allocator<std::string> myalloc;
		// auto vecal = endo::map(myalloc, vec, to_str);

		auto test = vec;
		auto vec2 = endo::map(std::move(vec), to_str);
		auto vec3 = endo::map(std::move(test), [](auto i) { return i; });
		auto vec4 = endo::lift([](auto i) { return i; }, test);
		// endo::applicative_value_t<std::vector<int>> ras = 5;

		std::optional<int> opt = 20;
		auto opt2 = endo::map(opt, to_str);
		std::cout << opt2.value_or("nullopt") << std::endl;

		auto add = [](auto&&... args) { return (... + args); };
		std::optional<int> opt3 = endo::point<endo::std_optional_tag>(2);
		std::optional<int> opt4 = endo::lift(add, opt, opt3, opt);
		std::optional<int> opt5 = endo::lift(add, opt, std::optional<int>(), opt3, opt);
		std::cout << opt4.value_or(-1) << std::endl;
		std::cout << opt5.value_or(-1) << std::endl;

		const auto optf = endo::point<endo::std_optional_tag>(add);
		std::optional<int> opt6 = endo::ap(optf, opt, opt3);
		std::cout << opt6.value_or(-1) << std::endl;

		std::vector<std::optional<int>> vo;
		vo.push_back({});
		vo.push_back(1000);
		vo = endo::map(std::move(vo), [=](auto o) {
			return endo::map(std::move(o), [](auto i) { return i + 2; });
		});

		std::transform(vo.begin(), vo.end(),
					   std::ostream_iterator<std::string>(std::cout, " "),
					   [](const auto& o) { return std::to_string(o.value_or(0)); });
		std::cout << std::endl;

		// endo::map(42, AS_LAMBDA(std::to_string)); // compilation error
		std::copy(vec2.begin(), vec2.end(),
				  std::ostream_iterator<std::string>(std::cout, " "));
		std::cout << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
	return 0;
}
