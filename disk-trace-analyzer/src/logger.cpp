#include <cstdio>
#include <cassert>
#include "parser.h"
#include "logger.h"
using namespace std;

void Logger::addToLog(TraceRecord &record) {
  int s = record.s;
  int b = record.b;
  map<unsigned long long, PageLog>::iterator log_it;
  while (s>0) {
    // find old log
    log_it = log.find(b);
    if (log_it != log.end()) {
      // found
      double interval = record.t - log_it->second.prev_t;
      log_it->second.prev_t = record.t;
      log_it->second.w_cnt++;
      // update write interval distribtuion
      int int_interval = (int)(interval/WRITE_INTERVAL_DIST_INTERVAL)*WRITE_INTERVAL_DIST_INTERVAL;
      addToDistInterval(int_interval);
    } else {
      // not found
      PageLog newLog;
      newLog.prev_t = record.t;
      newLog.w_cnt = 1;
      log[b] = newLog;
    }
    s -= 1024;
    b += 1024;
  }
}

void Logger::addToDistFrequency() {
  map<unsigned long long, PageLog>::iterator log_it;
  for (log_it=log.begin(); log_it!=log.end(); ++log_it) {
    int count = log_it->second.w_cnt;
    log_it->second.w_cnt = 0;
    if (dist.wr_per_day.find(count) == dist.wr_per_day.end()) {
      dist.wr_per_day[count] = 0;
    }
    dist.wr_per_day[count]++;
  }
}

void Logger::addToDistInterval(int interval) {
  if (dist.page_per_interv.find(interval) == dist.page_per_interv.end()) {
    // new interval
    dist.page_per_interv[interval] = 0;
  }
  dist.page_per_interv[interval]++;
}

