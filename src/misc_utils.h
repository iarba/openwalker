#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <streambuf>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include "log.h"

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

#define ow_assert(condition) ow_assert_msg(condition, "assertion failure")

#define ow_assert_msg(condition, message) if(condition){}else{throw std::logic_error(message);}

#define ow_safe(body) ow_safe_val(body, );
#define ow_safe_val(body, val) try{body;}catch(std::exception &e){ow_l_lock();ow_l_error(<<"caught critical(return) exception " << e.what() << "\n");ow_l_debug(<<"context [" << #body <<"]\n");ow_l_unlock();return val;}
#define ow_safe_cont(body) try{body;}catch(std::exception &e){ow_l_lock();ow_l_warn(<<"caught non-critical(continue) exception " << e.what() << "\n");ow_l_debug(<<"context [" << #body << "]\n");ow_l_unlock();}

#endif // MISC_UTILS_H
