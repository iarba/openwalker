#ifndef TEST_TEST_H
#define TEST_TEST_H

#include "../src/main.h"

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
};

#endif // TEST_TEST_H
