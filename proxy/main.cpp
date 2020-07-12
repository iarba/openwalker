#include "openwalker/openwalker.h"
#include "../tstlib/test.h"

int main(int argc, char **argv)
{
  server_t *s = NULL;
  openwalker_init(2313);
  tst_load();
  try
  {
    std::vector<std::string> args;
    for(int i = 0; i < argc; i++)
    {
      args.push_back(std::string(argv[i]));
    }
    ow_assert_msg(argc == 4, "improper usage")
    s = new server_t(argv[1], std::stoi(argv[2]));
    s->listen_global(std::stoi(argv[3]));
    std::string cmd;
    while(true)
    {
      ow_assert_msg(getline(std::cin, cmd), "input failure");
      if((cmd == "exit") || (cmd == "quit"))
      {
        ow_assert_msg(false, "user termination");
      }
      std::cerr << "unrecognised command - " << cmd << "\n";
    }
  }
  catch(std::exception &e)
  {
    std::cerr << "Termination due to [" << e.what() << "]\n";
    if(s)
    {
      delete s;
    }
  }
  openwalker_destroy();
  return 0;
}
