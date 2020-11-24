# Containers

## Vectors

Assigning a vector involves copying its elements. Thus, after the initialization of `book2`, `book2` and `phone_book` hold separate copies of every `Entry` in the phone book.

```c++
vector<Entry> book2 = phone_book;
```

The standard-library `vector` is very flexible and efficient. Use it as your default container; that is, use it unless you have a solid reason to use some other container.

### Elements

Like all standard-library containers, `vector` is a container of elements of some type `T`, that is, a `vector<T>`. Just about any type qualifies as an element type: built-in numeric types (such as `char`, `int`, and `double)`, user-defined types (such as `string`, `Entry`, `list<int>`, and `Matrix<double,2>`), and pointers (such as `const char*`, `Shape*`, and `double*`).

When you insert a new element, its value is copied into the container. The element is not a reference or a pointer to some object.

If you have a class hierarchy that relies on virtual functions to get polymorphic behavior, do not store objects directly in a container. Instead store a pointer (or a smart pointer).

```c++
// No, don't - there is no room for a Circle or a Smiley
vector<Shape> vs;

// better, but see §4.5.3
vector<Shape*> vps;

vector<unique_ptr<Shape>> vups;    // OK
```

#### [4.5.3 Avoiding Resource Leaks]

```c++
void user()
{
    std::vector<Shape*>v;
    while (cin)
        v.push_back(read_shape(cin));
    draw_all(v);
    rotate_all(v,45);
    for (auto p : v)
        delete p;
}
```

[...] three opportunities for mistakes:

- The implementer of `Smiley` may fail to `delete` the pointer to `mouth`.
- A user of `read_shape()` might fail to `delete` the pointer returned.
- The owner of a container of `Shape` pointers might fail to `delete` the objects pointed to.

In that sense, pointers to objects allocated on the free store is dangerous: a “plain old pointer” should not be used to represent ownership.

```c++
void user(int x)
{
    Shape* p = new Circle{Point{0,0},10};
    // ...
    if (x<0) throw Bad_x{};   // potential leak
    if (x==0) return;         // potential leak
    // ...
    delete p;
}
```

One simple solution to such problems is to use a standard-library `unique_ptr` (§13.2.1) rather than a “naked pointer” when deletion is required:

```c++
class Smiley : public Circle {
    // ...
private:
    vector<unique_ptr<Shape>> eyes;
    unique_ptr<Shape> mouth;
};
```

As a pleasant side effect of this change, we no longer need to define a destructor for `Smiley`. The compiler will implicitly generate one that does the required destruction of the `unique_ptr`s in the `vector`. The code using `unique_ptr` will be exactly as efficient as code using the raw pointers correctly.

```c++
unique_ptr<Shape> read_shape(istream& is)
{
    switch (k) {
    case Kind::circle:
        return unique_ptr<Shape>{new Circle{p,r}};
    // ...
}

void user()
{
    vector<unique_ptr<Shape>> v;
    while (cin)
        v.push_back(read_shape(cin));
    draw_all(v);
    rotate_all(v,45);
} // all Shapes implicitly destroyed
```

### Range Checking

The standard-library vector does not guarantee range checking.

```c++
void silly(vector<Entry>& book)
{
    int i = book[book.size()].number;  // out of range
    // ...
}
```

That initialization is likely to place some random value in i rather than giving an error. This is undesirable, and `out-of-range` errors are a common problem.

```c++
template<typename T>
class Vector : public std::vector<T>
{
public:
    T& operator[](int i) 
        { return std::vector<T>::at(i); }
    const T& operator[](int i) const 
        { return std::vector<T>::at(i); }
};
```

The `at()` operation is a `vector` subscript operation that throws an exception of type `out_of_range` if its argument is out of the vector’s range.

Why doesn’t the standard guarantee range checking? Many performance-critical applications use `vector`s and checking all subscripting implies a cost on the order of 10%.

At least `vector` is easily range checked at debug time and we can build checked versions on top of the unchecked default. Some implementations save you the bother of defining `Vec` (or equivalent) by providing a range-checked version of `vector` (e.g., as a compiler option).

A range-`for` avoids range errors at no cost by accessing elements through iterators in the range `[begin():end()`).

## List

The standard library offers a doubly-linked list called `list`.

Every standard-library container provides the functions `begin()` and `end()`, which return an iterator to the first and to one-past-the-last element, respectively.

```c++
int get_number(const string& s)
{
    for (auto p = phone_book.begin(); p!=phone_book.end(); ++p)
        if (p−>name==s)
            return p−>number;
    return 0; // use 0 to represent "number not found"
}
```

Given an iterator `p`, and when p refers to a class with a member m, then 
- `*p` is the element to which it refers, 
- `++p` advances `p` to refer to the next element, and
- `p−>m` is equivalent to `(*p).m`.

```c++
void f(const Entry& ee, 
    list<Entry>::iterator p, list<Entry>::iterator q)
{
    // add ee before the element referred to by p
    phone_book.insert(p, ee);
    // remove the element referred to by q
    phone_book.erase(q);
}
```

For a `list`, `insert(p, elem)` inserts an element with a copy of the value `elem` before the element pointed to by `p`. [...] Conversely, `erase(p)` removes the element pointed to by `p` and destroys it.

When all we want is a sequence of elements, we have a choice between using a `vector` and a `list`. Unless you have a reason not to, use a `vector`.

The standard library also offers a singly-linked list called `forward_list`.

A `forward_list` differs from a list by only allowing forward iteration. The point of that is to save space. There is no need to keep a predecessor pointer in each link and the size of an empty `forward_list` is just one pointer. A `forward_list` doesn’t even keep its number of elements. If you need the element count, count. If you can’t afford to count, you probably shouldn’t use a `forward_list`.

