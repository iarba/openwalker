#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <openwalker/openwalker.h>
#include <mutex>

class manipulator_t
{
public:
  manipulator_t(node_t *n);
  virtual ~manipulator_t();
  bool is_closed();
  void close();
protected:
  node_t *n;
private:
  bool closed = false;
};

#endif // MANIPULATOR_H
