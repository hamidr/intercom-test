#include "../../includes/module/base_process.h"

namespace Ableton
{

  std::string base_process::chain_call(const std::string& first_input) const
  {
    std::string res = this->process(first_input);

    if (next_)
      res = next_->chain_call(res);

    return res;
  }

  bool base_process::connect(std::shared_ptr<base_process>& process)
  {
    if (process->next_ && process->next_.get() == this)
      return false;

    next_ = process;
    return true;
  }
}
