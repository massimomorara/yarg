
// Copyright (c) 2016, 2017 massimo morara

// Distributed under the Mit License
// See accompanying file LICENSE
// See https://github.com/massimomorara/yarg for latest version

#include "yarg/yarg.h"

#include <set>
#include <array>
#include <queue>
#include <stack>
#include <regex>
#include <iostream>
#include <forward_list>
#include <unordered_set>

#include "catch.hpp"

class date
 {
   private:
      int y, m, d;

   public:
      date (int y0 = 1, int m0 = 1, int d0 = 1)
         : y{y0}, m{m0}, d{d0}
          { }

      friend bool operator== (date const & d0, date const & d1)
       { return (d0.y == d1.y) && (d0.m == d1.m) && (d0.d == d1.d); }

      friend std::istream & operator>> (std::istream & is, date & dt)
       {
         char ch1, ch2;

         is >> dt.y >> ch1 >> dt.m >> ch2 >> dt.d;

         if (    (1 > dt.d) || (31 < dt.d) || (1 > dt.m) || (12 < dt.m)
              || (ch1 != ch2) || ('.' != ch1) )
            is.setstate(std::ios_base::failbit);

         return is;
       }
 };


TEST_CASE ( "yarg::opt tests", "[yarg::opt]" )
 {
   SECTION("opt of bool")
    {
      yarg::opt<bool>  ob1;

      CHECK ( 0U    == ob1.getDim() );
      CHECK ( false == ob1.getVal() );

      ob1.setVal("1");

      CHECK ( 1U   == ob1.getDim() );
      CHECK ( true == ob1.getVal() );

      ob1.setVal(std::string("0"));

      CHECK ( 1U    == ob1.getDim() );
      CHECK ( false == ob1.getVal() );

      CHECK_THROWS_AS ( ob1.setVal("foo"), std::runtime_error );

      yarg::opt<bool>  ob2 { true };

      CHECK ( 1U   == ob2.getDim() );
      CHECK ( true == ob2.getVal() );

      ob2.setBool(false);

      CHECK ( 1U    == ob2.getDim() );
      CHECK ( false == ob2.getVal() );

      ob2.setBool(true);

      CHECK ( 1U   == ob2.getDim() );
      CHECK ( true == ob2.getVal() );

      ob2.setVal("0");

      CHECK ( 1U    == ob2.getDim() );
      CHECK ( false == ob2.getVal() );
    }

   SECTION("opt of int")
    {
      yarg::opt<int>  oi1;

      CHECK ( 0U == oi1.getDim() );
      CHECK ( 0  == oi1.getVal() );

      oi1.setVal("12");

      CHECK ( 1U == oi1.getDim() );
      CHECK ( 12 == oi1.getVal() );

      oi1.setVal(std::string("-17"));

      CHECK ( 1U  == oi1.getDim() );
      CHECK ( -17 == oi1.getVal() );

      CHECK_THROWS_AS ( oi1.setVal("13.5"), std::runtime_error );

      yarg::opt<int>  oi2 { -23 };

      CHECK ( 1U  == oi2.getDim() );
      CHECK ( -23 == oi2.getVal() );
    }

   SECTION("opt of unsigned")
    {
      yarg::opt<unsigned>  ou1;

      CHECK ( 0U == ou1.getDim() );
      CHECK ( 0U == ou1.getVal() );

      ou1.setVal("23");

      CHECK (  1U == ou1.getDim() );
      CHECK ( 23U == ou1.getVal() );

      ou1.setVal(std::string("27"));

      CHECK (  1U == ou1.getDim() );
      CHECK ( 27U == ou1.getVal() );

      // different behaviour libstdc++ and libc++ (throw only libc++)
      //CHECK_THROWS_AS ( ou1.setVal("-15"), std::runtime_error );
 
      yarg::opt<unsigned>  ou2 { 41U };

      CHECK (  1U == ou2.getDim() );
      CHECK ( 41U == ou2.getVal() );
    }

   SECTION("opt of std::string")
    {
      yarg::opt<std::string>  os1;

      CHECK ( 0U == os1.getDim() );
      CHECK ( "" == os1.getVal() );

      os1.setVal("test-string-001");

      CHECK (                1U == os1.getDim() );
      CHECK ( "test-string-001" == os1.getVal() );

      os1.setVal("test string 002");

      CHECK (                1U == os1.getDim() );
      CHECK ( "test string 002" == os1.getVal() );

      os1.setVal(std::string("test string 003"));

      CHECK ( "test string 003" == os1.getVal() );

      yarg::opt<std::string>  os2 { "test string 004" };

      CHECK (                1U == os2.getDim() );
      CHECK ( "test string 004" == os2.getVal() );
    }

   SECTION("opt of date")
    {
      yarg::opt<date>  od1;

      CHECK (            0U == od1.getDim() );
      CHECK ( date(1, 1, 1) == od1.getVal() );

      od1.setVal("2015.2.23");

      CHECK (                1U == od1.getDim() );
      CHECK ( date(2015, 2, 23) == od1.getVal() );

      od1.setVal(std::string("2013.5.12"));

      CHECK (                1U == od1.getDim() );
      CHECK ( date(2013, 5, 12) == od1.getVal() );

      CHECK_THROWS_AS ( od1.setVal("2015.13.23"), std::runtime_error );
      CHECK_THROWS_AS ( od1.setVal("2015.12.0"), std::runtime_error );
      CHECK_THROWS_AS ( od1.setVal("2015.12/11"), std::runtime_error );
      CHECK_THROWS_AS ( od1.setVal("2015.12.11a"), std::runtime_error );
      CHECK_THROWS_AS ( od1.setVal("2015.12.11 "), std::runtime_error );

      yarg::opt<date>  od2 { date(2011, 7, 6) };

      CHECK (               1U == od2.getDim() );
      CHECK ( date(2011, 7, 6) == od2.getVal() );
    }
 }

