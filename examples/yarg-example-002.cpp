
// Copyright (c) 2016 massimo morara

// Distributed under the Mit License
// See accompanying file LICENSE
// See https://github.com/massimomorara/yarg for latest version

#include <iostream>

#include <yarg/yarg.h>

int main (int argc, char * argv[])
 {
   bool  errExit { true };

   try
    {
      // creation of a yarg instance; no parameter for constructor
      yarg::yarg  y;

      // add a flag (an option without an argument) that can be set with a
      // short option ("-h") and with a long option ("--help") without
      // expliciting a default value (so the default default value is
      // "false")
      auto & b0 = y.addFlag('h', "help", "some help");

      // add another flag that can be set with a long option only
      // ("--verbose") and that can be reverted (accept "--noverbose") (first
      // "true"; default is "false") and with an explicit default value
      // (second "true")
      auto & b1 = y.addFlag(0, "verbose", "verbose output", true, true);

      // add an integer option (with argument) that can be set with a short
      // option ("-l[ |=]<value>") or a long option ("--level[ |=]<value>");
      // the value is initalized with a default value (3, in this case) that
      // set the type of the argument
      auto & i0 = y.addOpt('l', "level", "set the level value", 3);

      // add another option but of type std::string (according the default
      // value "foo" of type std::string)
      auto & s0 = y.addOpt('n', "name", "set the name", std::string("foo"));

      // add another option of type long expliciting the type (via template
      // argument); the default value is long{} (so 0L); this option can be
      // set only by a short argument (the second parameter is empty)
      auto & l0 = y.addOpt<long>('o', "", "set a long option only");

      // add a container option (a repeatable option) of type int and store
      // the values in a `std::vector<int>`
      auto & vi0 = y.addOptCont<std::vector<int>>('r', "repeat",
                                                  "a repeatable option");

      // intercept the argv[0] value, in a std::string; can be done post
      // parse()
      auto & argv0 = y.getArgv0();

      // intercept the no option (post options) values, in a
      // std::deque<std::string>; can be done post parse()
      auto & args = y.getNoOpts();

      // parse argc and argv; in case of error return false, the parsing
      // error (a std::string) can be extract from getParserError()
      if ( false == y.parse(argc, argv) )
         throw std::runtime_error("yarg parsing error: " + y.getParserError()
                                  + "]");

      // the name of the program
      std::cout << "argv0 ?   " << argv0 << std::endl;

      // the values (extracted or default)
      std::cout << "help ?    " << b0 << std::endl;
      std::cout << "verbose ? " << b1 << std::endl;
      std::cout << "level ?   " << i0 << std::endl;
      std::cout << "name ?    " << s0 << std::endl;
      std::cout << "long ?    " << l0 << std::endl;

      std::cout << "repeat ? ";

      for ( auto const & i : vi0 )
         std::cout << ' ' << i;

      std::cout << std::endl;

      std::cout << "args ?   ";

      for ( auto const & s : args )
         std::cout << ' ' << s;

      std::cout << std::endl;

      errExit = false;
    }
   catch ( std::exception & e )
    {
      std::cerr << "\nmain(): standard exception of type \""
         << typeid(e).name() <<"\"\n"
         << "  ---> " << e.what() << " <---\n" << std::endl;
    }
   catch ( ... )
    {
      std::cerr << "\nmain(): *not* standard exception\n" << std::endl;
    }

   return ( errExit ? EXIT_FAILURE : EXIT_SUCCESS );
 }

