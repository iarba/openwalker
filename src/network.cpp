#include "network.h"
#include <functional>
#include "misc_utils.h"

#define MAX_CONNS 100

server_t::server_t(node_t *endpoint)
{
  duty_thread = std::thread(std::bind(&server_t::duty, this));
  this->endpoint = endpoint;
}

server_t::server_t(std::string host, int port)
{
  duty_thread = std::thread(std::bind(&server_t::duty, this));
  server_lock.lock();
  try
  {
    asio::ip::tcp::endpoint ep = (asio::ip::tcp::resolver(ioc).resolve(asio::ip::tcp::resolver::query(host, std::to_string(port), asio::ip::tcp::resolver::query::numeric_service)))->endpoint();
    asio::ip::tcp::iostream *stream = new asio::ip::tcp::iostream();
    connections[stream] = NULL;
    stream->socket().connect(ep);
    this->endpoint = new stream_fetching_node_t(stream, stream);
    connections[stream] = this->endpoint;
  }
  catch(std::exception &e)
  {
    server_lock.unlock();
    kill = true;
    ioc.stop();
    duty_thread.join();
    server_lock.lock();
    for(auto acceptor : acceptors)
    {
      acceptor->close();
      delete acceptor;
    }
    acceptors.clear();
    for(auto it : connections)
    {
      ow_safe_cont(it.first->socket().shutdown(asio::ip::tcp::socket::shutdown_type::shutdown_both););
      if(it.second)
      {
        delete it.second;
      }
      delete it.first;
    }
    connections.clear();
    server_lock.unlock();
    std::rethrow_exception(std::current_exception());
  }
  server_lock.unlock();
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
  acceptors.clear();
  for(auto it : connections)
  {
    ow_safe_cont(it.first->socket().shutdown(asio::ip::tcp::socket::shutdown_type::shutdown_both););
    if(it.second)
    {
      delete it.second;
    }
    delete it.first;
  }
  connections.clear();
  server_lock.unlock();
}

node_t *server_t::get_endpoint()
{
  return endpoint;
}

void server_t::listen_local()
{
}

void server_t::listen_global(int port)
{
  server_lock.lock();
  try
  {
    asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);
    asio::ip::tcp::acceptor *acceptor = new asio::ip::tcp::acceptor(ioc);
    acceptors.push_back(acceptor);
    acceptor->open(ep.protocol());
    acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
    acceptor->bind(ep);
    acceptor->listen(MAX_CONNS);
    start_accepting(acceptor);
  }
  catch(std::exception &e)
  {
    server_lock.unlock();
    std::rethrow_exception(std::current_exception());
  }
  server_lock.unlock();
}

void server_t::start_accepting(asio::ip::tcp::acceptor *origin)
{
  asio::ip::tcp::iostream *stream = new asio::ip::tcp::iostream();
  connections[stream] = NULL;
  origin->async_accept(stream->socket(), std::bind(&server_t::accept, this, origin, std::placeholders::_1, stream));
}

void server_t::accept(asio::ip::tcp::acceptor *origin, const asio::error_code error, asio::ip::tcp::iostream *stream)
{
  if(!error)
  {
    server_lock.lock();
    connections[stream] = new stream_forwarding_node_t(endpoint, stream, stream);
    connections[stream]->set_forwarding(true);
    server_lock.unlock();
    start_accepting(origin);
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
    std::map<asio::ip::tcp::iostream *, node_t *> new_connections;
    server_lock.lock();
    for(auto it : connections)
    {
      if(!it.second)
      { // listener created socket
        new_connections[it.first] = it.second;
        continue;
      }
      if(it.first->socket().is_open())
      { // socket is responsive
        new_connections[it.first] = it.second;
      }
      else
      { // socket is NOT responsive
        ow_safe_cont(it.first->socket().shutdown(asio::ip::tcp::socket::shutdown_type::shutdown_both);)
        delete it.second;
        delete it.first;
      }
    }
    connections = new_connections;
    server_lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  }
}
