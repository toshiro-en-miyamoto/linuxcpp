# Templates

## Function Objects

One particularly useful kind of template is the *function object* (sometimes called a *functor*), which is used to define objects that can be called like functions.

```c++
template<typename T>
class less_than
{
    T val;
public:
    less_than(T x) : val{ x } {}
    bool operator()(const T& x) const { return x < val; }
};
```

The function called `operator()` implements the “function call,” “call,” or “application” operator `()`.

Such function objects are widely used as arguments to algorithms. For example, we can count the occurrences of values for which a predicate returns `true`:

```c++
template<typename C, typename P>
int count(const C& c, P pred)
{
    int cnt = 0;
    for (const auto& v : c)
    {
        if (pred(v)) cnt++;
    }
    return cnt;
}

int main()
{
    int vi[] { 1, 2, 3, 4, 5, 6 };
    less_than lti { 5 };
    assert(count(vi, lti) == 4);
}
```

The beauty of these function objects is that they carry the value to be compared against with them.
[...] Also, for a simple function object like `less_than`, inlining is simple, so a call of `less_than` is far more efficient than an indirect function call. The ability to carry data plus their efficiency makes function objects particularly useful as arguments to algorithms.

Function objects used to specify the meaning of key operations of a general algorithm (such as `less_than` for `count()`) are often referred to as *policy objects*.

## Lambda Expressions

```c++
int main()
{
    int vi[] { 1, 2, 3, 4, 5, 6 };
    constexpr int x = 5; 
    assert(count(vi, [&](int a){ return a < x;}) == 4);
}
```

The notation `[&](int a){ return a < x; }` is called a *lambda expression*. It generates a function object exactly like `less_than<int>{ 5 }`.

The `[&]` is a *capture list* specifying that all local names used in the lambda body (such as `x`) will be accessed through references. 

Had we wanted to “capture” only x, we could have said so: `[&x]`. Had we wanted to give the generated object a copy of `x`, we could have said so: `[=x]`. 

Capture nothing is `[ ]`, capture all local names used by reference is `[&]`, and capture all local names used by value is `[=]`.

