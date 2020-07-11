#include "network.h"
#include <functional>

server_t::server_t(node_t *endpoint)
{
  this->endpoint = endpoint;
  duty_thread = std::thread(std::bind(&server_t::duty, this));
}

server_t::~server_t()
{
  kill = true;
  ioc.stop();
  duty_thread.join();
  server_lock.lock();
  for(auto acceptor : acceptors)
  {
    acceptor->close();
    delete acceptor;
  }
  for(auto it : connections)
  {
    delete it.second;
    delete it.first;
  }
  server_lock.unlock();
}

void server_t::listen_local()
{
}

void server_t::listen_global(int port)
{
  server_lock.lock();
  asio::ip::tcp::acceptor *acceptor = new asio::ip::tcp::acceptor(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
  acceptors.push_back(acceptor);
  start_accepting(acceptor);
  server_lock.unlock();
}

void server_t::start_accepting(asio::ip::tcp::acceptor *origin)
{
  asio::ip::tcp::iostream *stream = new asio::ip::tcp::iostream();
  connections[stream] = new stream_forwarding_node_t(endpoint, stream, stream);
  origin->async_accept(stream->socket(), std::bind(&server_t::accept, this, origin, std::placeholders::_1, stream));
}

void server_t::accept(asio::ip::tcp::acceptor *origin, const asio::error_code error, asio::ip::tcp::iostream *stream)
{
  std::cout << "!!!acceptor returned!!!\n";
  if(!error)
  {
    start_accepting(origin);
    std::cout << "!!!and forwarding enabled!!!\n";
    connections[stream]->set_forwarding(true);
  }
  else
  {
  }
}

void server_t::duty()
{
  while(true)
  {
    server_lock.lock();
    if(kill)
    {
      server_lock.unlock();
      return;
    }
    server_lock.unlock();
    ioc.run();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}
