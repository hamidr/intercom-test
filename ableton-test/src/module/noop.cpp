#include "../../includes/module/noop.h"

namespace Ableton
{
  namespace module {

    std::string noop::process(std::string input) const
    {
      return input;
    }

  }
}
