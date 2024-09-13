# xcl

Extended C Library

## Naming convention

`xc[StructName]Init[...]`: initializes the contents of a type but does not allocate it

`xc[StructName]New[...]`: initializes the contents of a type on a new heap-allocated instance

`xc[StructName]Destroy`: destroys the contents of a type, does not free the pointer passed to it (inverse of `Init`)

`xc[StructName]Free`: destroys the contents of a type and frees the pointer passed to it (inverse of `New`)

`xc[StructName]Next[...]`: used to iterate over a type

Any out parameters must be placed last and begin with `out`
