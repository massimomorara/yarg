
#if 0

Copyright (c) 2016, 2017 massimo morara

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Except as contained in this notice, the names of the authors or above
copyright holders shall not be used in advertising or otherwise to promote
the sale, use or other dealings in this Software without prior written
authorization.

#endif

#ifndef __yarg_h__
#define __yarg_h__

#include <map>
#include <array>
#include <deque>
#include <regex>
#include <limits>
#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <type_traits>

// Yet Another Reinvented Getopt
namespace yarg
 {
   // see https://github.com/massimomorara/yarg for latest version
   #define  Yarg_Version_Major  0
   #define  Yarg_Version_Minor  2
   #define  Yarg_Version_Patch  0

   template <typename X>
   X extractVal (std::string const & str)
    {
      std::istringstream iss { str };

      X val; 

      iss >> val;

      if ( iss.fail() )
         throw std::runtime_error {
              "extractVal(): error reading val [" + iss.str() + "] (from ["
            + str + "])" };

      if ( iss.good() )
       {
         std::string  left { iss.str().substr(iss.tellg()) };

         if ( false == left.empty() )
            throw std::runtime_error {
                 "extractVal(): no empty string left [" + left + "] (from ["
               + str + "])" };
       }

      return val;
    }

   template <>
   std::string extractVal (std::string const & str)
    { return str; }

   struct optBase
    { 
      virtual void        setVal  (std::string const &) = 0;
      virtual void        setBool (bool)                = 0;
      virtual std::size_t getDim  () const              = 0;
    };

   template <typename X>
   class opt : public optBase
    {
      private:

         X    val { };
         bool set { false };

      public:

         opt ()
          { }

         opt (X const & v0)
            : val { v0 }, set { true }
          { }

         X const & getVal () const
          { return val; }

         X & getVal ()
          { return val; }

         std::size_t getDim () const final
          { return set; }

         void setVal (std::string const & str) final
          { val = extractVal<X>(str); set = true; }

         template <typename Y = void>
         typename std::enable_if<
               true == std::is_same<X, bool>::value, Y>::type
            setBoolT (bool b)
          { val = b; set = true; }

         template <typename Y = void>
         typename std::enable_if<
               false == std::is_same<X, bool>::value, Y>::type
            setBoolT (bool)
          { }

         void setBool (bool b) final
          { setBoolT(b); }
    };

   struct emptyStruct
    { };

   template <typename ... Ts>
   struct funcType;

   template <typename T, typename ... Ts>
   struct funcType<T, Ts...>
    { using type
         = typename std::conditional<T::result,
              typename T::type, typename funcType<Ts...>::type>::type; };

   template <>
   struct funcType<>
    { using type = emptyStruct; };

#define methodCheck_1(meth)                                                 \
                                                                            \
   struct helpMeth_1_##meth { };                                            \
                                                                            \
   template <typename T, typename A>                                        \
   struct isWithMethod_1_##meth                                             \
    {                                                                       \
      template<typename U>                                                  \
      static decltype(std::declval<U>().meth(std::declval<A>())) func (U*); \
                                                                            \
      template<typename U>                                                  \
      static emptyStruct func (...);                                        \
                                                                            \
      static const bool result                                              \
         = ! std::is_same<emptyStruct,                                      \
                decltype(func<T>(nullptr))>::value;                         \
                                                                            \
      using  type = helpMeth_1_##meth;                                      \
    }

#define methodCheck_0(meth)                                \
                                                           \
   struct helpMeth_0_##meth { };                           \
                                                           \
   template <typename T>                                   \
   struct isWithMethod_0_##meth                            \
    {                                                      \
      template<typename U>                                 \
      static decltype(std::declval<U>().meth()) func (U*); \
                                                           \
      template<typename U>                                 \
      static emptyStruct func (...);                       \
                                                           \
      static const bool result                             \
         = ! std::is_same<emptyStruct,                     \
                decltype(func<T>(nullptr))>::value;        \
                                                           \
      using  type = helpMeth_0_##meth;                     \
    }

   methodCheck_1(insert);
   methodCheck_1(push);
   methodCheck_1(push_back);
   methodCheck_1(push_front);

   methodCheck_0(size);

   template <typename X>
   class optC;

   template <template<typename, typename ...> class C, typename X,
             typename ... Xs>
   class optC<C<X, Xs...>> : public optBase
    {
      private:

         using addModeType2 = typename funcType<
            isWithMethod_1_push_back<C<X, Xs...>, X>,
            isWithMethod_1_insert<C<X, Xs...>, X>,
            isWithMethod_1_push<C<X, Xs...>, X>,
            isWithMethod_1_push_front<C<X, Xs...>, X>>::type;

         using sizeModeType = typename funcType<
            isWithMethod_0_size<C<X, Xs...>>>::type;

         static constexpr addModeType2  addMode { };
         static constexpr sizeModeType  sizeMode { };

         C<X, Xs...> val { };

         void addVal (std::string const & s, helpMeth_1_push_back const)
          { val.push_back(extractVal<X>(s)); }

         void addVal (std::string const & s, helpMeth_1_push const)
          { val.push(extractVal<X>(s)); }

         void addVal (std::string const & s, helpMeth_1_insert const)
          { val.insert(extractVal<X>(s)); }

         void addVal (std::string const & s, helpMeth_1_push_front const)
          { val.push_front(extractVal<X>(s)); }

         void addVal (std::string const & s, emptyStruct const)
          { throw std::runtime_error { "optC<>::addVal without mode for"
                                       " value [" + s + "]" }; }

         std::size_t getDim (helpMeth_0_size const) const
          { return val.size(); }

         std::size_t getDim (emptyStruct const) const
          { return std::distance(val.cbegin(), val.cend()); }

      public:
         optC ()
          { }

         optC (C<X, Xs...> const & v0)
            : val { v0 }
          { }

         C<X, Xs...> const & getVal () const
          { return val; }

         C<X, Xs...> & getVal ()
          { return val; }

         std::size_t getDim () const final
          { return getDim(sizeMode); }

         void setVal (std::string const & s) final
          { addVal(s, addMode); }

         void setBool (bool) final
          { }
       };

   template <typename X, std::size_t N>
   class optC<std::array<X, N>> : public optBase
    {
      private:

         std::array<X, N> val { { } };
         std::size_t      pos { 0 };

      public:
         optC ()
          { }

         optC (std::array<X, N> const & v0, std::size_t p0)
            : val { v0 }, pos { p0 }
          { }

         std::array<X, N> const & getVal () const
          { return val; }

         std::size_t getDim () const final
          { return pos; }

         std::array<X, N> & getVal ()
          { return val; }

         void setVal (std::string const & s) final
          { val.at(pos++) = extractVal<X>(s); }

         void setBool (bool) final
          { }
    };

   class yData
    {
      private:

         char const                so;
         std::string const         lo;
         bool const                opt;
         bool const                rev;

         bool                      fnd;
         std::unique_ptr<optBase>  optB;

      public:

         yData (char so0, std::string const & lo0, bool opt0, bool rev0,
                std::unique_ptr<optBase> optB0)
            : so { so0 }, lo { lo0 }, opt { opt0 }, rev { rev0 },
              fnd { false }, optB { std::move(optB0) }
          { }

         char getShortOption () const
          { return so; }

         std::string const & getLongOption () const
          { return lo; }

         bool isOption () const
          { return opt; }

         bool isReversible () const
          { return rev; }

         bool isFound () const
          { return fnd; }

         void setFound () 
          { fnd = true; }

         std::unique_ptr<optBase> const & getPnt () const
          { return optB; }
    };

   class yarg
    {
      private:

         using hRow = std::pair<std::size_t, std::string>; // rows of help

         int          nextId { std::numeric_limits<char>::max() };
         std::size_t  usageW { 79U };

         std::string  argsD;
         std::string  initialD;
         std::string  finalD;
         std::string  argv0;
         std::string  errParse;

         std::map<std::string, int>  loMap;
         std::map<std::string, int>  loNoMap;
         std::map<int, yData>        idMap;
         std::deque<std::string>     noOpts;
         std::deque<hRow>            optH;


         static void usageLine (std::size_t numW, std::string const & ur,
                                std::size_t usageW, 
                                std::ostream & os = std::cerr)
          {
            std::size_t const wdt { usageW - numW - 1U };

            if ( wdt > usageW )
               throw std::runtime_error { "usageLine(): width too short" };

            std::string        tk;
            std::istringstream iss { ur };

            for ( std::size_t pos { 0U } ; iss >> tk ; pos += tk.size() )
             {
               if ( usageW <= pos + tk.size() )
                  pos = 0U, os << '\n';

               if ( 0U == pos )
                  os << std::string(pos = numW, ' ');
               else
                  ++pos, os << ' ';

               os << tk;
             }

            os << '\n';
          }

         int prepareOption (char so, std::string const & lo,
                            bool revert = false)
          {
            int  id { int{so} };

            if ( 0 == id )
             {
               if ( lo.empty() )
                  throw std::runtime_error { "prepareOption(): no options" };

               id = ++nextId;
             }
            else if ( idMap.count(id) )
               throw std::runtime_error {   "prepareOption(): double short"
                                            " option [" + std::to_string(id)
                                          + "]" };

            if ( false == lo.empty() )
             {
               if ( loMap.count(lo) )
                  throw std::runtime_error {
                     "prepareOption(): long options collision ["
                     + std::to_string(id) + "]" };

               if ( revert )
                {
                  if ( loMap.count("no"+lo) )
                     throw std::runtime_error {
                          "prepareOption(): revert long options collision ["
                        + std::to_string(id) + "]" };

                  loNoMap.emplace("no"+lo, id);
                }

               loMap.emplace(lo, id);
             }
            else if ( revert )
               throw std::runtime_error {
                    "prepareOption(): no long options to revert ["
                  + std::to_string(id) + "]" };

            return id;
          }

         void addOptDescr (char so, std::string const & lo,
                           std::string const & descr, bool isFlag,
                           bool revert, bool repeat)
          {
            std::string  line;

            if ( so )
               line.append(1U, '-').append(1U, so);

            if ( lo.size() )
               line.append( line.size() ? ", " : "" ).append("--")
                  +=   lo + ( isFlag ? "" : " = <value>" )
                     + ( revert ? ", --no"+lo+" to negate" : "" );
            else if ( false == isFlag )
               line += " = <value>";

            if ( repeat )
               line += ", repeatable";

            optH.emplace_back(0U, "");
            optH.emplace_back(1U, line);

            if ( descr.size() )
               optH.emplace_back(3U, descr);
          }

      public:

         bool & addFlag (char so, std::string const & lo,
                         std::string const & descr = "", bool revert = false,
                         bool def = false)
          {
            int id { prepareOption(so, lo, revert) };

            opt<bool> * optP { nullptr };

            addOptDescr(so, lo, descr, true, revert, false);

            idMap.emplace(std::piecewise_construct,
                  std::forward_as_tuple(id),
                  std::forward_as_tuple(so, lo, false, revert,
                     std::unique_ptr<optBase>{ optP = new opt<bool>{def} }));

            return optP->getVal();
          }

         template <typename T>
         T & addFlagCont (char so, std::string const & lo,
                          std::string const & descr = "",
                          bool revert = false, T const & def = T{})
          {
            int id { prepareOption(so, lo, revert) };

            optC<T> * optP { nullptr };

            addOptDescr(so, lo, descr, true, revert, true);

            idMap.emplace(std::piecewise_construct,
               std::forward_as_tuple(id),
               std::forward_as_tuple(so, lo, false, revert,
                  std::unique_ptr<optBase>{ optP = new optC<T>{def} }));

            return optP->getVal();
          }

         template <typename T>
         T & addOpt (char so, std::string const & lo,
                     std::string const & descr = "", T const &  def = T{})
          {
            int id { prepareOption(so, lo) };

            opt<T> * optP { nullptr };

            addOptDescr(so, lo, descr, false, false, false);

            idMap.emplace(std::piecewise_construct,
                  std::forward_as_tuple(id),
                  std::forward_as_tuple(so, lo, true, false,
                     std::unique_ptr<optBase>{ optP = new opt<T>{def} }));

            return optP->getVal();
          }

         template <typename T>
         T & addOptCont (char so, std::string const & lo,
                         std::string const & descr = "", T const & def = T{})
          {
            int id { prepareOption(so, lo) };

            optC<T> * optP { nullptr };

            addOptDescr(so, lo, descr, false, false, true);

            idMap.emplace(std::piecewise_construct,
               std::forward_as_tuple(id),
               std::forward_as_tuple(so, lo, true, false,
                  std::unique_ptr<optBase>{ optP = new optC<T>{def} }));

            return optP->getVal();
          }

         std::string const & getArgv0 () const
          { return argv0; }

         std::deque<std::string> const & getNoOpts () const
          { return noOpts; }

         std::string const & getParserError () const
          { return errParse; }

         bool parse (int argc, char const * const * const argv)
          {
            bool  ret { false };

            try
             {
               errParse.clear();

               if ( 1 > argc )
                  throw std::runtime_error { "parse(): no args" };

               noOpts = std::deque<std::string> { argv, argv+argc };
               argv0  = noOpts.front();

               noOpts.pop_front();

               auto itMap   = idMap.begin();
               auto itLoMap = loMap.cbegin();

               bool loop    { true };
               bool revert;

               std::string  longO;
               std::string  savStrKey;
               std::string  savStrValue;

               std::deque<int>              sOpts;
               std::deque<std::ssub_match>  vssm;

               static std::string const mm { "--" };
               static std::regex const
                  rgxKey { "^--([^=]+)=(?:(\'([^\']*)\')|(\"([^\"]*)\")"
                     "|(.+))$|^--([^=]*)$" };
               static std::regex const
                  rgxValue { "^\"([^\"]*)\"$|^\'([^\']*)\'$|^(.+)$" };

               while (   loop
                      && (   (false == noOpts.empty())
                          || (false == sOpts.empty())) )
                {
                  revert = false;
                  itMap  = idMap.end();

                  vssm.clear();

                  if ( false == sOpts.empty() )
                   {
                     int  ch;

                     itMap = idMap.find( ch = sOpts.front() );

                     sOpts.pop_front();

                     if ( idMap.end() == itMap )
                        throw std::runtime_error {
                             "parse(): unrecognized short option ["
                           + (std::string()+=char(ch)) + "]" };
                   }
                  else if ( mm == noOpts.front() )
                   {
                     noOpts.pop_front();

                     loop = false;
                   }
                  else if ( std::equal(mm.cbegin(), mm.cend(),
                                       noOpts.front().cbegin()) )
                   {
                     std::smatch srgx;

                     savStrKey = noOpts.front();

                     noOpts.pop_front();

                     std::regex_search(savStrKey, srgx, rgxKey);

                     for ( std::size_t i { 1U } ; i < srgx.size() ; ++i )
                        if ( srgx[i].matched )
                           vssm.push_back(srgx[i]);

                     if ( 0U == vssm.size() )
                        throw std::runtime_error { "parse(): no key match" };

                     longO = vssm.front().str();

                     vssm.pop_front();

                     itLoMap = loMap.find(longO);

                     if ( loMap.cend() == itLoMap )
                      {
                        revert  = true;
                        itLoMap = loNoMap.find(longO);

                        if ( loNoMap.cend() == itLoMap )
                           throw std::runtime_error {
                                "parse(): unrecognized long option [" + longO
                              + "]" };
                      }

                     itMap = idMap.find(itLoMap->second);

                     if ( idMap.end() == itMap )
                        throw std::runtime_error {
                           "parse(): long option mismatch" };
                   }
                  else if ( '-' == noOpts.front().at(0U) )
                   {
                     sOpts = std::deque<int> { noOpts.front().cbegin(),
                                               noOpts.front().cend() };

                     noOpts.pop_front();
                     sOpts.pop_front(); // remove '-'
                   }
                  else
                     loop = false;

                  if ( idMap.end() != itMap )
                   {
                     if (   (true == revert)
                         && (false == itMap->second.isReversible()) )
                        throw std::runtime_error {
                           "parse(): revert option mismatch" };

                     if (   (false == vssm.empty() )
                         && (false == itMap->second.isOption()) )
                        throw std::runtime_error {
                           "parse(): value for flag" };

                     if ( false == itMap->second.isOption() )
                        itMap->second.getPnt()->setBool( ! revert );
                     else
                      {
                        if ( vssm.empty() )
                         {
                           if ( noOpts.empty() )
                              throw std::runtime_error {
                                 "parse(): no value for param" };

                           savStrValue = noOpts.front();

                           noOpts.pop_front();

                           std::smatch srgx;

                           std::regex_search(savStrValue, srgx, rgxValue);

                           for ( std::size_t i {1U} ; i < srgx.size() ; ++i )
                              if ( srgx[i].matched )
                                 vssm.push_back(srgx[i]);
                         }

                        if ( 1U != vssm.size() )
                           throw std::runtime_error {
                                "parse(): number of values mismatch (1"
                                " awaited, " + std::to_string(vssm.size())
                              + " received)" };

                        itMap->second.getPnt()->setVal(vssm.front().str());
                      }

                     itMap->second.setFound();
                   }
                }

               ret = true;
             }
            catch ( std::exception const & exc )
             { errParse = "yarg parser error: [" + std::string{ exc.what() }
                  + "]"; }
            catch ( ... ) 
             { errParse = "yarg parser not standard"; }

            return ret;
          }

         void setUsageArgsDescr (std::string const & ad)
          { argsD = ad; }

         void setUsageFinalDescr (std::string const & fd)
          { finalD = fd; }

         void setUsageInitialDescr (std::string const & id)
          { initialD = id; }

         void setUsageWidth (std::size_t uw)
          { usageW = uw; }

         void usage (std::ostream & os = std::cerr) const
          {
            os << "\n Usage: " << argv0 << (optH.empty() ? "" : " [options]")
               << ' ' << argsD << '\n';

            if ( false == initialD.empty() )
               os << '\n', usageLine(1U, initialD, usageW, os);

            if ( false == optH.empty() )
               os << "\n where options are\n";

            for ( auto const & p : optH )
               usageLine(p.first, p.second, usageW, os);

            if ( false == finalD.empty() )
               os << '\n', usageLine(1U, finalD, usageW, os);

            os << std::endl;
          }
    };
 }

#endif // ndef __yarg_h__

