# Concepts and Generic Programming

Together with concepts, the template is C++’s main support for generic programming. Templates provide (compile-time) parametric polymorphism.

## Concepts

Consider the `sum()`:

```c++
template<typename Seq, typename Num>
Num sum(Seq s, Num v)
{
    for (const auto& x : s )
        v += x;
    return v;
}
```

It can be invoked for any data structure that supports `begin()` and `end()` so that the range-for will work. [...]
Furthermore, the element type of the data structure is limited only by its use: it must be a type that we can add to the `Num` argument. [...]
We could say that the `sum()` algorithm is generic in two dimensions: the type of the data structure used to store elements (“the sequence”) and the type of elements.

So, `sum()` requires that its first template argument is some kind of sequence and its second template argument is some kind of number. We call such requirements *concepts*.

```c++
template<Sequence Seq, Number Num>
Num sum(Seq s, Num v)
{
     for (const auto& x : s)
           v += x;
     return v;
}
```

That’s much clearer. Once we have defined what the concepts `Sequence` and `Number` mean, the compiler can reject bad calls by looking at `sum()`’s interface only, rather than looking at its implementation. This improves error reporting.

However, the specification of `sum()`’s interface is not complete: I “forgot” to say that we should be able to add elements of a `Sequence` to a `Number`. We can do that:

```c++
template<Sequence Seq, Number Num>
    requires Arithmetic<Value_type<Seq>,Num>
Num sum(Seq s, Num n);
```

The `Value_type` of a sequence is the type of the elements in the sequence. `Arithmetic<X,Y>` is a concept specifying that we can do arithmetic with numbers of types `X` and `Y`.

Unsurprisingly, `requires Arithmetic<Value_type<Seq>,Num>` is called a requirements-clause.

The `template<Sequence Seq>` notation is simply a shorthand for an explicit use of `requires Sequence<Seq>`. If I liked verbosity, I could equivalently have written

```c++
template<typename Seq, typename Num> requires 
    Sequence<Seq> && Number<Num> && 
    Arithmetic<Value_type<Seq>,Num>
Num sum(Seq s, Num n);
```

On the other hand, we could also use the equivalence between the two notations to write:

```c++
template<Sequence Seq, Arithmetic<Value_type<Seq>> Num>
Num sum(Seq s, Num n);
```

## Variadic Templates

A template can be defined to accept an arbitrary number of arguments of arbitrary types. Such a template is called a *variadic template*.

```c++
template<typename... T>
int sum_variadic_fold(T... v)
{
    return (v + ... + 0);
}
```

The `typename... T` indicates that `T` is a sequence of types. The `T... v` indicates that `v` is a sequence of values of the types in `T`. A parameter declared with a `...` is called a *parameter pack*. Here, `v` is a (function argument) parameter pack where the elements are of the types found in the (template argument) parameter pack `T`.

To simplify the implementation of simple variadic templates, C++17 offers a limited form of iteration over elements of a parameter pack.

Here, `(v+...+0)` means add all the elements of `v` starting with the initial value 0. The first element to be added is the one with the highest index (“rightmost”). It is called a *right fold*.

```c++
(v+...+0)
(v[0]+(v[1]+(v[2]+(v[3]+(v[4]+0)))))
```

Alternatively, we could have used a *left fold*:

```c++
(0+...+v)
(((((0+v[0])+v[1])+v[2])+v[3])+v[4])
```

In C++, the fold expressions are currently restricted to simplify the implementation of variadic templates.



