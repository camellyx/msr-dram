#ifndef _LOGGER_H
#define _LOGGER_H

#include <map>
#include "statistics.h"
using namespace std;

struct PageLog {
  double prev_t;
  unsigned long long w_cnt;
};

class Logger {
  public:
    Distribution dist;
    map<unsigned long long, PageLog> log;

    Logger() {}
    ~Logger() {}

    void clear() {
      dist.clear();
      log.clear();
    }
    void addToLog(TraceRecord &record);

    void addToDistFrequency();
    void addToDistInterval(int interval);

    void finishDay(int day) {
      addToDistFrequency();
      printf("*****************************************\n");
      printf("****** Day %d ******\n", day);
      printDistribution(dist);
      clear();
    }
};

#endif /*_LOGGER_H*/
