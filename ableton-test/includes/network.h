#pragma once

#include <memory>
#include <string>
#include <ostream>
#include <unordered_map>

#include "module/base_process.h"

namespace Ableton
{
  using std::string;
  typedef std::shared_ptr<base_process> amodule;
  struct network_test;

  class network
  {
    friend class Ableton::network_test;

    enum CommandType {
      Process = 0,
      Module,
      Connect,
      Undefined
    };

    std::ostream& out_;
    std::shared_ptr<base_process> head_;
    std::unordered_map<string, amodule> amodules_;

  private:
    void show_error(const string&);

  public:
    network(std::ostream&);

    static CommandType fetch_command_type(string& line);
    void interpret_line(const string& line);

    void def_module(const string& name, const string& type);
    static bool match_module_args(const string& line, string& name, string& type);

    void connect(const string& such, const string& to);
    static bool match_connect_args(const string& line, string& such, string& to);

    void start_process(const string& words);
    static bool match_process_args(const string& line, string& rest_of_line);
  };

}
