
// Copyright (c) 2016-2018 massimo morara

// Distributed under the Mit License
// See accompanying file LICENSE
// See https://github.com/massimomorara/yarg for latest version

#include <iostream>

#include <yarg/yarg.h>

int main (int argc, char * argv[])
 {
   yarg::yarg  y;

   // Attention: l is a *reference*
   auto & l = y.addOpt('l', "level", "set the level value", 3);

   if ( y.parse(argc, argv) )
      std::cout << "level is " << l << std::endl;
   else
    {
      std::cerr << std::endl << y.getParserError() << std::endl;

      y.usage();
    }


   return EXIT_SUCCESS;
 }

