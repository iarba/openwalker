#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "../src/main.h"

def_zone(test);
def(test, walkable);

void initialise();

void run();

void terminate();

class test_context : public context_t
{
public:
  test_context();
  ~test_context();
  void run();
};

class road : public structure_t
{
public:
  road(glm::ivec2 position, context_t *ctx);
  structure_delta compute_delta() const;
  ~road();
};

#endif // TEST_TEST_H
