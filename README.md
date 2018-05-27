Work In Progress

TODO rename Endo

# `loa`

This is a header-only C++17 library of functional programming abstractions. It contains higher-order utility functions and common functional programming 'categories' (e.g. functor, monad, monoid). Its aims are to provide generic, zero-overhead abstractions for functional programming in (idiomatic) C++, taking full advantage of perfect forwarding and move-semantics.
Although the library has `constexpr` support, its main focus is on runtime computations, not meta-programming (like Boost.Hana) or working with lazy heterogeneous containers (like Boost.Fusion). In Icelandic folklore the first appearance of the bird lóa (Pluvialis apricaria) signifies the arrival of spring.

# Quick Guide: Utility Functions
Let `A,B,C` denote any types and `Function` be the concept of a function, the following are function objects:

| Signature                              | Description |
|----------------------------------------|-------------|
| `loa::id(A) -> A`                      | Identity function |
| `loa::compose(C(B), B(...)) -> C(...)` | Function composition |
| `loa::flip(C(A,B)) -> C(B,A)`          | Flip function arguments |
| `loa::constant(A) -> A(...)`           | Function returning a single value |
| `loa::curry(A(...)) -> Function`       | Function currying |
| `loa::rcurry(A(...)) -> Function`      | Function currying, reverse order |
| `loa::on(B(A), C(B,B)) -> C(A,A)`      | Compose an unary function and a binary function (std::less<> by default) |
| `loa::fst(A) -> B`      | Tuple/pair first (via std::get) |
| `loa::snd(A) -> B`      | Tuple/pair second |
| `loa::thrd(A) -> B`      | Tuple third |

# Categories
For compilers supporting C++ concepts (gcc with `-std=c++1z -fconcepts`) the following are defined as concepts.

## Lazy
A type is an instance of Lazy if it can be converted to a lazy view type and back. It is assume that `compose(strict, lazy) == id` holds. A lazy view is non-owning. (Deep lazy?)
* `lazy(Lazy) -> Lazy`
* `strict(Lazy) -> Lazy`

## Functor
A type is an instance of Functor if it can be mapped. It is assumed that the Functor laws hold `map(x, id) == x` and `map(x, compose(f, g)) == map(map(x, g), f)` for all `x`.
* `map(Functor, Function) -> Functor`

# Examples
Composition and flipping arguments.
```c++
const auto tostr = [](auto x) { return std::to_string(x) + "!"; };
const auto plusstr = loa::compose(tostr, std::plus<>{});
std::cout << plusstr(40, 2) << '\n';
std::cout << loa::flip(std::plus<>{})("foo"s, "bar"s) << '\n';
// output:
// 42!
// barfoo
```
Reverse currying.
```c++
const auto mod5 = loa::rcurry(std::modulus<>{})(5);
std::cout << mod5(7) << '\n';
// output:
// 2
```
Map a Functor with lazy evaluation to avoid temporary vector construction.
```c++
std::vector<std::vector<std::unique_ptr<int>>> vvus(1);
vvus[0].push_back(std::make_unique<int>(77));
vvus[0].push_back(std::make_unique<int>(79));
const auto mapplus1 = loa::rcurry(loa::map)([](const auto& x) { return *x + 1; });
for (const auto& tv : loa::map(loa::lazy(vvus), loa::compose(mapplus1, loa::lazy)))
{
    for (const auto& x : tv)
        std::cout << x << ',';
}
// output:
// 78,80,
```
