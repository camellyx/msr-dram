#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// structures and types
struct RHistMeta {
  size_t size;
  unsigned long long b/*Start Block*/;
};
struct HistMeta {
  double t/*Time Stamp*/;
};
typedef vector<RHistMeta> VecRHistMeta;
typedef vector<HistMeta> VecHistMeta;

// function prototypes
bool compSize(RHistMeta a, RHistMeta b) { return a.size > b.size; };
void parseTraceCello(FILE *fTrace, map<unsigned long long, VecHistMeta> &history);
void hist2rhist(map<unsigned long long, VecHistMeta> &history, VecRHistMeta &rHistory);

int main(int argc, char* argv[]) {
  FILE *fTrace = fopen(argv[argc-1], "r");
  if (fTrace == NULL) {
    perror(argv[argc-1]);
    return -1;
  }

  map<unsigned long long, VecHistMeta> history;
  parseTraceCello(fTrace, history);
  fclose(fTrace);

  VecRHistMeta rHistory;
  hist2rhist(history, rHistory);

  // print size distribution
  map<size_t, size_t> distribution;
  for (int i=0; i<rHistory.size(); i++) {
    map<size_t, size_t>::iterator it = distribution.find(rHistory[i].size);
    if (it != distribution.end()) {
      // find old size count
      it->second++;
    } else {
      // not found size count
      distribution[rHistory[i].size] = 1;
    }
  }

  // print
  for (vector<RHistMeta>::iterator it=rHistory.begin(),end=rHistory.end(); it!=end; ++it) {
    printf("\n%llu:\n\t", it->b);
    for (int i=1; i<it->size; i++) {
      printf("%f ", history[it->b][i].t - history[it->b][i-1].t);
      if (i%10 == 0) printf("\n\t");
    }
  }

  // print distribution
  printf("\n-----------------------------------------------\n");
  printf("%10s %10s\n", "size", "count");
  for (map<size_t, size_t>::iterator it=distribution.begin(),end=distribution.end(); it!=end; ++it) {
    printf("%10d %10d\n", it->first, it->second);
  }

  return 0;
}

void parseTraceCello(FILE *fTrace, map<unsigned long long, VecHistMeta> &history) {
  // read trace
  while (!feof(fTrace)) {
    double t/*Time Stamp*/;
    unsigned int d/*Device id*/;
    int s/*Size*/;
    unsigned long long b/*Start Block*/;
    char w/*Is Write*/;
    if ( fscanf(fTrace, "%lf %u %llu %d %c", &t, &d, &b, &s, &w) == 5 ) {
      while (s > 0) {
        map<unsigned long long, VecHistMeta>::iterator it = history.find(b);
        HistMeta newHist = {t};
        if (it == history.end()) {
          // not found: new element
          history[b] = VecHistMeta(1,newHist);
        } else {
          // found: append element
          history[b].push_back(newHist);
        }
        s -= 512;
        b += 512;
      }
    }
  }
}

void hist2rhist(map<unsigned long long, VecHistMeta> &history, VecRHistMeta &rHistory) {
  // sort by size
  for (map<unsigned long long, VecHistMeta>::iterator it=history.begin(),end=history.end(); it!=end; ++it) {
    RHistMeta newElem;
    newElem.size = it->second.size();
    newElem.b = it->first;
    rHistory.push_back(newElem);
  }
  sort(rHistory.begin(), rHistory.end(), compSize);
}

