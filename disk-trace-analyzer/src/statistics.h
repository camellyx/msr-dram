#ifndef _STATISTICS_H
#define _STATISTICS_H

#include <map>
using namespace std;

#define WRITE_INTERVAL_DIST_INTERVAL 10

struct Statistics {
};

struct Distribution {
  void clear() {
    wr_per_day.clear();
    page_per_interv.clear();
  }
  map<int, unsigned long long> wr_per_day;      // write per day distribution
  map<int, unsigned long long> page_per_interv; // write interval distribution
};

void printDistribution(Distribution &dist);

#endif /*_STATISTICS_H*/

