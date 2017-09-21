#include "../../includes/module/echo.h"

namespace Ableton
{
  namespace module {

    std::string echo::process(std::string input) const
    {
      return input + input;
    }

  }
}
