# Architecture
This document aims to describe the file structure and contents of this project.

# Root
The root directory contains the `itplus.h` header file, which is a single header version of the full library.

# include
The include directory contains all the header files for the `iterplus` interface library.
<table>
<tr>
  <th>File</th>
  <th>Description</th>
</tr>
<tr>
  <td>

  `itplus_chain.h`

  </td>
  <td>

  Macros for implementing the [`chain`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.chain) abstraction using the `IterChain` struct.

  An IterChain struct is a struct that stores 2 iterables, and starts consuming from the other one
  once one of them has been fully consumed. Essentially chaining together the 2 iterables.

  </td>
</tr>
<tr>
  <td>

  `itplus_collect.h`

  </td>
  <td>

  Macros for implementing the [`collect`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.collect) abstraction.

  Note: Rust's `collect` has a polymorphic return type. The `collect` here just turns an iterable into an array.

  </td>
</tr>
<tr>
  <td>

  `itplus_defn.h`

  </td>
  <td>

  Helpers for implementing all iterplus utilities at once, for given type.

  </td>
</tr>
<tr>
  <td>

  `itplus_drop.h`

  </td>
  <td>

  Macros for implementing the [`drop`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:drop) abstraction using the `IterDrop` struct.

  An IterDrop struct is a struct that keeps track of how many elements have been dropped out of an iterator as well as a limit to start extracting the elements at - the iterator impl for this struct will keep dropping from an iterator until it hits that limit, or until the source iterable gets exhausted, whichever comes first. Once enough elements have been dropped, it'll start extracting and returning the rest of the elements.

  </td>
</tr>
<tr>
  <td>

  `itplus_dropwhile.h`

  </td>
  <td>

  Macros for implementing the [`dropWhile`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:dropWhile) abstraction using the `IterDropWhile` struct.

  An IterDropWhile struct is a struct that stores a source iterable, a predicate function, and a flag to indicate when to stop testing with the predicate function. It keeps dropping from the source iterable until the predicate returns false, and starts returning the elements from the iterators from there on.

  </td>
</tr>
<tr>
  <td>

  `itplus_enumerate.h`

  </td>
  <td>

  Macros for implementing the [`enumerate`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.enumerate) abstraction using the `IterEnumr` struct.

  An IterEnumr struct is a struct that stores an iterable, and its iteration index. Its `next` function implementation that returns a `Pair` of 2 elements, first being the index, and second being an element from the source iterable.

  </td>
</tr>
<tr>
  <td>

  `itplus_filter.h`

  </td>
  <td>

  Macros for implementing the [`filter`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:filter) abstraction using the `IterFilt` struct.

  An IterFilt struct is a struct that stores a filtering function within it, as well as the source iterable.

  </td>
