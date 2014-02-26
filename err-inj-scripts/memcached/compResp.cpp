#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

#define MAX_KEY_LEN 1024

struct KVPair {
  int line;
  int type;
  int key_len;
  char key[MAX_KEY_LEN];
  int value_len;
};

bool operator ==(KVPair &a, KVPair &b) {
  return ( a.value_len == b.value_len ) && ( a.key_len == b.key_len ) && ( strcmp(a.key, b.key) == 0 ) && ( a.type == b.type );
}

void readRespFile(FILE * pFile, vector<KVPair> &vTarget, int &line);
bool read1000(FILE * pFile, vector<KVPair> &vTarget, int &line);
void cancelRespRelax(vector<KVPair> &vResp1, vector<KVPair> &vResp2);
void cancelResp(vector<KVPair> &vResp1, vector<KVPair> &vResp2);
void printKVPair(KVPair &pair);
void printResp(vector<KVPair> &vResp);

int main (int argc, char * argv[]) {
  if (argc != 3) {
    cerr << "Usage: ./compResp [resp1] [resp2]" << endl;
  }
  FILE * pResp1 = fopen(argv[1], "r");
  FILE * pResp2 = fopen(argv[2], "r");

  vector<KVPair> vResp1, vResp2;

  if (pResp1 == NULL || pResp2 == NULL) {
    cerr << "Error: file open error!!! " << argv[1] << " " << argv[2] << endl;
    return -1;
  }

  int lineNum1 = 0, lineNum2 = 0;
  while ( read1000(pResp1, vResp1, lineNum1) && read1000(pResp2, vResp2, lineNum2) ) {
    cancelRespRelax(vResp1, vResp2);
    //cout << vResp1.size() << " " << vResp2.size() << endl;
  }
  
  //cout << lineNum1 << " " << lineNum2 << endl;

  readRespFile(pResp1, vResp1, lineNum1);
  readRespFile(pResp2, vResp2, lineNum2);

  fclose(pResp1);
  fclose(pResp2);

  cancelResp(vResp1, vResp2);

  cout << vResp1.size() << " " << vResp2.size() << endl;

  printResp(vResp1);
  printResp(vResp2);

  return 0;
}

bool read1000(FILE * pFile, vector<KVPair>  &vTarget, int &line) {
  KVPair tmp;
  int count = 10;
  while (fscanf(pFile, "@ %d %d %s %d\n", &tmp.type, &tmp.key_len, tmp.key, &tmp.value_len) == 4) {
    tmp.line = line;
    vTarget.push_back(tmp);
    count--;
    line++;
    if (count == 0) break;
  }
  return count == 0;
}

void readRespFile(FILE * pFile, vector<KVPair>  &vTarget, int &line) {
  KVPair tmp;
  while ( fscanf(pFile, "@ %d %d %s %d\n", &tmp.type, &tmp.key_len, tmp.key, &tmp.value_len) == 4 ) {
    tmp.line = line;
    vTarget.push_back(tmp);
    line++;
  }
  cout << "Read succeeded" << endl;
}

void cancelRespRelax(vector<KVPair> & vResp1, vector<KVPair> & vResp2) {
  vector<KVPair>::iterator iter1, iter2;
  for (iter1 = vResp1.begin(); iter1 != vResp1.end(); iter1++) {
    for (iter2 = vResp2.begin(); iter2 != vResp2.end(); iter2++) {
      if ( *iter1 == *iter2 ) {
        iter1 = vResp1.erase(iter1);
        iter1 --;
        vResp2.erase(iter2);
        break;
      }
    }
  }
}

void cancelResp(vector<KVPair> & vResp1, vector<KVPair> & vResp2) {
  for (int i=0;i<vResp1.size();i++) {
    for (int j=0;j<vResp2.size();j++) {
      if ( vResp1[i] == vResp2[j] ) {
        vResp1.erase(vResp1.begin()+i);
        vResp2.erase(vResp2.begin()+j);
        i--;
        //cout << i << " bla " << j << endl;
        break;
      } else {
        //printKVPair(vResp1[i]);
        //printKVPair(vResp2[j]);
      }
    }
  }
}

void printKVPair(KVPair &pair) {
  cout << pair.line << " @ " << pair.type << " " << pair.key_len << " " << pair.key << " " << pair.value_len << endl;
}

void printResp(vector<KVPair> &vResp) {
  for ( vector<KVPair>::iterator i = vResp.begin();
        i != vResp.end(); i++ ) {
    printKVPair(*i);
  }
}

