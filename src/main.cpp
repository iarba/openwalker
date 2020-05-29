#include "grid.h"

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    return -1;
  }
  srand(std::atoi(argv[1]));
  return 0;
}