## Map

The standard library offers a balanced binary search tree (usually, a red-black tree) called `map`. [...] In other contexts, a `map` is known as an associative array or a dictionary.

```c++
map<string, int> phone_book {
    {"David Hume", 123456},
    {"Karl Popper", 234567},
    {"Bertrand Arthur William Russell", 345678}
};
```

When indexed by a value of its first type (called the *key*), a map returns the corresponding value of the second type (called the *value* or the *mapped type*).

```c++
int get_number(const string& s)
{
    return phone_book[s];
}
```

If a key isn’t found, it is entered into the map with a default value for its value. The default value for an integer type is 0.

If we wanted to avoid entering invalid numbers into our phone book, we could use `find()` and `insert()` instead of `[ ]`.

## Unordered Maps

The cost of a map lookup is `O(log(n))` where n is the number of elements in the map. That’s pretty good. For example, for a map with 1,000,000 elements, we perform only about 20 comparisons and indirections to find an element. 

However, in many cases, we can do better by using a hashed lookup rather than a comparison using an ordering function, such as `<`. The standard-library hashed containers are referred to as “unordered” because they don’t require an ordering function.

The standard library provides a default hash function for `string`s as well as for other built-in and standard-library types. If necessary, you can provide your own.

```c++
struct Record
{
    string name;
    int product_code;
}

struct Rhash
{
    size_t operator()(const Record& r) const
    {
        return hash<string>()(r.name) ^ hash<int>()(r.product_code);
    }
}

unordered_set<Record, Rhash> my_set;
```

We can avoid explicitly passing the `hash` operation by defining it as a specialization of the standard-library `hash`:

```c++
namespace std
{
    template<> struct hash<Record>
    {
        using argument_type = Record;
        using result_type = std::size_t;

        size_t operator()(const Record& r) const
        {
            return hash<string>()(r.name) ^ hash<int>()(r.product_code);
        }
    }
}
```

Note the differences between a `map` and an `unordered_map`:

- A `map` requires an ordering function (the default is `<`) and yields an ordered sequence.
- A `unordered_map` requires an equality function (the default is `==`); it does not maintain an order among its elements.

Given a good `hash` function, an `unordered_map` is much faster than a `map` for large containers.

## Container Overview

| Container | Description |
| --- | --- |
| vector\<T>               | A variable-size vector |
| list\<T>                 | A doubly-linked |
| forward_list\<T>         | A singly-linked list |
| deque\<T>                | A double-ended queue |
| set\<T>                  | A set (a map with just a key and no value) |
| multiset\<T>             | A set in which a value can occur many times |
| map\<K,V>                | An associative array |
| multimap\<K,V>           | A map in which a key can occur many times |
| unordered_map\<K,V>      | A map using a hashed lookup |
| unordered_multimap\<K,V> | A multimap using a hashed lookup |
| unordered_set\<T>        | A set using a hashed lookup |
| unordered_multiset\<T>   | A multiset using a hashed lookup |
| | |

The standard library also provides more specialized container-like types, such as `array<T,N>` and `bitset<N>`  (§13.4).

The standard containers and their basic operations are designed to be similar from a notational point of view. Furthermore, the meanings of the operations are equivalent for the various containers. Basic operations apply to every kind of container for which they make sense and can be efficiently implemented:

| Operation | Description |
| --- | --- |
| value_type     | The type of an element |
| p=c.begin()    | `p` points to first element of `c`; also `cbegin()` for an iterator to `const` |
| p=c.end()      | `p` points to one-past-the-last element of `c`; also `cend()` for an iterator to `const` |
| k=c.size()     | `k` is the number of elements in `c` |
| c.empty()      | Is `c` empty? |
| k=c.capacity() | `k` is the number of elements that `c` can hold without a new allocation |
| c.reserve(k)   | Make the capacity `k` |
| c.resize(k)    | Make the number of elements `k`; added elements has the value `value_type{}` |
| c[k]           | The kth element of `c`; no range checking |
| c.at(k)        | The kth element of `c`; if out of range, throw `out_of_range` |
| c.push_back(x) | Add `x` at the end of `c`; increase the size of `c` by one |
| c.emplace_back(a) | Add `value_type{a}` at the end of `c`; increase the size of `c` by one |
| q=c.insert(p,x) | Add `x` before `p` in `c` |
| q=c.erase(p)    | Remove element at `p` from `c` |
| c=c2            | Assignment |
| b=(c==c2), also != | Equality of all elements of `c` and `c2`; `b==true` if equal |
| x=(c\<c2), also \<=, >, >= | Lexicographical order of `c` and `c2`: `x<0` if less than, `x==0` if equal, and `0<x` if greater than |
| | |

This notational and semantic uniformity enables programmers to provide new container types that can be used in a very similar manner to the standard ones.

However, each has strengths and weaknesses. For example, 

- subscripting and traversing a `vector` is cheap and easy
- `vector` elements are `move`d when we insert or remove elements 
- `list` has exactly the opposite properties
- `forward_list` is optimized for the empty sequence

An emplace operation, such as `emplace_back()` takes arguments for an element’s constructor and builds the object in a newly allocated space in the container, rather than copying an object into the container. For example, for a `vector<pair<int,string>>` we could write:

```c++
// make a pair and move it into v
v.push_back(pair{1,"copy or move"});

// build a pair in v
v.emplace_back(1,"build in place");
```

## Advice

- Use `reserve()` to avoid invalidating pointers and iterators to elements
- Don’t use iterators into a resized `vector`
- Pass a container by reference and return a container by value
