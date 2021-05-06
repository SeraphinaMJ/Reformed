# The Zip Function

## What is it?

`zip()` is a utility function which allows iterating over multiple lists at the
same time. Its design is based on the
[Python function of the same name](https://docs.python.org/3/library/functions.html#zip),
and works very similarly.

## How do I use it?

Usage of `zip()` is very simple. All that you need is at least one container to
give to `zip()`, and it will take care of creating a wrapper object for you to
iterate with.

```cpp
std::vector<int> my_container{5, 4, 3, 2, 1};
std::list<std::string> my_list{"fizz", "buzz", "spam", "foo", "bar"};
std::set<float> my_set{3.4, 3.5, 6.0};

for(auto&& result : zip(my_container, my_list, my_set)) {
    std::cout << "< " << std::get<0>(result) << ", " << std::get<1>(result)
              << ", " << std::get<2>(result) << ">" << std::endl;
}
```

Output:
```
<5, fizz, 3.4>
<4, buzz, 3.5>
<3, spam, 6.0>
```

Notice what is being done here: We are creating 3 different containers and
passing them directly to the `zip()` function before we simply iterate over its
return value. What specifically this function isn't all that important, just so
long as it is understood that the return value follows the C++ Named
Requirements for a [Container](https://en.cppreference.com/w/cpp/named_req/Container).
This means that it behaves like other standard C++ containers and supports
`std::begin`, `std::end`, etc...

Additionaly, as is implied by the support for `std::begin`, an iterator type is
also defined which follows the C++ Named Requirements for a
[LegacyForwardIterator](https://en.cppreference.com/w/cpp/named_req/ForwardIterator).
Therefore, one can use it in any context that they would normally use other
standard containers, such as the algorithms in `<algorithm>` or in range-based
for loops (as can be seen above).

The other thing to note about the iterator used by `zip()` is that the type one
gets from dereferencing it is a `std::tuple` containing the value from each
given container at the current step. This will be explained more in the next
section, but what it means is that one could rewrite the earlier example using
C++17 structured bindings like so:

```cpp
std::vector<int> my_container{5, 4, 3, 2, 1};
std::list<std::string> my_list{"fizz", "buzz", "spam", "foo", "bar"};
std::set<float> my_set{3.4, 3.5, 6.0};

for(auto&& [v1, v2, v3] : zip(my_container, my_list, my_set)) {
    std::cout << "< " << v1 << ", " << v2 << ", " << v3 << ">" << std::endl;
}
```
And the output will be the same as above.

Finally, one last thing to note about the way `zip()` behaves when used. Notice
how the output contains only 3 values being printed, rather than 5 like
`my_container` holds. The reason for this is that `zip()` will only continue to
give values up until the end of the shortest container given. The reason for
this is both to maintain compatibility with Python's implementation of `zip()`
as well as because the function needs to be able to fill all fields of the tuple
at each iteration step, which cannot be done after walking past the end of a
container.

## How does it work?

`zip()` is a very heavily templated function, the exact details of which are a
bit beyond the scope of this blog post. As such I will mostly just be going over
how it works at a high level. For information on the template tricks used
internally, read up on [template parameter packs](https://en.cppreference.com/w/cpp/language/parameter_pack),
[fold expressions](https://en.cppreference.com/w/cpp/language/fold), and
[the various std::tuple helper functions](https://en.cppreference.com/w/cpp/header/tuple).

At the high level, when calling `zip()` what is being created is a view of every
provided container at the same time. This means that the iterator one gets from
the object returned by `zip()` is internally storing an iterator into each given
container, which are all incremented in lock-step. When dereferencing this
iterator, the result one gets back out is a `std::tuple` of the result of
dereferencing each internally stored iterator.

As another way of representing this, here is a diagram of what `zip()` looks
like at each step of the for-loop one would use to call it.

| step | `my_container` | `my_list` | `my_set` |    `std::tuple`    |
|:----:|:--------------:|:---------:|:--------:|:------------------:|
|  0   |        5       |   "fizz"  |    3.4   | `<5, "fizz", 3.4>` |
|  1   |        4       |   "buzz"  |    3.5   | `<5, "buzz", 3.5>` |
|  2   |        3       |   "spam"  |    6.0   | `<5, "spam", 6.0>` |
|  3   |        2       |   "foo"   |          |                    |
|  4   |        1       |   "bar"   |          |                    |

