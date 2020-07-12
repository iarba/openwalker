#include <openwalker/openwalker.h>
#include <ow_f_lib/ow_f_lib.h>
#include "../tstlib/test.h"

int main(int argc, char **argv)
{
  server_t *s = NULL;
  openwalker_set_logger(&std::cout);
  openwalker_init(2313);
  ow_f_lib_init();
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
      ow_l_error(<< "unrecognised command - " << cmd);
    }
  }
  catch(std::exception &e)
  {
    ow_l_warn(<< "Termination die to [" << e.what() << "]");
    if(s)
    {
      delete s;
    }
  }
  openwalker_destroy();
  return 0;
}
