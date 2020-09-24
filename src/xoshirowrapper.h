#include <cstdint>

class xoshirorandomiser
{
public:
  static void load();
  static void destroy();
  static xoshirorandomiser *g_rand_get();
  xoshirorandomiser(uint64_t seed1 = 0, uint64_t seed2 = 0, uint64_t seed3 = 0, uint64_t seed4 = 0);
  xoshirorandomiser(const xoshirorandomiser &other);
  xoshirorandomiser(const xoshirorandomiser *other);
  uint64_t next();
  double next_d();
  float next_f();
protected:
  uint64_t s[4];
private:
};

