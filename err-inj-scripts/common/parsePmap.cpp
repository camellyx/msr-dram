#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
using namespace std;

void insertPages(vector<unsigned long long> * target, unsigned long long startAddr, int size);
void printVector(vector<unsigned long long> * target);

int main (int argc, char * argv[]) {

  vector<unsigned long long> anon, stack, shared;

  // Parse arguments
  if (argc < 2) {
    cerr << "Usage: parsePmap [pmap_file.txt] [anon|stack|shared]" << endl;
    return -1;
  }

  // Parse pmap.txt
  fstream fPmap;
  fPmap.open(argv[1]);
  while (fPmap.good()) {
    char inputLine[1024];
    unsigned long long addr;
    int size;
    char perm[100], type[100];
    fPmap.getline(inputLine, 1000);
    if ( sscanf(inputLine, "%llx\t%dK %s\t%[^\t\n]", &addr, &size, perm, type ) != 4 ) {
      continue;
    }
    //cout << inputLine << endl; cout << "> " << hex << addr << "|" << dec << size << "|" << type << "|" << endl;
    if ( strcmp(type, "[ anon ]") == 0 ) {
      if ( strcmp(perm, "rw---") == 0 ) {
        insertPages(&anon, addr, size);
      }
    } else if ( strcmp(type, "[ stack ]") == 0 ) {
      insertPages(&stack, addr, size);
    } else if ( strchr(type, '/') != NULL ) {
      insertPages(&shared, addr, size);
    } else {
      cerr << "Error: unexpected memory type: " << type << " !" << endl;
      return -1;
    }
  }
/*
  cout << "-------------------stack-------------------" << endl;
  printVector(&stack);
  cout << "-------------------shared------------------" << endl;
  printVector(&shared);
  cout << "-------------------anon--------------------" << endl;
  printVector(&anon);
*/
  if (argc == 3) {
    vector<unsigned long long> * target;
    if ( strcmp(argv[2], "anon") == 0 ) {
      target = &anon;
    } else if ( strcmp(argv[2], "stack") == 0 ) {
      target = &stack;
    } else if ( strcmp(argv[2], "shared") == 0 ) {
      target = &shared;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);
    unsigned long long returnedAddr;
    returnedAddr = target->at(rand()%target->size());
    returnedAddr |= rand()%4096;
    cout << hex << returnedAddr << endl;
  }

  return 0;
}

void insertPages(vector<unsigned long long> * target, unsigned long long startAddr, int size) {
  while (size) {
    target->push_back(startAddr);
    size -= 4;
    startAddr += 4096;
  }
}

void printVector(vector<unsigned long long> * target) {
  if (target->size() > 1000) {
    cout << "size = " << dec << target->size() << endl;
    return;
  }
  vector<unsigned long long>::iterator i;
  cout << "{";
  for (i = target->begin(); i != target->end(); i++) {
    cout << hex << *i << ", ";
  }
  cout << "}" << endl;
}
