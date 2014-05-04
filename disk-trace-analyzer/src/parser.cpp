#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "parser.h"
using namespace std;

unsigned long long line_num;

int parseTraceCello(FILE *fTrace, TraceRecord &record) {
  bool foundzero = false;
  // read trace
  while (!feof(fTrace)) {
    if ( fscanf(fTrace, "%lf %u %llu %d %c", &record.t, &record.d, &record.b, &record.s, &record.w) == 5 ) {
      record.l = line_num;
      line_num++;
      return 1;
    }
  }
  return 0;
}

void printRecord(TraceRecord &record) {
  printf("%lld:%lf %u %llu %d %c\n", record.l, record.t, record.d, record.b, record.s, record.w);
}
