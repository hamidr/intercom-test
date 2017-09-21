#pragma once

#include <memory>
#include <string>

namespace Ableton {
  class base_process
  {
    std::shared_ptr<base_process> next_;

  protected:
    virtual std::string process(std::string) const = 0;

  public:
    std::string chain_call(const std::string&) const;
    bool connect(std::shared_ptr<base_process> &);
  };

}
