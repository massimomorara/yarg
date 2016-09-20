
#Yarg

## Yet Another Reinvented Getotp

Yarg is a one-header-only C++11 object oriented replacement for getopt() and similar software.

Support long options (`--help`), short options (`-h`), more short option in sequence (`-hvl`), options without values (flags) or with values, negative flags (`--verbose` and `--noverbose` bounded on the same flag) repeatable options (`--name foo --name=bar --name "foo bar"`).

Every option is associated with a specific type (`bool` for flags) and trying to assign a wrong type vale (as in `--level three` when "level" is defined of type `int`) cause an error in the parsing phase.

For repeatable option it's possible to choose the type of container (`std::set`, `std::vector`, `std::array`, etc.) where to store the values; so, by example, if it's important maintain the order in command line, `std::vector` could be a good choice, otherwise `std::set` (or `std::multi_set`, if it's important maintain the multiplicity) could be good.

A minimal example can be the following (see examples/yarg-example-001.cpp)

```
#include <iostream>

#include <yarg/yarg.h>

int main (int argc, char * argv[])
 {
   yarg::yarg  y;

   auto & l = y.addOpt('l', "level", "set the level value", 3);

   if ( y.parse(argc, argv) )
      std::cout << "level is " << l << std::endl;

   return EXIT_SUCCESS;
 }
```

Created a `yarg::yarg` instance, its possible to set an option of type `int`, the type of the default value (`3`), receiving the reference of the variable that is created to store the corresponding value.

It's possible set the level with 

```
 -l=<int>
 -l <int>
 --level=<int>
 --level <int>
```

After the parse, in variable `l` contain the value received from `argc`/`argv` or the default value.

A more complete example can be seen in examples/yarg-example-002.cpp