TEST_CASE ( "yarg::optC tests", "[yarg::optC]" )
 {
   SECTION("opt of std::set<short> (insert add mode)")
    {
      yarg::optC<std::set<short>>  oss1;

      CHECK ( 0U == oss1.getDim() );

      oss1.setVal("23");
      oss1.setVal("7");
      oss1.setVal("19");
      oss1.setVal("7");
      oss1.setVal("-103");

      CHECK ( 4U == oss1.getDim() );

      CHECK ( operator==(oss1.getVal(), { 7, -103, 23, 19, 23 }) );

      yarg::optC<std::set<short>>  oss2 { { 19, 13, 19, -115} };

      CHECK ( 3U == oss2.getDim() );

      CHECK ( operator==(oss2.getVal(), { -115, 13, 19, 13 }) );
    }

   SECTION("opt of std::vector<int> (push_back add mode)")
    {
      yarg::optC<std::vector<int>>  ovi1;

      CHECK ( 0U == ovi1.getDim() );

      ovi1.setVal("33");
      ovi1.setVal("-7");
      ovi1.setVal("29");
      ovi1.setVal("-13");
      ovi1.setVal("29");

      CHECK ( 5U == ovi1.getDim() );

      CHECK ( operator==(ovi1.getVal(), { 33, -7, 29, -13, 29 }) );

      yarg::optC<std::vector<int>>  ovi2 { { -91, 12, 65, -91 } } ;

      CHECK ( 4U == ovi2.getDim() );

      CHECK ( operator==(ovi2.getVal(), { -91, 12, 65, -91 }) );
    }

   SECTION("opt of std::queue<long> (push add mode)")
    {
      yarg::optC<std::queue<long>>  oql1;

      CHECK ( 0U == oql1.getDim() );

      oql1.setVal("27");
      oql1.setVal("-12");
      oql1.setVal("126");
      oql1.setVal("-13");
      oql1.setVal("126");

      CHECK ( 5U == oql1.getDim() );

      CHECK ( operator==(oql1.getVal(), 
                         std::queue<long>(
                            { 27L, -12L, 126L, -13L, 126L })) );

      yarg::optC<std::queue<long>>  oql2
       { std::queue<long>({ 23L, -7L, 123L, -7L }) };

      CHECK ( 4U == oql2.getDim() );

      CHECK ( operator==(oql2.getVal(), 
                         std::queue<long>({ 23L, -7L, 123L, -7LL })) );
    }

   SECTION("opt of std::forward_list<long long> (push_front add mode)")
    {
      yarg::optC<std::forward_list<long long>>  oflll1;

      CHECK ( 0U == oflll1.getDim() );

      oflll1.setVal("13");
      oflll1.setVal("-142");
      oflll1.setVal("45");
      oflll1.setVal("-23");
      oflll1.setVal("-142");

      CHECK ( 5U == oflll1.getDim() );

      CHECK ( operator==(oflll1.getVal(),
                         { -142LL, -23LL, 45LL, -142LL, 13LL }) );

      yarg::optC<std::forward_list<long long>>  oflll2
       { { -45LL, 45LL, 23LL, -12LL } };

      CHECK ( 4U == oflll2.getDim() );

      CHECK ( operator==(oflll2.getVal(), { -45LL, 45LL, 23LL, -12LL }) );
    }

   SECTION("opt of std::deque<unsigned short> (push_back)")
    {
      yarg::optC<std::deque<unsigned short>>  odqus1;

      CHECK ( 0U == odqus1.getDim() );

      odqus1.setVal("17");
      odqus1.setVal("91");
      odqus1.setVal("74");
      odqus1.setVal("718");
      odqus1.setVal("91");

      CHECK ( 5U == odqus1.getDim() );

      CHECK ( operator==(odqus1.getVal(), { 17U, 91U, 74U, 718U, 91U }) );

      yarg::optC<std::deque<unsigned short>>  odqus2
       { { 12U, 23U, 12U, 99U } };

      CHECK ( 4U == odqus2.getDim() );

      CHECK ( operator==(odqus2.getVal(), { 12U, 23U, 12U, 99U }) );
    }

   SECTION("opt of std::multiset<unsigned> (insert)")
    {
      yarg::optC<std::multiset<unsigned>>  omsu1;

      CHECK ( 0U == omsu1.getDim() );

      omsu1.setVal("51");
      omsu1.setVal("97");
      omsu1.setVal("72");
      omsu1.setVal("5");
      omsu1.setVal("72");

      CHECK ( 5U == omsu1.getDim() );

      CHECK ( operator==(omsu1.getVal(), { 72U, 5U, 97U, 51U, 72U }) );

      yarg::optC<std::multiset<unsigned>>  omsu2 { { 19U, 23U, 45U, 23U } };

      CHECK ( 4U == omsu2.getDim() );

      CHECK ( operator==(omsu2.getVal(), { 23U, 19U, 45U, 23U }) );
    }

   SECTION("opt of std::unordered_set<unsigned long> (insert)")
    {
      yarg::optC<std::unordered_set<unsigned long>>  ousul1;

      CHECK ( 0U == ousul1.getDim() );

      ousul1.setVal("41");
      ousul1.setVal("57");
      ousul1.setVal("22");
      ousul1.setVal("7");
      ousul1.setVal("22");

      CHECK ( 4U == ousul1.getDim() );

      CHECK ( operator==(ousul1.getVal(), { 57UL, 22UL, 7UL, 41UL, 7UL }) );

      yarg::optC<std::unordered_set<unsigned long>>  ousul2 
       { { 91UL, 33UL, 91UL, 5UL } };

      CHECK ( 3U == ousul2.getDim() );

      CHECK ( operator==(ousul2.getVal(), { 5UL, 91UL, 5UL, 33UL }) );
    }

   SECTION("opt of std::unordered_multiset<unsigned long long> (insert)")
    {
      yarg::optC<std::unordered_multiset<unsigned long long>>  oumsull1;

      CHECK ( 0U == oumsull1.getDim() );

      oumsull1.setVal("67");
      oumsull1.setVal("19");
      oumsull1.setVal("81");
      oumsull1.setVal("8");
      oumsull1.setVal("19");

      CHECK ( 5U == oumsull1.getDim() );

      CHECK ( operator==(oumsull1.getVal(),
                         { 19ULL, 67ULL, 19ULL, 81ULL, 8ULL }) );

      yarg::optC<std::unordered_multiset<unsigned long long>>  oumsull2
       { { 41ULL, 92ULL, 41ULL, 21ULL } };

      CHECK ( 4U == oumsull2.getDim() );

      CHECK ( operator==(oumsull2.getVal(),
                         { 92ULL, 41ULL, 21ULL, 41ULL }) );
    }

   SECTION("opt of std::stack<std::string> (push)")
    {
      yarg::optC<std::stack<std::string>>  oss1;

      CHECK ( 0U == oss1.getDim() );

      oss1.setVal("abcd");
      oss1.setVal("dcba");
      oss1.setVal("12 34");
      oss1.setVal("abcd");
      oss1.setVal("34 12");

      CHECK ( 5U == oss1.getDim() );

      CHECK ( operator==(oss1.getVal(),
                         std::stack<std::string>(
                            { "abcd", "dcba", "12 34", "abcd", "34 12" })) );

      yarg::optC<std::stack<std::string>>  oss2
       { std::stack<std::string>({ "23ab", "c34d", "zz12", "34 cd" }) };

      CHECK ( 4U == oss2.getDim() );

      CHECK ( operator==(oss2.getVal(),
                         std::stack<std::string>(
                            { "23ab", "c34d", "zz12", "34 cd" })) );
    }

   SECTION("opt of std::priority_queue<char> (push)")
    {
      yarg::optC<std::priority_queue<char>>  opqc1;

      CHECK ( 0U == opqc1.getDim() );

      opqc1.setVal("Z");
      opqc1.setVal("m");
      opqc1.setVal("5");
      opqc1.setVal("#");
      opqc1.setVal("5");

      CHECK ( 5U == opqc1.getDim() );

      std::vector<char>  vc;

      vc.reserve(opqc1.getVal().size());

      for ( ; false == opqc1.getVal().empty() ; opqc1.getVal().pop() )
         vc.push_back(opqc1.getVal().top());

      CHECK ( 0U == opqc1.getDim() );

      CHECK ( operator==(vc, {'m', 'Z', '5', '5', '#'}) );

      std::priority_queue<char> pqc;

      pqc.push('3');
      pqc.push('d');
      pqc.push('A');
      pqc.push('d');

      yarg::optC<std::priority_queue<char>>  opqc2 { pqc };

      CHECK ( 4U == opqc2.getDim() );

      vc.clear();

      for ( ; false == opqc2.getVal().empty() ; opqc2.getVal().pop() )
         vc.push_back(opqc2.getVal().top());

      CHECK ( 0U == opqc2.getDim() );

      CHECK ( operator==(vc, {'d', 'd', 'A', '3'}) );
    }

   SECTION("opt of std::array<unsigned char, 5>")
    {
      yarg::optC<std::array<unsigned char, 6U>>  oauc1;

      CHECK ( 0U == oauc1.getDim() );

      CHECK ( operator==(oauc1.getVal(), { {0, 0, 0, 0, 0, 0} }) );

      oauc1.setVal("v");
      oauc1.setVal("X");
      oauc1.setVal("7");
      oauc1.setVal("@");
      oauc1.setVal("7");

      CHECK ( 5U == oauc1.getDim() );

      CHECK ( operator==(oauc1.getVal(),
                         { {'v', 'X', '7', '@', '7'} }) );
      CHECK ( operator==(oauc1.getVal(),
                         { {'v', 'X', '7', '@', '7', 0} }) );

      oauc1.setVal("4");

      CHECK_THROWS_AS ( oauc1.setVal("d"), std::out_of_range );

      yarg::optC<std::array<unsigned char, 5U>>  oauc2
       { { { 'd', 'g', '3', 'e' } }, 3U };

      CHECK ( 3U == oauc2.getDim() );

      CHECK ( operator==(oauc2.getVal(), { {'d', 'g', '3', 'e'} }) );
      CHECK ( operator==(oauc2.getVal(), { {'d', 'g', '3', 'e', 0} }) );

      oauc2.setVal("h");

      CHECK ( 4U == oauc2.getDim() );

      CHECK ( operator==(oauc2.getVal(), { {'d', 'g', '3', 'h'} }) );
      CHECK ( operator==(oauc2.getVal(), { {'d', 'g', '3', 'h', 0} }) );

      oauc2.setVal("9");

      CHECK_THROWS_AS ( oauc2.setVal("z"), std::out_of_range );
    }
 }

TEST_CASE ( "yarg::yarg tests", "[yarg::yarg]" )
 {
   SECTION("test 001")
    {
      yarg::yarg  y0;

      auto & oi0 = y0.addOpt('l', "level", "some level info", 3);
      auto & os0 = y0.addOpt<std::string>(0, "name", "some name info",
                                          "def name");

      auto & f0 = y0.addFlag('h', "help", "some help");
      auto & f1 = y0.addFlag(0, "verbose", "verbose output", true);
      auto & a0 = y0.getNoOpts();


      char const * const av0[]
       { "parseTest", "-lh", "5", "--name", "foo", "--verbose", "no-opts-1",
         "no-opts-2" };

      CHECK ( true == y0.parse(sizeof(av0)/sizeof(av0[0]), av0) );

      CHECK (     5 == oi0 );
      CHECK ( "foo" == os0 );
      CHECK (  true == f0 );
      CHECK (  true == f1 );

      CHECK ( operator==(a0, { "no-opts-1", "no-opts-2" }) );
    }
 }

