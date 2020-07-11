#ifndef NETWORK_H
#define NETWORK_H

#include <thread>
#include <mutex>
#include <asio.hpp>
#include <vector>
#include "node.h"

class server_t
{
public:
  server_t(node_t *endpoint);
  ~server_t();
  void listen_local();
  void listen_global(int port = 7777);
protected:
  std::mutex server_lock;
  int delay = 10;
  node_t *endpoint;
private:
  void start_accepting(asio::ip::tcp::acceptor *origin);
  void accept(asio::ip::tcp::acceptor *origin, const asio::error_code error, asio::ip::tcp::iostream *sock);
  void duty();
  bool kill = false;
  std::thread duty_thread;
  asio::io_context ioc;
  std::vector<asio::ip::tcp::acceptor *> acceptors;
  std::map<asio::ip::tcp::iostream *, stream_forwarding_node_t *> connections;
};

#endif // NETWORK_H
