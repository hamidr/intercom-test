#include "../includes/module/base_process.h"
#include "../includes/module/echo.h"
#include "../includes/module/reverse.h"
#include "../includes/module/delay.h"
#include "../includes/module/noop.h"


TEST_CASE("An interface to module processes", "[base_process]")
{
  struct compile_me : Ableton::base_process {
  protected:
    std::string process(std::string val) const override {
      return val;
    }
  };

  compile_me obj;
  Ableton::module::noop obj2;
  REQUIRE(obj.chain_call("hello world") == "hello world");
  REQUIRE(obj2.chain_call("hello world") == obj2.chain_call("hello world"));
}


TEST_CASE("Echo module", "[ableton::module::echo]")
{
  Ableton::module::echo obj;
  REQUIRE(obj.chain_call("hello world") == "hello worldhello world");
  REQUIRE(obj.chain_call("hello world") != "hello world hello world");
}


TEST_CASE("Delay module", "[ableton::module::delay]")
{
  Ableton::module::delay obj;
  REQUIRE(obj.chain_call("test") == "hello test");
  REQUIRE(obj.chain_call("test") != "test");
}


TEST_CASE("Noop module", "[ableton::module::noop]")
{
  Ableton::module::noop obj;
  REQUIRE(obj.chain_call("test") == "test");
}

TEST_CASE("Reverse module", "[ableton::module::reverse]")
{
  Ableton::module::reverse obj;
  REQUIRE(obj.chain_call("test") == "tset");
  REQUIRE(obj.chain_call("hello world") == "olleh dlrow");
  REQUIRE(obj.chain_call(" hello world ") == " olleh  dlrow");
}
