#ifndef NODE_H
#define NODE_H

#include "world.h"
#include <mutex>
#include <set>
#include <iostream>
#include <thread>

#define OW_CMD_NOP 0
#define OW_CMD_PAUSE 1

class command_t
{
public:
  command_t();
  command_t(std::istream &is);
  ~command_t();
  void serialise(std::ostream &os);
  int opcode = OW_CMD_NOP;
  explicit operator bool() const;
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
  void set_forwarding(bool value);
  void forget(node_t *n);
protected:
  virtual void apply_delta(node_delta *nd);
  bool self_apply = true;
  bool forwarding_enabled = true;
  node_t *parent = NULL;
  std::mutex node_lock;
private:
  std::set<node_t *>listeners;
};

class stream_forwarding_node_t : public node_t
{
public:
  stream_forwarding_node_t(node_t *parent, std::ostream *os, std::istream *is = NULL);
  virtual ~stream_forwarding_node_t();
  virtual void feed(node_delta *nd);
protected:
  std::ostream *os;
  std::istream *is;
  int delay = 10;
private:
  void duty();
  bool kill = false;
  std::thread duty_thread;
};

class stream_fetching_node_t : public node_t
{
public:
  stream_fetching_node_t(std::ostream *os, std::istream *is);
  virtual ~stream_fetching_node_t();
  virtual void receive_com(command_t c);
protected:
  std::ostream *os;
  std::istream *is;
  int delay = 10;
private:
  void duty();
  bool kill = false;
  std::thread duty_thread;
};

#endif // NODE_H
