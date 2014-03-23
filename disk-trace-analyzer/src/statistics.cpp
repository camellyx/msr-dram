#include <cstdio>
#include <cassert>
#include <map>
#include "statistics.h"
using namespace std;

void printDistribution(Distribution &dist) {
  // print write frequency distribution
  printf("---------------------------------------\n");
  printf("-Printing write frequency distribution-\n");
  printf("---------------------------------------\n");
  printf("Writes/Day, Pages\n");
  int max_freq = dist.wr_per_day.rbegin()->first;
  map<int, unsigned long long>::iterator it = dist.wr_per_day.begin();
  for (int i=1; i<=max_freq; i++) {
    unsigned long long count = 0;
    if (i == it->first) {
      count = it->second;
      it++;
    }
    //if (count) printf("%d, %llu\n", i, count);
  }
  // print write interval distribution
  printf("-----------------------------------------\n");
  printf("- Printing write frequency distribution -\n");
  printf("-----------------------------------------\n");
  printf("Write Interval, Count\n");
  int max_interval = dist.page_per_interv.rbegin()->first;
  it = dist.page_per_interv.begin();
  for (int i=0; i<=max_interval; i+=WRITE_INTERVAL_DIST_INTERVAL) {
    unsigned long long count = 0;
    if (i == it->first) {
      count = it->second;
      assert(count);
      it++;
    }
    printf("%d, %llu\n", i, count);
  }
}


