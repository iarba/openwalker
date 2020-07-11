#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <streambuf>
#include <sstream>
#include <mutex>
#include <condition_variable>

bool eq(double a, double b);

bool eq(double a, double b, double eps);

bool eq(glm::dvec2 a, glm::dvec2 b);

bool eq(glm::dvec2 a, glm::dvec2 b, double eps);

std::vector<std::string> tokenize(std::string origin);

class b_streambuf : public std::stringbuf
{
public:
  b_streambuf();
  virtual ~b_streambuf();
protected:
  int overflow(int c = EOF);
  std::streamsize xsputn(const char* s, std::streamsize n);
  int underflow();
  int uflow();
private:
  std::mutex lock;
  std::condition_variable cv;
};

#endif // MISC_UTILS_H
