#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "parser.h"
using namespace std;

int parseTraceCello(FILE *fTrace, TraceRecord &record) {
  bool foundzero = false;
  // read trace
  while (!feof(fTrace)) {
    if ( fscanf(fTrace, "%lf %u %llu %d %c", &record.t, &record.d, &record.b, &record.s, &record.w) == 5 ) {
      return 1;
    }
  }
  return 0;
}

void printRecord(TraceRecord &record) {
  printf("%lf %u %llu %d %c\n", record.t, record.d, record.b, record.s, record.w);
}
