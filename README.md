
# Yarg

## Yet Another Reinvented Getotp

Yarg is a one-header-only C++11 object oriented replacement for `getopt()`.

Support long options (example: `--help`), short options (`-h`), more short options in sequence (`-hvl`), options without values (flags) or followed by a value, negative flags (`--verbose` and `--noverbose` bounded on the same flag), repeatable options (`--name foo --name=bar --name "foo bar"`) and the special `--` argument to stop the option recognition.

Every option is associated with a specific type (`bool` for flags) and trying to assign a wrong type value (as in `--level three` when "level" is defined of type `int`) cause an error in the parsing phase.

For repeatable options it's possible to choose the type of container (`std::set`, `std::vector`, `std::array`, etc.) where to store the values; so, by example, if it's important maintain the order expressed in the command line, `std::vector` could be a good choice, otherwise `std::set` (or `std::multi_set`, if it's important maintain the multiplicity) could be good.

## Minimal example

A minimal example can be the following (see `examples/yarg-example-001.cpp`)

```c++
#include <iostream>

#include <yarg/yarg.h>

int main (int argc, char * argv[])
 {
   yarg::yarg  y;

   // Attention: l is a *reference*
   auto & l = y.addOpt('l', "level", "set the level value", 3);

   if ( y.parse(argc, argv) )
      std::cout << "level is " << l << std::endl;

   return EXIT_SUCCESS;
 }
```

Other examples in the `examples/` directory.

## Usage

### Header Inclusion

```c++
#include <yarg/yarg.h>
```

### Yarg object instantiation

```c++
yarg::yarg  y;
```

### Options declaration interface

```c++
// flag (option without value) declaration
bool & addFlag (char shortOption, std::string const & longOption,
                std::string const & description = "", bool revert = false,
                bool defaultValue = false);

// option (with value) declaration
template <typename T>
T & addOpt (char shortOption, std::string const & longOption,
            std::string const & description = "",
            T const &  defaultValue = T());

// repeteable flag declaration (Cont is a container of bool)
template <typename Cont>
Cont & addFlagCont (char shortOption, std::string const & longOption,
                    std::string const & description = "",
                    bool revert = false, Cont const & defaultValue = Cont());

// repeteable option declaration (Cont is a container of values)
template <typename Cont>
Cont & addOptCont (char shortOptions, std::string const & longOptions,
                   std::string const & description = "",
                   Cont const & defaultValue = Cont());
```

These methods of `yarg::yarg` return a reference to the internal variable with the default value that can be modified in following parsing phase if the corresponding option is encountered.

The single value method (`addOpt()`) is intended to work with `bool`, all standard integer types (both `signed` and `unsigned` versions of `short int`, `int`, `long int` and `long int int`), `std::string` and should work with every type with input `operator<<()` defined.

The multiple value containers versions (`addFlagCont()` and `addOptCont()`) are intended to work with `std::vector`, `std::array`, `std::set`, `std::multiset`, `std::unordered_set`, `std::unordered_multiset`, `std::deque`, `std::queue`, `std::priority_queue`, `std::forward_list` and `std::stack` and should work with every container type for single values (so `std::map` and similar double value containers are excluded) with a `insert()` or `push()` or `push_back()` or `push_front()` method.

Argument description:

+ **shortOption** - a single char that set the short option; by example, pass `'h'` to activate the `-h` recognition; to exclude the use of short option, pass `0` (zero)
+ **longOption** - a string that set the long option, excluding the starting `--`; by example, pass `"help"` to activate `--help` recognition; to exclude the use of long option, pass `""` (empty string); if **revert** is `true` (only for flags) this value activate a double long option (see **revert**)
+ **description** - a human understandable description of the option; an example: "show this help and exit"; intended for a future `getHelp()` method
+ **revert** - only for flags (`addFlag()` and `addFlagCont()`); if `true` and if a not empty **longOption** (see) is set, activate another long option (pre posing "no") to intercept a negative value; by example, if `true` and the long option is `"verbose"`, set to `true` the flag (or add a `true` flag in the container, when repeatable) when (a) `--verbose` argument is encountered and set to `false` the same flag (o add a `false` flag in the container, when repeatable) when (a) `--noverbose` argument is encountered
+ **defaultValue** - default value for the flag (or option) value (or container of values); can be used to set the template type (not in `addFlag()`, where is fixed to `bool`), otherwise is necessary to explicit it (se "option declaration examples")

### Option declaration examples

```c++
auto & v = y.addFlag ('h', "help", "show this helps and exit");
```

Return a reference to a `bool` that is set to `true` if the parsing phase encounter a `-h` or a `--help` argument; otherwise remain `false` (the default value); observe that `v` is a **reference** to `auto` (that become a reference to a `bool`) because must be bounded to the value, internal to the `yarg::yarg y` instance, where the flag is stored.


```c++
auto & l = y.addOpt('l', "level", "set the level value", 3);
```

Return a reference to a `int` that is set to the value of the `-l` (or `--level`) option; without a `-l` or `--level`, `l` remain with the initial default value, `3`; observe that the type of `l` is determined by the type of the default value (`3` is an `int`, in this example).

```c++
auto & l = y.addOpt<long>('l', "level", "set the level value");
```

Like in preceding example but the default value isn't expressed (so it's `T()`, that is `0L` is this example); observe that, without the default value, you have to explicit (the `<long>` part after `addOpt`) the template type (the type of `l`).


