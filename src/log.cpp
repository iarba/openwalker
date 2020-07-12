#include "log.h"

std::ostream *ow_l = NULL;

std::string get_current_time()
{
  char s[1000];
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::time_t t = std::time(&now);
  strftime(s, 1000, "%F %T %z", std::localtime(&t));
  return std::string(s);
}

// level 1 lock acts as concurrency protection for the level 2 lock and control variables
std::mutex l1lock;
// level 2 lock acts as concurrency protection for the logger
std::mutex l2lock;
// control variables
std::thread::id ow_l_owner;
int nesting = 0;

void ow_l_lock()
{
  l1lock.lock();
  if(ow_l_owner == std::this_thread::get_id())
  { // nested locking, disregard
    nesting++;
    l1lock.unlock();
    return;
  }
  // i am not the owner of the lock
  l1lock.unlock();
  // attempt to acquire lock
  l2lock.lock();
  l1lock.lock();
  // set myself as the owner
  ow_l_owner = std::this_thread::get_id();
  l1lock.unlock();
}

void ow_l_unlock()
{
  l1lock.lock();
  if(nesting > 0)
  {
    nesting--;
    l1lock.unlock();
    return;
  }
  // forcefully enter locked state
  l2lock.try_lock();
  // forcefully enter unlocked state
  l2lock.unlock();
  // clear ownership
  ow_l_owner = std::thread::id();
  l1lock.unlock();
}
