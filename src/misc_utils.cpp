#include "misc_utils.h"
#include <sstream>

#define DEF_EPS 0.0001

bool eq(double a, double b)
{
  return eq(a, b, DEF_EPS);
}

bool eq(double a, double b, double eps)
{
  return std::abs(a - b) < eps;
}

bool eq(glm::dvec2 a, glm::dvec2 b)
{
  return eq(a, b, DEF_EPS);
}

bool eq(glm::dvec2 a, glm::dvec2 b, double eps)
{
  return eq(a.x, b.x, eps) && eq(a.y, b.y, eps);
}

std::vector<std::string> tokenize(std::string origin)
{
  std::vector<std::string> tokens;
  std::stringstream ss(origin);
  std::string str;
  while(ss >> str)
  {
    tokens.push_back(str);
  }
  return tokens;
}

b_streambuf::b_streambuf() : std::stringbuf()
{
}

b_streambuf::~b_streambuf()
{
}

int b_streambuf::overflow(int c)
{
  int rval = this->std::stringbuf::overflow(c);
  cv.notify_all();
  return rval;
}

std::streamsize b_streambuf::xsputn(const char* s, std::streamsize n)
{
  std::streamsize rval = this->std::stringbuf::xsputn(s, n);
  cv.notify_all();
  return rval;
}

int b_streambuf::underflow()
{
  std::unique_lock<std::mutex> l(lock);
  cv.wait(l, [this]{return this->in_avail() > 0;});
  return this->std::stringbuf::underflow();
}

int b_streambuf::uflow()
{
  std::unique_lock<std::mutex> l(lock);
  cv.wait(l, [this]{return this->in_avail() > 0;});
  return this->std::stringbuf::uflow();
}
