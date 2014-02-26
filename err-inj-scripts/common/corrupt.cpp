#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

int main (int argc, char * argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./corrupt [number_in_hex]" << endl;
  }
  int origNum, corrNum;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);
  sscanf(argv[1], "%x", &origNum);
  corrNum = origNum ^ (1<<(rand()%8));
  cout << hex << corrNum << endl;
  return 0;
}
