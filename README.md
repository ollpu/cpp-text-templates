# C++ Text Templates

## What?
Create template `HTML` (or any kind of file) with native C++ written into it.
`<%` starts a code block and `%>` ends it. Using `<%=` will cause the expression
to be evaluated and output.

#### Example:

```
Hello world!
<% for (int i = 0; i < 5; ++i) { %>
  Iteration <%= i %>
<% } %>
<% #include "other.cpp" %>
```

gets turned into:

```cpp
#ifndef OUT
#define OUT out
#define TMP_OUT
#endif
{
OUT << u8"Hello world!\n";
 for (int i = 0; i < 5; ++i) { 
;OUT << u8"\n  Iteration ";
OUT << ( i );
OUT << u8"\n";
 } 
;OUT << u8"\n";
 #include "other.cpp" 
;OUT << u8"\n";
}
#ifdef TMP_OUT
#undef OUT
#undef TMP_OUT
#endif
```

You can now use the generated `.cpp` file in other programs like this:

```cpp
std::stringstream out;
#include "file.cpp"
```

The template will be executed and it will output into `out`. (change the output
buffer name by defining macro OUT).
It will have access to all the variables in the scope it was
included at. The possibilites are endless.

**Warning:** Text output using the `<%= %>`-tag is not escaped in any way. You
might want to use a `stringstream` wrapper, which handles string escaping.

Use `\<%` or `\%>` to indicate a literal tag.

## Using the parser

Compile first:
```
./compile.sh
```

```
./parser [FILE]... # Parse list of files (output .cpp)
./parser -- # Use standard input & output
./parser (-d|--delimiters) "<% = %>" ... # Use different tags
```

Try the example!
```
./parser example
```
