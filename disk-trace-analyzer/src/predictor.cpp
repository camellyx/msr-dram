#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <cassert>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

// global variables
float detectInterval = 60;
size_t cacheSize = 1000;

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
struct cacheMeta {
  unsigned long long b;
  vector<double> t;
};

// function prototypes
bool compSize(RHistMeta a, RHistMeta b) { return a.size > b.size; };
void parseTraceCello(FILE *fTrace, map<unsigned long long, VecHistMeta> &history);
void hist2rhist(map<unsigned long long, VecHistMeta> &history, VecRHistMeta &rHistory);
void predictor(FILE *fTrace, map<unsigned long long, VecHistMeta> &history);
void cacheAdd(deque<cacheMeta> &c, cacheMeta &cb);
void moveToFront(deque<cacheMeta> &c, deque<cacheMeta>::iterator it);
deque<cacheMeta>::iterator cacheFind(deque<cacheMeta> &c, unsigned long long &b);

int main(int argc, char* argv[]) {
  FILE *fTrace = fopen(argv[argc-1], "r");
  if (fTrace == NULL) {
    perror(argv[argc-1]);
    return -1;
  }

  map<unsigned long long, VecHistMeta> history;
  //parseTraceCello(fTrace, history);
  //fclose(fTrace);

  //fTrace = fopen(argv[argc-1], "r");
  predictor(fTrace, history);
  fclose(fTrace);

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

void predictor(FILE *fTrace, map<unsigned long long, VecHistMeta> &history) {
  struct Statistics {
    unsigned long long hit;
    unsigned long long truePos;
    unsigned long long falsePos;
  };
  Statistics stats;
  stats.hit = 0;
  stats.truePos = 0;
  stats.falsePos = 0;

  deque<cacheMeta> l1,l2,l3;
  unsigned long long lineCnt = 0;
  while (!feof(fTrace)) {
    double t/*Time Stamp*/;
    unsigned int d/*Device id*/;
    int s/*Size*/;
    unsigned long long b/*Start Block*/;
    char w/*Is Write*/;
    if ( fscanf(fTrace, "%lf %u %llu %d %c", &t, &d, &b, &s, &w) == 5 ) {
      b /= 512;
      while (s > 0) {
        deque<cacheMeta>::iterator it = cacheFind(l1, b);
        if (it != l1.end()) {
          // hit in l1
          it->t.push_back(t);
          if (it->t[it->t.size()-1] - it->t[it->t.size()-2] < detectInterval) {
            stats.truePos++;
          } else {
            stats.falsePos++;
          }
          moveToFront(l1, it);
          stats.hit++;
        } else {
          // miss in l1
          cacheMeta newBlock;
          newBlock.b = b;
          newBlock.t.push_back(t);
          cacheAdd(l1, newBlock);
        }
        s -= 512;
        b ++;
      }
    }
    lineCnt++;
    if (lineCnt%100000 == 0) {
      printf("Hits: %llu, truePos: %llu, falsePos: %llu\n", stats.hit, stats.truePos, stats.falsePos);
    }
  }
  printf("Hits: %llu, truePos: %llu, falsePos: %llu\n", stats.hit, stats.truePos, stats.falsePos);
}

void cacheAdd(deque<cacheMeta> &c, cacheMeta &cb) {
  if (c.size() >= cacheSize) {
    c.pop_back();
  }
  c.push_front(cb);
}

void moveToFront(deque<cacheMeta> &c, deque<cacheMeta>::iterator it) {
  cacheMeta tmp = *it;
  c.erase(it);
  c.push_front(tmp);
}

deque<cacheMeta>::iterator cacheFind(deque<cacheMeta> &c, unsigned long long &b) {
 deque<cacheMeta>::iterator it;
 for (it=c.begin(); it!=c.end(); ++it) {
   if (it->b == b) break;
 }
 return it;
}
