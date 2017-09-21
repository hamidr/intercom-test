#pragma once

#include <memory>
#include <string>

#include "base_process.h"

namespace Ableton {
  namespace module {
    class echo : public base_process
    {
    protected:
      std::string process(std::string) const override;
    };
  }
}