</tr>
<tr>
  <td>

  `itplus_filtermap.h`

  </td>
  <td>

  Macros for implementing the [`filter_map`](https://doc.rust-lang.org/std/iter/trait.Iterator.html#method.filter_map) abstraction using the `IterFiltMap` struct.

  An IterFiltMap struct is a struct that stores a filter-mapping function within it, as well as the source iterable. The filter-mapping function is just a function that returns a Maybe(T). A `Just` value indicates that the raw value should be kept, a `Nothing` value indicates it should be filtered out.

  </td>
</tr>
<tr>
  <td>

  `itplus_fold.h`

  </td>
  <td>

  Macros for implementing the [`foldl`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl) abstraction.

  </td>
</tr>
<tr>
  <td>

  `itplus_foreach.h`

  </td>
  <td>

  Definition of the `foreach` macro. Used to iterate through an `Iterable`.

  </td>
</tr>
<tr>
  <td>

  `itplus_iterator.h`

  </td>
  <td>

  Utilities to define and implement an Iterable.

  </td>
</tr>
<tr>
  <td>

  `itplus_macro_utils.h`

  </td>
  <td>

  General utility macros.

  </td>
</tr>
<tr>
  <td>

  `itplus_map.h`

  </td>
  <td>

  Macros for implementing the [`map`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:map) abstraction using the `IterMap` struct.

  An IterMap struct is a struct that stores a mapping function within it, as well as the source iterable.

  </td>
</tr>
<tr>
  <td>

  `itplus_maybe.h`

  </td>
  <td>

  Utilities to define and use a [Maybe](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Maybe.html) type.

  An IterMap struct is a struct that stores a mapping function within it, as well as the source iterable.

  </td>
</tr>
<tr>
  <td>

  `itplus_pair.h`

  </td>
  <td>

  Utilities to define and use a [Pair](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-Tuple.html) type. A Tuple of 2 elements.

  </td>
</tr>
<tr>
  <td>

  `itplus_reduce.h`

  </td>
  <td>

  Macros for implementing the [`reduce`/`foldl1`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:foldl1) abstraction.

  </td>
</tr>
<tr>
  <td>

  `itplus_take.h`

  </td>
  <td>

  Macros for implementing the [`take`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:take) abstraction using the `IterTake` struct.

  An IterTake struct is a struct that keeps track of how many elements have been extracted out of an iterator as well as a limit to stop at - the iterator impl for this struct will keep consuming from an iterator until it hits that limit, or until the source iterable gets exhausted, whichever comes first.

  </td>
</tr>
<tr>
  <td>

  `itplus_takewhile.h`

  </td>
  <td>

  Macros for implementing the [`takeWhile`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:takeWhile) abstraction using the `IterTakeWhile` struct.

  An IterTakeWhile struct is a struct that stores a source iterable, a predicate function, and a flag to indicate when to stop testing with the predicate function. It keeps consuming from the source iterable, and stops once the predicate returns false.

  </td>
</tr>
<tr>
  <td>

  `itplus_typeclass.h`

  </td>
  <td>

  Utilities to define a typeclass and its instance.

  </td>
</tr>
<tr>
  <td>

  `itplus_zip.h`

  </td>
  <td>

  Macros for implementing the [`zip`](https://hackage.haskell.org/package/base-4.15.0.0/docs/Data-List.html#v:zip) abstraction using the `IterZip` struct.

  An IterZip struct is a struct that stores 2 iterables, and has a `next` function implementation that returns a `Pair` of 2 elements, each from the 2 iterables.

  </td>
</tr>
</table>

# tests
The `tests/` directory contains usage tests (essentially examples) for the library.

<table>
<tr>
  <th>File</th>
  <th>Description</th>
</tr>
<tr>
  <td>

  `common.h`

  </td>
  <td>

  Declarations of iterator and iterplus structs used in the test executable.

  </td>
</tr>
<tr>
  <td>

  `impls.h`

  </td>
  <td>

  Declarations of utilities, structs, and functions for which iterator and iterplus utilities have been implemented.

  </td>
</tr>
<tr>
  <td>

  `impls.c`

  </td>
  <td>

  Implementations of iterables and iterplus utilities.

  </td>
</tr>
<tr>
  <td>

  `main.c`

  </td>
  <td>

  The main test executable using all the iterplus utilities.

  </td>
</tr>
<tr>
  <td>

  `preproc_map.h`

  </td>
  <td>

  Macro to [map over](https://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros) a given function/macro over __VA_ARGS__ - supports upto 8 arguments.

  </td>
</tr>
<tr>
  <td>

  `sugar.h`

  </td>
  <td>

  "Syntactic sugar". Convenience macros for using the iterplus utilities.

  The examples use C11's `_Generic` to statically dispatch to the correct functions based on the type of the iterplus structs. Syntax sugar, as you may know them. These are the definitions of those sugar macros.

  Essentially, wrapper functions are defined, which take a pre-allocated iterplus struct (of a specific type), the iterable to put into said struct (and other stuff if necessary), and return an Iterable corresponding to that iterplus struct. `_Generic` then statically dispatches to these functions.

  These wrapper functions are defined in `sugar.c`.

  The structure pre-allocation and passing to this functions is done in the `_Generic` macros by taking the address of a compound literal. The iterable struct, and other such members that have a varying type cannot be filled up inside the `_Generic` assoc list expressions, which is why the functions need to do it for us.

  </td>
</tr>
<tr>
  <td>

  `sugar.c`

  </td>
  <td>

  Wrapper functions to fill pre-allocated iterplus structs of specific types, and turn them into Iterables.

  </td>
</tr>
</table>

# samples
This is more or less a subset of tests. It contains examples of solving 2 problems-
* Finding longest common prefix of a string array
* Calculating the dot product of 2 arrays and then summing it.
