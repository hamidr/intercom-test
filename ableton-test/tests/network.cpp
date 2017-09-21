#include "../includes/network.h"

#include <sstream>

using std::string;

class Ableton::network_test
{
  Ableton::network& net_;

public:
  network_test(Ableton::network& net)
    : net_(net)
  {}

  ssize_t size() const {
    return net_.amodules_.size();
  }

  const Ableton::amodule& head() {
    return net_.head_;
  }

  const Ableton::amodule access_module(const string& name) const {
    try {
      return net_.amodules_.at(name);
    } catch(...) {
      return nullptr;
    }
  }
};


SCENARIO("Ableton Network Modules", "[ableton::network]")
{

  GIVEN("An Abletonian network module") {
    std::ostringstream output;

    Ableton::network network(output);
    Ableton::network_test net_test(network);

    REQUIRE(net_test.size() == 0);
    REQUIRE(net_test.head() == nullptr);

    REQUIRE(output.str() == "");

    WHEN("Defining a module in the network") {
      REQUIRE(net_test.access_module("alpha") == nullptr);

      network.interpret_line("module alpha echo");
      REQUIRE(net_test.access_module("alpha") != nullptr);

      REQUIRE(net_test.head() == net_test.access_module("alpha"));
      REQUIRE(net_test.access_module("beta") == nullptr);
      REQUIRE(output.str() == "");

      network.interpret_line("module beta echoo");
      REQUIRE(net_test.access_module("beta") == nullptr);
      REQUIRE(net_test.size() == 1);
      REQUIRE(output.str() == "ERROR: Undefined type!\n");

      network.interpret_line("module beta echo");
      REQUIRE(net_test.access_module("beta") != nullptr);
      REQUIRE(net_test.size() == 2);


      THEN("Process before connect") {
        output.str("");
        REQUIRE(net_test.head() != nullptr);

        network.interpret_line("process hello world");
        REQUIRE(output.str() == "hello worldhello world\n");
      }
    }

    WHEN("Connect two modules") {
      //Same example in the description
      network.interpret_line("module alpha reverse");
      network.interpret_line("module beta delay");
      network.interpret_line("connect alpha beta");

      REQUIRE(net_test.access_module("alpha") != nullptr);
      REQUIRE(net_test.access_module("beta") != nullptr);
      REQUIRE(output.str() == "");

      network.interpret_line("process hello world");
      REQUIRE(output.str() == "hello olleh dlrow\n");
    }

    WHEN("Recursive connect catch") {
      //Same example in the description
      network.interpret_line("module alpha reverse");
      network.interpret_line("module beta delay");
      network.interpret_line("connect alpha beta");
      network.interpret_line("connect beta alpha");

      REQUIRE(net_test.access_module("alpha") != nullptr);
      REQUIRE(net_test.access_module("beta") != nullptr);
      REQUIRE(output.str() == "ERROR: Recursive call! Smart! :)\n");
      output.str("");

      network.interpret_line("process hello world");
      REQUIRE(output.str() == "hello olleh dlrow\n");
    }

    WHEN("Use multiple modules") {
      //Same example in the description
      network.interpret_line("module a reverse");
      network.interpret_line("module b delay");
      network.interpret_line("module c noop");
      network.interpret_line("module d echo");
      network.interpret_line("connect a b");
      network.interpret_line("connect b c");
      network.interpret_line("connect c d");
      output.str("");

      REQUIRE(net_test.size() == 4);

      network.interpret_line("process hello world");
      REQUIRE(output.str() == "hello olleh dlrowhello olleh dlrow\n");
    }
  }
}