```c++
auto & flagsV = y.addFlagCont<std::vector<bool>>(0, "flag", "a repeteable boolean flag", true);
```

Return a reference to a `std::vector` of `bool` where are stored the values corresponding to a repeatable and reversible flags; so when a `--flag` argument is encountered in the parsing phase, a `true` is added (`push_back(true)`) at the end of `flagsV`; when a `--noflag` is encountered in the parsing phase, a `false` value is added at the end of `flagsV`; observe that using a `std::vector` the order of the `--flags`/`--noflags` arguments in the command line is preserved.


```c++
Cont & namesS = y.addOptCont<std::set<std::string>>('n', "--name", "set a name (repeteable)");
```

Return a reference to a `std::set` of `std::string` where are stored the values corresponding to a repeatable option; so when a `-n <value>`, or `--name <value>`, is encountered in the parsing phase, the string value is added (`insert(value)`) in `namesS`; observe that using a `std::set` the order and the multiplicity of the values in the command line isn't preserved.


### Parsing

Given a `main ()` declared as 

```c++
int main (int argc, char * argv[])
```

instantiated a `y` `yarg::yarg` object and declared some flags, you can parse the command line arguments, extracting the options, simply with

```c++
y.parse(argc, argv)
```

The `parse()` method return `true` when the parsing phase conclude with success, return `false` otherwise (in case of error).

In case of error, the method `getParserError()` return a human readable `std::string` with informations about the actual error.

So the parsing can be done as follows

```c++
if ( ! y.parse(argc, argv) )
   throw std::runtime_error("yarg parsing error: " + y.getParserError());
```

### Program name

After the parsing phase (without errors) you can get the name of the program (the classic `argv[0]` argument) using the method `getArgv0()`, that return a `std::string`.

### Not options arguments

After the parsing phase (without errors) you can get the not options values arguments list using the method `getNoOpts()` that return a `std::deque` of `std::string`.

### Line command examples

Calling the executable obtained from the minimal example that you can see in "Minimale example" section (that intercept only a `-l`/`--level` integer option) with the following command line

```
./example --level 5 name1 "name 2" name3
```

the `l` level value is set to `5`, the `getArgv0()` method return `./example` and `getNoOpts()` return a `std::deque` containing three strings: `name1`, `name 2` and `name3`.

Calling it with 

```
./example -- --level 5 name1 "name 2" name3
```

the `l` level value remain set to `3` (the default value) because the special `--` argument stop the option recognition; so `getNoOpts()` return a `std::deque` containing **five** strings: `--level`, `5`, `name1`, `name 2` and `name3`.

Calling it with 

```
./example --level five name1 "name 2" name3
```

the parsing phase fail and the `parse()` method return `false` because the `five` value is incompatible with the `--level` option that is defined of type `int`.

