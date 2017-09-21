#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

#include "network.h"
#include "module/echo.h"
#include "module/delay.h"
#include "module/reverse.h"
#include "module/noop.h"

namespace Ableton
{
  network::network(std::ostream& out)
    : out_ (out)
  { }

  network::CommandType network::fetch_command_type(string& line)
  {
    ssize_t index = 0;
    string command;

    for (auto c: line) {
      ++index;
      if (c == ' ')
        break;

      command.push_back(c);
    }

    line.erase(0, index);

    if (command == "process")
      return CommandType::Process;

    if (command == "connect")
      return CommandType::Connect;

    if (command == "module")
      return CommandType::Module;

    return CommandType::Undefined;
  }

  std::vector<string> split(const string& chars) {
    std::istringstream iss(chars);
    string s;

    std::vector<string> words;

    while ( getline( iss, s, ' ' ) )
      words.push_back(s);

    return words;
  }

  string& ltrim(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::ptr_fun<int, int>(std::isgraph)));
    return s;
  }

  string& rtrim(string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::ptr_fun<int, int>(std::isgraph)).base(), s.end());
    return s;
  }

  string& trim(string& s) {
    return ltrim(rtrim(s));
  }

  bool network::match_module_args(const string& line, string& name, string& type)
  {
    auto words = split(line);

    if (words.size() != 2)
      return false;

    name = words[0];
    type = words[1];

    return true;
  }

  bool network::match_process_args(const string& line, string& str)
  {
    if (line.size() == 0)
      return false;

    str = line;
    return true;
  }

  bool network::match_connect_args(const string& line, string& such, string& to)
  {
    auto words = split(line);
    if (words.size() != 2)
      return false;

    such = words[0];
    to   = words[1];
    return true;
  }

  void network::interpret_line(const string& mline)
  {
    string line = mline;
    trim(line);

    switch(network::fetch_command_type(line))
    {
      case CommandType::Process:
      {
        string rest_of_line;
        if (network::match_process_args(line, rest_of_line))
          return this->start_process(rest_of_line);
      }
      break;

      case CommandType::Module:
      {
        string name, type;
        if (network::match_module_args(line, name, type))
          return this->def_module(name, type);
      }
      break;

      case CommandType::Connect:
      {
        string such, to;
        if (network::match_connect_args(line, such, to))
          return this->connect(such, to);
      }
      break;

      default:
        return this->show_error("Undefined command");
    }

    this->show_error("Arguments didn't match to the command");
  }

  void network::def_module(const string& name, const string& type)
  {
    amodule m;

    if (type == "reverse")
      m = std::make_shared<module::reverse>();
    else if (type == "delay")
      m = std::make_shared<module::delay>();
    else if (type == "echo")
      m = std::make_shared<module::echo>();
    else if (type == "noop")
      m = std::make_shared<module::noop>();
    else
      return this->show_error("Undefined type!");

    if (!head_)
      head_ = m;

    amodules_[name] = m;
  }

  void network::connect(const string& such, const string& to)
  {
    try {
      auto &m = amodules_.at(such);
      auto &n = amodules_.at(to);

      if (m == n) {
        this->show_error("Connect me to me?! :))");
        return;
      }

      if (!m->connect(n))
        this->show_error("Recursive call! Smart! :)");

    } catch (...) {
      this->show_error("Connect what to what?");
    }
  }

  void network::start_process(const string& rest_of_line)
  {
    if (!head_)
      return this->show_error("No module declared!");

    out_ << this->head_->chain_call(rest_of_line)
         << std::endl;
  }


  void network::show_error(const string& msg)
  {
    //technically this should be CERR
    out_ << "ERROR: "
         << msg
         << std::endl;
  }
}
