# C-Iterplus
Functional abstractions for working with iterators, as demonstrated in [c-iterators](https://github.com/TotallyNotChase/c-iterators).

[Finding the longest common prefix of an array of strings](./samples/main.c)
```c
#include "itplus.h"
#include "sugar.h"

#include <string.h>

static bool pair_is_equal(Pair(char, char) x) { return fst(x) == snd(x); }
static char fst_chr(Pair(char, char) chrpair) { return fst(chrpair); }

/* Using iterators to find common prefix - with C11 sugar */
static char* cmn_prefx(string s1, string s2)
{
    size_t len = 0;

    char* const prefx = collect(
        map(takewhile(
                zip(chrarr_to_iter(s1, strlen(s1)), chrarr_to_iter(s2, strlen(s2))),
            pair_is_equal),
        fst_chr),
    &len);
    prefx[len] = '\0';
    return prefx;
}

/* Accumulator function wrapping `cmn_prefx` - accumulator type is a heap allocated string */
static inline char* acc_cmn_prefx(char* acc, string s)
{
    char* const res = cmn_prefx(acc, s);
    free(acc);
    return res;
}

int main(void)
{
    string arr[]  = {"flower", "flow", "flight"};
    size_t arrlen = sizeof(arr) / sizeof(*arr);

    char* const lngest_prefx = fold(strarr_to_iter(arr + 1, arrlen - 1), strdup_(arr[0]), acc_cmn_prefx);
    puts(lngest_prefx); /* "fl" */ 
    free(lngest_prefx);
    return 0;
}
```
Output-
> fl

Just like the [polymorphic iterators](https://github.com/TotallyNotChase/c-iterators), most of the abstractions are also implemented using [The Typeclass Pattern](https://github.com/TotallyNotChase/c-iterators/Typeclass%20Pattern.md). A polymorphism pattern based around actions, similar to **Haskell typeclasses**, **Java/C# interfaces**, or **Rust traits**. This pattern aims for-
* Extensibility (through dynamic dispatch)
* Minimal runtime cost (the only *real cost* being the dynamic dispatch)
* No dynamic memory allocations
* Type safety is a first class concern

The term "Iterplus" is shamelessly ~~stolen~~ borrowed from [the original iterplus](https://github.com/Aplet123/iterplus).

Information about file structure and file contents is described in [ARCHITECTURE](./ARCHITECTURE.md).
You can find the generated docs and general information [here](https://totallynotchase.github.io/c-iterplus/).

# Highlights
* Wrtten in pure, standard C (C99 for the actual utilities, C11 for extra syntactic sugar)
* Every utility that returns an iterable is *lazy*; nothing is *evaluated* until the iterable is **explicitly iterated over**
* No hidden memory allocations
* Type safety through the typeclass pattern
* It's kinda fun to play around with functional abstractions, I guess

# Available Abstractions
* [`take`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:take) - defined in [itplus_take.h](./include/itplus_take.h)
* [`drop`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:drop) - defined in [itplus_take.h](./include/itplus_drop.h)
* [`take_while`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:takeWhile) - defined in [itplus_takewhile.h](./include/itplus_takewhile.h)
* [`drop_while`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:dropWhile) - defined in [itplus_dropwhile.h](./include/itplus_dropwhile.h)
* [`map`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:map) - defined in [itplus_map.h](./include/itplus_map.h)
* [`filter`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:filter) - defined in [itplus_filter.h](./include/itplus_filter.h)
* [`filter_map`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.filter_map) - defined in [itplus_filtermap.h](./include/itplus_filtermap.h)
* [`reduce`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl1) - defined in [itplus_reduce.h](./include/itplus_reduce.h.h)
* [`fold`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl) - defined in [itplus_fold.h](./include/itplus_fold.h)
* [`enumerate`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.enumerate) - defined in [itplus_enumerate.h](./include/itplus_enumerate.h)
* [`zip`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:zip) - defined in [itplus_zip.h](./include/itplus_zip.h)
* [`collect`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.collect) - defined in [itplus_collect.h](./include/itplus_collect.h)

You can also implement your own abstractions using the same pattern. Refer to [Semantics](#semantics-and-explanation).

# Usage
Just include `itplus.h` and get to using it!

Before anything, you should familiarize yourself with the basics of these iterators. Discussed in [c-iterators](https://github.com/TotallyNotChase/c-iterators). You just need to know how to iterate through iterables though. TL;DR- you use the [`foreach`](./include/itplus_foreach.h) macro to iterate over an iterable.

Refer to [tests](./tests/main.c) or [samples](./samples/main.c) to look at how to implement the utilities. In general the pattern goes like this-
1. Define the iterplus structs required to define the specific utilities. Include it in a header.

   For `tests/`, these are in [common.h](./tests/common.h).

   For `samples/`, these are in [impls.h](./samples/impls.h).
2. Define the specific iterplus utility function using the corresponding `define_` macro.

   For `tests/`, these are in [impls.c](./tests/impls.c).

   For `samples/`, these are in [impls.c](./samples/impls.c).
3. Include the function signature of this defined in a header.
   
   For `tests/`, these are in [impls.h](./tests/impls.h).

   For `samples/`, these are in [impls.h](./samples/impls.h).

There's also the `IterPlus`, `DeclIterPlus`, and `DefnIterPlus` macro to define/declare all iterplus utility structs, and utilities themselves at once, for a specific type.

Most of these functions take in a pointer to an iterplus struct, that is filled with necessary information - and turns it into an iterable. For example, the `define_itertake_func` macro defines a function that takes in an `IterTake` struct, containing a specific type of iterable and the `limit` value (as well as `i` set to 0), and returns an `Iterable` that has, at most `limit` amount of elements. All of these elements are consumed from the iterable put into the `IterTake` struct you passed to the function. The *lifetime of the returned iterable* is the **same as the lifetime of the `IterTake` struct** pointed to by the given pointer.

Though I should mention, these abstractions are just for fun. I wanted to see what I could do with [c-iterators](https://github.com/TotallyNotChase/c-iterators). I think the [the typeclass based polymorphism pattern](https://github.com/TotallyNotChase/c-iterators/Typeclass%20Pattern.md) is really useful. Even the regular lightweight iterators are certainly useful. But with the lack of closures, no way to capture types, and somewhat "meh" static dispatch support - there's **a LOT** of boilerplate needed to get you up on your feet even after including `itplus.h`. It's pretty fun once you actually define all of this boilerplate - but that's not a great thing.

# Semantics and Explanation
Most abstractions are documented in their own files, so you can view them in [docs](https://totallynotchase.github.io/c-iterplus/). The general idea behind every abstraction is further explained in the ["Lazy Abstractions"](https://github.com/TotallyNotChase/c-iterators#lazy-abstractions) part of the c-iterators README.

# C11 Syntax Sugar
With [`_Generic`](https://en.cppreference.com/w/c/language/generic) you can make macros to statically dispatch to the iterplus utility functions. Essentially making the usage be expression oriented, and certainly much better to use. But once again, more boilerplate, heh.

The main skeleton of these can be found in [sugar.h](./tests/sugar.h)) and [sugar.c](./tests/sugar.c).

In general, you could implement generic selection with a macro `take`, that takes an iterable and the amount of items to "take" from it. To do this, however, you need
* A function that can put that iterable into an `IterTake` struct, as well as the given limit.
* A way to pass this `IterTake` struct into the function defined using `define_itertake_func`.

  As mentioned before, most of the functions defined using the `define_` macros, take in a pointer to an iterplus struct (in this case `IterTake`), and return an `Iterable` according to the `IterTake` struct's `Iterator` implementation.

All of this should ideally avoid heap allocation. This means you can't have a function that creates a local `IterTake` struct, since its lifetime ends when the function ends, so the returned `Iterable` would be pointing to invalid memory. You need to somehow make the `IterTake` struct in the same scope.

Suppose the `take` utility has been implemented for an `Iterable(int)`-
```c
define_itertake_func(int, takeint_to_itr)
```
i.e the function that turns an `IterTake(int)*` to the corresponding `Iterable(int)`, is named `takeint_to_itr`. The signature of this function is- `Iterable(int) takeint_to_itr(IterTake(int)* x);`

You would use the `take` utility on an `Iterable(int)`, `it` like so-
```c
Iterable(int) it = ...;

Iterable(int) first_10 = takeint_to_itr(&(IterTake(int)){ .limit = 10, .src = it });
```
[Compound literals](https://en.cppreference.com/w/c/language/compound_literal) allow you to create the struct and pass a pointer to it into the function, all in the same scope, in the same line.

This is pretty neat, but now you need to make this generic, something like-
```c
Iterable(int) first_10 = take(it, 10);
```
where `takeint_to_itr` is statically dispatched to by figuring out the type of `it`.

But `_Generic` takes actual expressions as values in its assoc list. This means you can't do-
```c
#define take(it, n) _Generic((it), Iterable(int): takeint_to_itr)(&(IterTake(_Generic((it), Iterable(int): int))){ .limit = (n), .src = (it) })
```
Since `int` is not a valid expression, it's a type! Making `_Generic((it), Iterable(int): int)` a compile error.

You also can't do-
```c
#define take(it, n) _Generic((it), Iterable(int): takeint_to_itr)(_Generic((it), Iterable(int): &(IterTake(int)){ .limit = (n), .src = (it) }))
```
[Because every expression in the assoc list of `_Generic` must be correct, whether or not they're actually chosen](https://stackoverflow.com/questions/24743520/incompatible-pointer-types-passing-in-generic-macro). `it` of type `Iterable(int)` would not be a valid assignment to `.src` to `IterTake(char)` for example. Even though that branch wouldn't be chosen anyway. This is a rather unfortunate and silly part of the spec.

Instead, you'll need a function, that does the following
* Takes in a pointer to an iterplus struct allocated in the same scope as the macro caller
* Takes in the iterable itself
* Takes in some extra context arguments if required
* Puts the iterable and the extra context specific arguments into the given struct

For the `take` utility, this function looks like (for `Iterable(int)`)-
```c
Iterable(int) prep_tkint(IterTake(int) * tk, Iterable(int) x)
{
    tk->src = x;
    return takeint_to_itr(tk);
}
```

Now you can make the `take` macro using `_Generic` selection-
```c
#define take(it, n) _Generic((it), Iterable(int): prep_tkint)(_Generic((it), Iterable(int): &(IterTake(int)){ .limit = (n)}), (it))
```
And you can add in more types as needed to the assoc list, provided you have their specific `prep_tkint`-like wrapper function.

This is the core idea, it's expanded upon using a tiny bit of metaprogramming in [sugar.h](./tests/sugar.h).

# Pitfalls, Memory, Lifetime, and Ownership
* When you use one iterable to build another iterable through the utilities, no implicit cloning is done. The new iterable is just consuming from the original iterable, possibly with some added context. This is pretty normal for iterators - but should be kept in mind.

  Unless you're chaining abstractions together, don't feed the same iterator into multiple utilities. Iterating through one of those utilities will iterate from the other one as well. This is why, in snippets like [this one](https://github.com/TotallyNotChase/c-iterplus/blob/master/tests/main.c#L231), `get_fibitr` is called multiple times to acquire a new iterable. Otherwise, the utility `chain` would be chaining together 2 of the same iterables.... essentially achieving nothing.

  Instead, make a new iterator. Making iterators should be very inexpensive.
* Consumers of iterables **should not** be expected to free the elements extracted from the iterable. Iterables **should never** clone data from their source. The iterator implementation for an array of strings should yield **the exact same pointers** contained within the array. No implicit cloning of the strings. The memory is owned by the array, and it is the responsibility of that array's owner to free the strings in it. The strings extracted from the iterable should not be freed.

  This is not a forced requirement, but it is certainly encouraged to have a standard memory model. Every iterplus utility follows this principle. Nothing will ever be cloned, the values from the sources are simply re-extracted. It's the responsibility of the original source to manage memory.

  This works great when chaining abstractions as well. Convert a array of pointers to an iterable, `zip` with another array of pointers (doesn't have to be the same type of pointers), `filter` it by a `predicate` function, `drop` the first `N` elements, `enumerate` them to get a final iterable. Now, when you iterate over this resulting iterable, every element is still the same elements from the original arrays. No need to manage the memory for these, leave that to whoever is handling the arrays themselves.

  A problem may arise, however, when you use utilities such as `map`, or `filter_map`. Here, it's upto the user about what they return. You could `map` over an iterable to make another iterable where each element is actually heap allocated. In which case, you **do** need to free the elements extracted from the iterable. This should not be a problem however, since it's completely transparent. If you mapped with a function that heap allocates - well you *know* the elements will need to be freed. The function is provided by *you*.
  
  But what happens if you need to feed this iterable into a general API function, an API function that adheres to the standard principle of **not freeing** the iterable's elements? In general, you should use the `collect` utility to collect the iterable's data into an array. Now the array contains the heap allocated data that you can free later. Convert this back to an iterable and feed it into the API function.

# Examples
You can find the implementations and examples within [tests](./tests/main.c) and [samples](./samples/main.c).
