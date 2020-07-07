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
