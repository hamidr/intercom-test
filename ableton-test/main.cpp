#include "network.h"

#include <iostream>

int main() {
  Ableton::network ableton_network(std::cout);

  std::string line;
  while(std::getline(std::cin, line)) {
    ableton_network.interpret_line(line);
  }

  return 0;
}
