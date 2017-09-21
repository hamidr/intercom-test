#include "../../includes/module/delay.h"

namespace Ableton
{
  namespace module {

    std::string delay::process(std::string input) const
    {
      return "hello " + input;
    }

  }
}
