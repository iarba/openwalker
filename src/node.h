#ifndef NODE_H
#define NODE_H

#include "world.h"
#include <mutex>
#include <set>
#include <iostream>

class command_t
{
  int opcode;
};

class node_delta
{
public:
  node_delta();
  node_delta(const node_delta *other);
  node_delta(std::istream &is);
  ~node_delta();
  void serialise(std::ostream &os);
  world_delta *wd = NULL;
};

class node_t : public world_t
{
public:
  node_t(node_t *parent);
  virtual ~node_t();
  virtual void serialise(std::ostream &os);
  virtual void feed(node_delta *nd);
  virtual void receive_com(command_t c);
  void add_listener(node_t *n);
  void remove_listener(node_t *n);
protected:
  virtual void apply_delta(node_delta *nd);
  bool self_apply = true;
  node_t *parent = NULL;
  std::mutex node_lock;
private:
  std::set<node_t *>listeners;
};

class stream_forwarding_node_t : public node_t
{
public:
  stream_forwarding_node_t(node_t *parent, std::ostream *os);
  virtual ~stream_forwarding_node_t();
  virtual void feed(node_delta *nd);
protected:
  std::ostream *os;
};

#endif // NODE_H
