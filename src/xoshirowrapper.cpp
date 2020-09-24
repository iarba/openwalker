#include "xoshirowrapper.h"
#include <time.h>

#define REPLACEMENT_SEED_1 2990669493925
#define REPLACEMENT_SEED_2 5635002807746640
#define REPLACEMENT_SEED_3 461168711737038690
#define REPLACEMENT_SEED_4 4951232068203577382

static xoshirorandomiser *g_rand;

xoshirorandomiser::xoshirorandomiser(uint64_t seed1, uint64_t seed2, uint64_t seed3, uint64_t seed4)
{
  if(seed1 == 0)
  {
    seed1 = REPLACEMENT_SEED_1;
  }
  if(seed2 == 0)
  {
    seed2 = REPLACEMENT_SEED_2;
  }
  if(seed3 == 0)
  {
    seed3 = REPLACEMENT_SEED_3;
  }
  if(seed4 == 0)
  {
    seed4 = REPLACEMENT_SEED_4;
  }
  this->s[0] = seed1;
  this->s[1] = seed2;
  this->s[2] = seed3;
  this->s[3] = seed4;
  next();
  next();
  next();
  next();
  next();
  next();
  next();
}

xoshirorandomiser::xoshirorandomiser(const xoshirorandomiser &other)
{
  this->s[0] = other.s[0];
  this->s[1] = other.s[1];
  this->s[2] = other.s[2];
  this->s[3] = other.s[3];
}

xoshirorandomiser::xoshirorandomiser(const xoshirorandomiser *other)
{
  this->s[0] = other->s[0];
  this->s[1] = other->s[1];
  this->s[2] = other->s[2];
  this->s[3] = other->s[3];
}


#define rotl(x, k) ((x << k) | (x >> (64 - k)))

uint64_t xoshirorandomiser::next()
{
  const uint64_t result = rotl(s[1] * 5, 7) * 9;

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);

  return result;
}

#define TWO63 0x8000000000000000u 
#define TWO64f (TWO63 * 2.0)

double xoshirorandomiser::next_d()
{
  double y = (double)this->next();
  return y / TWO64f;
}

float xoshirorandomiser::next_f()
{
  return (float)this->next_d();
}

void xoshirorandomiser::load()
{
  g_rand = new xoshirorandomiser(time(NULL));
}

void xoshirorandomiser::destroy()
{
  delete g_rand;
}

xoshirorandomiser *xoshirorandomiser::g_rand_get()
{
  return g_rand;
}
