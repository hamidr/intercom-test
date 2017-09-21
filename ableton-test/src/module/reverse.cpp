#include "../../includes/module/reverse.h"

namespace Ableton
{
  namespace module
  {

    std::string reverse::process(std::string input) const
    {
      int s_pos = -1;
      int size = input.size() - 1;
      for (int i = 0; i <= size; ++i)
      {
        char c = input[i];
        if (c != ' ' && s_pos == -1)
          s_pos = i;

        if ((c == ' ' || i == size) && s_pos != -1) {
          if (i == size)
            ++i;

          int h = ((i - s_pos) / 2);
          int j = i - 1;

          while(j >= (i - h)) {
            std::swap(input[j], input[s_pos]);
            --j;
            ++s_pos;
          }

          s_pos = -1;
        }
      }

      return input;
    }
  }
}
