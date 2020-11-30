# Numerics

In `<numeric>`, we find a small set of generalized numerical algorithms:

| Algorithm | Description |
| --- | --- |
| x=accumulate(b,e,i)              | `x` is the sum of `i` and the elements of `[b:e)` |
| x=accumulate(b,e,i,f)            | accumulate using `f` instead of `+` |
| x=inner_product(b,e,b2,i)        | `x` is the inner product of `[b:e)` and `[b2:b2+(e−b))`, that is, the sum of `i` and `(*p1)*(*p2)` for each `p1` in `[b:e)` and the corresponding `p2` in `[b2:b2+(e−b))` |
| x=inner_product(b,e,b2,i,f,f2)   | `inner_product` using `f` and `f2` instead of `+` and `*` |
| p=partial_sum(b,e,out)           | Element `i` of `[out:p)` is the sum of elements `[b:b+i]` |
| p=partial_sum(b,e,out,f)         | `partial_sum` using `f` instead of `+` |
| p=adjacent_difference(b,e,out)   | Element `i` of `[out:p)` is `*(b+i)−*(b+i−1)` for `i>0`; if `e−b>0`, then `*out` is `*b` |
| p=adjacent_difference(b,e,out,f) | `adjacent_difference` using `f` instead of `−` |
| iota(b,e,v)                      | For each element in `[b:e)` assign `++v`; thus the sequence becomes `v+1`, `v+2`, ... |
| x=gcd(n,m)                       | `x` is the greatest common denominator of integers `n` and `m` |
| x=lcm(n,m)                       | `x` is the least common multiple of integers `n` and `m` |
| | |

In `<numeric>`, the numerical algorithms have parallel versions (§12.9):

| Algorithm | Description |
| --- | --- |
| x=reduce(b,e,v)                             | `x=accumulate(b,e,v)`, except out of order |
| x=reduce(b,e)                               | `x=reduce(b,e,V{})`, where `V` is `b`’s value type |
| x=reduce(pol,b,e,v)                         | `x=reduce(b,e,v)` with execution policy `pol` |
| x=reduce(pol,b,e)                           | `x=reduce(pol,b,e,V{})`, where `V` is `b`’s value type |
| p=exclusive_scan(pol,b,e,out)               | `p=partial_sum(b,e,out)` according to `pol`, excludes the `i`th input element from the `i`th sum |
| p=inclusive_scan(pol,b,e,out)               | `p=partial_sum(b,e,out)` according to `pol` includes the `i`th input element in the `i`th sum |
| p=transform_reduce(pol,b,e,f,v)             | `f(x)` for each `x` in `[b:e)`, then reduce |
| p=transform_exclusive_scan(pol,b,e,out,f,v) | `f(x)` for each `x` in `[b:e)`, then `exclusive_scan` |
| p=transform_inclusive_scan(pol,b,e,out,f,v) | `f(x)` for each `x` in `[b:e)`, then `inclusive_scan` |
| | |

The parallel algorithms (e.g., `reduce()`) differ from the sequentional ones (e.g., `accumulate()`) by allowing operations on elements in unspecified order.

| Execution Policy defined in `<execution>` |
| --- |
| inline constexpr std::execution::sequenced_policy seq { /* unspecified */ }; |
| inline constexpr std::execution::parallel_policy par { /* unspecified */ }; |
| inline constexpr std::execution::parallel_unsequenced_policy par_unseq { /* unspecified */ }; |
| inline constexpr std::execution::unsequenced_policy unseq { /* unspecified */ }; |
| |

> Advice
>
> Consider `accumulate()`, `inner_product()`, `partial_sum()`, and `adjacent_difference()` before you write a loop to compute a value from a sequence

## Random Numbers

A random number generator consists of two parts:
- An engine that produces a sequence of random or pseudo-random values
- A distribution that maps those values into a mathematical distribution in a range

```c++
constexpr int d_min {1};
constexpr int d_max {6};
std::uniform_int_distribution<> distri {d_min, d_max};
std::default_random_engine engine {};

constexpr std::size_t cnt {1'000'000};
static_assert(d_max * cnt < std::numeric_limits<int>::max());
std::vector<int> v(cnt);

for (auto& p : v)
{ p = distri(engine); }

int sum = std::accumulate(v.begin(), v.end(), 0);
float avg = static_cast<float>(sum) / cnt;
assert(3.49 < avg && avg < 3.51);
```

> Advice
>
> Don’t use the C standard-library `rand()`; it isn’t insufficiently random for real uses.
