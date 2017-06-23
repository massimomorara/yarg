
// Copyright (c) 2016, 2017 massimo morara

// Distributed under the Mit License
// See accompanying file LICENSE.txt
// See https://github.com/massimomorara/yarg for latest version

#include <set>
#include <iostream>

#include <yarg/yarg.h>

int main (int argc, char * argv[])
 {
   yarg::yarg  y;

   y.setUsageArgsDescr("[args]");
   y.setUsageInitialDescr("Initial (optional) description message, printed"
                          " before of the options description. Can be used"
                          " to give a short description of the program.");
   y.setUsageFinalDescr("Final (optional) description message, printed after"
                        " the options description. Can be used to give a"
                        " longer and exaustive description of the program.");

   // some flags and options with description
   y.addFlag('a', "flag0", "a flag, not reversible, not repeatable, with"
             " long and short option");
   y.addFlag('b', "", "a flag, not reversible, not repeatable, with short"
             " option only");
   y.addFlag(0, "flag2", "a flag, not reversible, not repeatable, with long"
             " option only");
   y.addFlag('d', "flag3", "a flag, reversible, not repeatable, with long"
             " and short option", true);
   y.addFlag(0, "flag4", "a flag, reversible, not repeatable, with long"
             " option only", true);
   y.addFlagCont<std::set<int>>('f', "flag5", "a flag, not reversible,"
                                " repeatable, with long and short option",
                                false);
   y.addFlagCont<std::set<int>>('g', "", "a flag, not reversible,"
                                " repeatable, with short option only",
                                false);
   y.addFlagCont<std::set<int>>(0, "flag7", "a flag, not reversible,"
                                " repeatable, with long option only", false);
   y.addFlagCont<std::set<int>>('i', "flag8", "a flag, reversible,"
                                " repeatable, with long and short option",
                                true);
   y.addFlagCont<std::set<int>>(0, "flag9", "a flag, reversible, repeatable,"
                                " with long option only", true);
   y.addOpt('k', "option0", "an option, not repeatable, with long and short"
            " option", 0);
   y.addOpt('l', "", "an option, not repeatable, with short option only", 0);
   y.addOpt(0, "option2", "an option, not repeatable, with long option only",
            0);
   y.addOptCont<std::set<int>>('n', "option3", "an option, repeatable, with"
                               " long and short option");
   y.addOptCont<std::set<int>>('o', "", "an option, repeatable, with short"
                               " option only");
   y.addOptCont<std::set<int>>(0, "option5", "an option, repeatable, with"
                               " long option only and a longer, really"
                               " longer than usual, description; and with"
                               " \"really longer than usual\", I really"
                               " intend longer");

   y.parse(argc, argv);

   std::cerr << "---- help to std::cerr, 79 chars lines" << std::endl;

   y.setUsageWidth(79U); // length of the help lines (79 is the default)

   y.usage(); // by default to std::cerr

   std::cerr << "---- help to std::cout, 59 chars lines" << std::endl;

   y.setUsageWidth(59U); // reduced length of the help lines: 59 chars only

   y.usage(std::cout); // send to std::cout

   return EXIT_SUCCESS;
 }

