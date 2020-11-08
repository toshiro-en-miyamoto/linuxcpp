# Modularity
## Exceptions
Consider again the Vector example. What ought to be done when we try to access an element that is out of range for the vector from §2.3?

Assuming that out-of-range access is a kind of error that we want to recover from, the solution is for the Vector implementer to detect the attempted out-of-range access and tell the user about it. The user can then take appropriate action.

```C++
import Vector;
import <stdexcept>;

double& Vector::operator[](int i)
{
    assert(elem!=nullptr);
    if (i<0 || sz<=i)
        throw std::out_of_range{"Vector::operator[]"};
    return elem[i];
}
```

The use of exceptions to signal out-of-range access is an example of a function checking its argument and refusing to act because a basic assumption, a *precondition*, didn’t hold. Had we formally specified Vector’s subscript operator, we would have said something like “the index must be in the [0:sz) range,” and that was in fact what we tested in our operator[]().

In particular, we did say “elem points to an array of sz doubles” but we only said that in a comment. Such a statement of what is assumed to be true for a class is called a *class invariant*, or simply an *invariant*.

Throwing an exception is not the only way of reporting an error that cannot be handled locally. A function can indicate that it cannot perform its allotted task by:

- throwing an exception
- somehow return a value indicating failure
- terminating the program (by invoking a function like terminate(), exit(), or abort()).

Don’t believe that all error codes or all exceptions are bad; there are clear uses for both.

RAII (§4.2.2, §5.3) is essential for simple and efficient error-handling using exceptions. Code littered with try-blocks often simply reflects the worst aspects of error-handling strategies conceived for error codes.

If the condition of an `assert()` fails in “debug mode,” the program terminates. If we are not in debug mode, the assert() is not checked.

The `static_assert` mechanism can be used for anything that can be expressed in terms of constant expressions (§1.6).
The most important uses of static_assert come when we make assertions about types used as parameters in generic programming
