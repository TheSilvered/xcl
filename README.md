# xcl

Extended C Library

## Code conventions

### Type names

Other than the basic types (`i8`, `uint`, `f64`), any other type should begin with `XC` and use `PascalCase` for the rest of the name.

For example:

```text
XCArray ✅
xcArray ❌
```

### Function names

If a function is tied to a specific type it should start with the type name but beginning with `xc` instead of `XC` and using `PascalCase` for the rest of the name.

For example:

```text
xcArrayInit ✅
XCArrayInit ❌
```

A function name should go in order `[Type]-Task-[Specification]-[Variation]`. In other words the name for a function shoud start with the name of the type it is tied to if there is one, followed by what the function is tasked to do and then there may be other specifying features (the "how" of the function) of the function and ending with any variation from another function.

For example:

```text
xcArrayInit ✅
xcInitArray ❌
(type = xcArray, task = Init)

xcArrayRemoveFilter ✅
xcArrayFilterRemove ❌
(type = xcArray, task = Remove, specification (how?) = Filter)

xcArrayFindAll ✅
findAllXCArray ❌
(type = xcArray, task = Find, variation = All)
```

### Special function names

Functions containing these words should behave consistently:

- `Init`: initializes the contents of a type but does not allocate it
- `New`: initializes the contents of a type on a new heap-allocated instance
- `Destroy`: destroys the contents of a type, does not free the pointer passed to it (inverse of `Init`)
- `Free`: destroys the contents of a type and frees the pointer passed to it (inverse of `New`)
- `Next`: used to iterate over a type

### Function parameters

If a function is tied to a specific type and requires a reference to it, its argument must be placed first.

```text
XCRef xcArrayGet(XCArray *array, isize index); ✅
XCRef xcArrayGet(isize index, XCArray *array); ❌
```

Any out parameters must be placed last and begin with `out`

Any in-out parameters must be placed after all other parameters but before `out` parameters and must begin with `inout`.

### `size` and `len`

This terms should refer to two different concepts:

- `size` is the size in bytes of any given block of data
- `len` is the number of units (this term may refer to different things depending on the context) in any data structure

For example the string `"aò"`, encoded in UTF-8, has a size of `3` (three bytes are needed to encode it), but a length
of `2` (it is composed of two characters).

### Using `void *`

Using `void *` is highly discouraged, use either `XCMemBlock` or `XCRef` to communicate intent more clearly.

`XCMemBlock` should be used whenever the data being passed is a generic block of memory.

`XCRef` should be used whenever the data being passed is a generic reference to a specific value.

## Code style

### Line length

Lines should not be longer than 120 characters to aid redability.

### Breaking long lines

When a line is too long consider doing the following:

- use more variables to store complex calculations
- break a function call over multiple lines with one argument per line

### Braces

Opening braces should go on the same line.

In `if-else` chains, `else` should go on the same line of the last brace, similarly should `while` in `do-while` loops.

You can omit braces in `for` and `while` loops and in `if-else` statements when there would only be one line inside.

Always use braces in `do-while` loops.

For example

```c
// OK ✅

if (a == b) x++;

if (a == b)
    x++;

if (a == b) {
    x++;
}

if (a == b) {
    for (int i = 0; i < b; i++)
        x[i] = a;
} else if (a < b) {
    for (int i = 0; i < b; i++)
        x[i] = b * b;
} else {
    for (int i = 0; i < b; i++) {
        x[i] = 2 * b;
        y[i] = 3 * b;
    }
}

do {
    printf("Hello, world!\n");
} while (true);

// Wrong ❌

if (a == b)
{ // Brace on next line
    x++;
}

if (a == b) // Multiple lines inside if without braces
    for (int i = 0; i < b; i++)
        x[i] = a;
else if (a < b)
{ // Brace on next line
    for (int i = 0; i < b; i++)
        x[i] = b * b;
}
else // else on next line
{
    for (int i = 0; i < b; i++) {
        x[i] = 2 * b;
        y[i] = 3 * b;
    }
}

do // do-while loop without braces
    printf("Hello, world!\n");
while (true);

do {
    printf("Hello, world!\n");
}
while (true); // while on next line
```
