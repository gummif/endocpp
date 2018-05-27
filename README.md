Work In Progress

# `endocpp`

## Introduction

This is a header-only C++17 library of functional programming concepts. It provides common functional programming 'concepts' (or 'type classes') (e.g. functor, monad, monoid) and implementations for native and standard library types. Its aims are to provide generic, zero-overhead abstractions for functional programming in (idiomatic) C++, taking full advantage of perfect forwarding, move-semantics, and being allocator and executor aware.

Although the library has `constexpr` support, its main focus is on types and concepts and runtime computations. It is not a meta-programming library (like Boost.Hana). It is not for working with lazy heterogeneous containers (like Boost.Fusion). It does not provide generic higher-order functions and utilities (like Boost.HOF).

## Design Goals

* **Zero-overhead abstractions.** Using the library should give performance equal to writing the code manually, with no virtual functions and with `constexpr` support everywhere.
* **Customizable.** The library should provide mechanism to customize all reasonable functions and algorithms. The user should be able to execute algorithms in parallel, use custom allocators, specify executors, etc.
* **Extensible.** The library should provide utilities for and be able to serve as basis for other higher level libraries. Users should be able to implement concepts for their own types (or third-party or standard library types) without a lot of ceremony.
* **Ease of use.** Users familiar with functional programming concepts should be able to use the library immediately without any (unpleasant) surprise.

## Rationale

The concepts in this library require the types to specialization a traits class, and provide a type alias to an implementation class with static functions. This technique allows us to
* avoid argument-dependent loopup (ADL),
* avoid virtual functions and inheritance,
* fully support `constexpr` functions,
* and implement concepts for native and third-party types.

A critical design decision was to add an optional `ArrowContext` parameters to all functions. Given that the audience is C++ programmers, it is obvious that there needs be some kind of customization mechanism. The concept implementation for each type is allowed to specify the kinds of `ArrowContext`s and the effects they have for each function.

## API Reference

### Concept Overview

Concept functions, types, aliases and objects.
`HigherOrderType`: `compose_type`
`Pointed`: `point`, `point_eval`
`Functor`: `map`
`Applicative`: `lift`, `ap` (should `lift` and `ap` be in separate concepts, to support types that can not contain function objects?)

Next up:
`Semigroup`
`Monoid`
`Group` (with constexpr property `is_commutative`?)
Co concepts

### Functor
A type is an instance of Functor if it can be mapped. It is assumed that the Functor laws hold `map(x, id) == x` and `map(x, compose(f, g)) == map(map(x, g), f)` for all `x`.
* `map(Functor, Function) -> Functor`

Owning 'computational context'-like types are Functors:
* std::optional
* std::variant
* std::function
* std::future
* std::shared_future
* std::unique_ptr
* std::shared_ptr
* std::vector
* std::list
* std::forward_list
* std::array? (not monad)
* std::dequeue?
* std::stack?
* std::queue?
* std::map?

Range transforming views (possibly C++20) could be Functors. Non-owning pointers, references and special purpose classes are not Functors:
* std::reference_wrapper
* std::weak_ptr
* std::atomic
* std::promise
* std::span (C++20)
* std::observer_ptr (possibly C++20)
