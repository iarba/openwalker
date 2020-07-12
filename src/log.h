#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>

extern std::ostream *ow_l;

std::string get_current_time();

void ow_l_lock();

void ow_l_unlock();

#define T_PRINT "[" << std::this_thread::get_id() << "]"
#define D_PRINT get_current_time()
#define L_PRINT "@" << __FILE__ << ":" << __LINE__

#define ow_l_head(level, action) ow_l_lock(); *ow_l << "\n" << T_PRINT << D_PRINT << " " << #level << L_PRINT << "\n" action << "\n"; ow_l_unlock();

#define OW_L_LV_DEBUG
#ifdef OW_L_LV_DEBUG
#define ow_l_debug(action) ow_l_head(debug, action)
#else
#define ow_l_debug(action) 
#endif // OW_L_LV_DEBUG

#define OW_L_LV_TRACE
#ifdef OW_L_LV_TRACE
#define ow_l_trace(action) ow_l_head(trace, action)
#else
#define ow_l_trace(action) 
#endif // OW_L_LV_TRACE

#define OW_L_LV_INFO
#ifdef OW_L_LV_INFO
#define ow_l_info(action) ow_l_head(info, action)
#else
#define ow_l_info(action) 
#endif // OW_L_LV_INFO

#define ow_l_notice ow_l_info

#define OW_L_LV_WARN
#ifdef OW_L_LV_WARN
#define ow_l_warn(action) ow_l_head(warn, action)
#else
#define ow_l_warn(action) 
#endif // OW_L_LV_WARN

#define OW_L_LV_ERROR
#ifdef OW_L_LV_ERROR
#define ow_l_error(action) ow_l_head(error, action)
#else
#define ow_l_error(action) 
#endif // OW_L_LV_ERROR

#endif // LOG_H
