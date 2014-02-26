#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cassert>
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

void printKVPair(KVPair &pair);
void printResp(vector<KVPair> &vResp);

struct Stats {
  int totResult;
  int crashed;
  int unacceptable;
  int unmatched;
  int matched;
  int diffIncorrect;
  int cntIncorrect;
  int queryIncorrect;
  int queryCnt1;
  int queryCnt2;
  int queryCnt3;
  int queryCnt4;
  int queryCnt5;
  int queryCnt5p;
};

Stats stat;

void updateCnt(int queryCnt);

int main() {
  string input, folder;
  int diffCnt1, diffCnt2;
  KVPair kvInput;
  vector<KVPair> vecKV;
  while ( !cin.eof() ) {
    getline(cin, input);
    if (input[0] == '#') {
      stat.totResult++;
      folder = input;
    } else if ( input.find("Read succeeded") != string::npos ) { continue; } // Skip
    else if ( input.find("Crashed") != string::npos ) {
      int cnt1, cnt2;
      sscanf( input.c_str(), "Crashed: %d %d", &cnt1, &cnt2 );
      cnt2 = 3000000 + cnt1 - cnt2;
      cout << "Crashed: " << folder << endl;
      stat.crashed++;
    } else if ( input.find("too many wrong queries") != string::npos ) {
      cout << "Unacceptable: " << folder << endl;
      stat.unacceptable++;
    } else if ( sscanf( input.c_str(), "%d %d", &diffCnt1, &diffCnt2 ) == 2 ) {
      int filtCnt1 = 0, filtCnt2 = 0;
      for (int i=0; i<diffCnt1; i++) {
        getline(cin, input);
        if ( sscanf( input.c_str(), "%d @ %d %d %s %d", &kvInput.line, &kvInput.type, &kvInput.key_len, kvInput.key, &kvInput.value_len ) != 5 ) {
          cerr << "Error: Expecting a KVPair in " << input << endl;
        }
        if ( strcmp( kvInput.key, "uS6z5D61mAy23s33NQTUz4SMjTJ1AwVUfPhaicZsCoktGbmhS4ZhYIR7rqXPMGyPlFOrHDJ7RTrlTBqBvgGkOlf3B0Jbw7RFAvW8Yv5EdmNWM4l78RfM0jDr9asu77ZysMuf5pJ8AmSce37cIdMymgPlg6Dc3tyk5SnY5WXt6Ft9xrb5sMuES7NW1hWTAi1t0ehUY2bUwvRTKTM2t6ub" ) == 0 && ( kvInput.value_len == 0 || kvInput.value_len == 598 )  ) {
          continue;
        }
        vecKV.push_back(kvInput);
        filtCnt1++;
      }
      for (int i=0; i<diffCnt2; i++) {
        getline(cin, input);
        if ( sscanf( input.c_str(), "%d @ %d %d %s %d", &kvInput.line, &kvInput.type, &kvInput.key_len, kvInput.key, &kvInput.value_len ) != 5 ) {
          cerr << "Error: Expecting a KVPair in " << input << endl;
        }
        if ( strcmp( kvInput.key, "uS6z5D61mAy23s33NQTUz4SMjTJ1AwVUfPhaicZsCoktGbmhS4ZhYIR7rqXPMGyPlFOrHDJ7RTrlTBqBvgGkOlf3B0Jbw7RFAvW8Yv5EdmNWM4l78RfM0jDr9asu77ZysMuf5pJ8AmSce37cIdMymgPlg6Dc3tyk5SnY5WXt6Ft9xrb5sMuES7NW1hWTAi1t0ehUY2bUwvRTKTM2t6ub" ) == 0 && ( kvInput.value_len == 0 || kvInput.value_len == 598 )  ) {
          continue;
        }
        vecKV.push_back(kvInput);
        filtCnt2++;
      }
      if (diffCnt1 != diffCnt2) {
        if ( abs(diffCnt2 - diffCnt1) > 100 ) {
          stat.crashed++;
          cout << "Crashed: " << folder << endl;
        } else { // Missing/More queries
          stat.unmatched++;
          stat.cntIncorrect++;
          stat.queryIncorrect += max(filtCnt1, filtCnt2);
          cout << "Diff by " << abs(diffCnt2-diffCnt1) << " " << folder << endl;
          updateCnt(max(filtCnt1, filtCnt2));
        }
      } else if ( filtCnt1 != 0 ) { // exist unmatched query
        stat.unmatched++;
        stat.diffIncorrect++;
        stat.queryIncorrect += filtCnt1;
        updateCnt(filtCnt1);
        if (filtCnt1 > 5) {
          cout << "Large count: " << filtCnt1 << " " << folder << endl;
        }
      } else {
        //assert( filtCnt1 == filtCnt2 && filtCnt1 == 0);
        stat.matched++;
      }
    }
  }

  cout << "-----------------------Statistics-----------------------" << endl;
  cout << setw(30) << "       Total results:" << setw(8) << stat.totResult << endl;
  cout << setw(30) << "     Crashed results:" << setw(8) << stat.crashed << endl;
  cout << setw(30) << "Unacceptable results:" << setw(8) << stat.unacceptable << endl;
  cout << setw(30) << "   Unmatched results:" << setw(8) << stat.unmatched << endl;
  cout << setw(30) << "     Matched results:" << setw(8) << stat.matched << endl;
  cout << setw(30) << "   Unmatched(differ):" << setw(8) << stat.diffIncorrect << endl;
  cout << setw(30) << "     Unmatched(miss):" << setw(8) << stat.cntIncorrect << endl;
  cout << setw(30) << "       Total queries:" << setw(8) << stat.queryIncorrect << endl;
  cout << setw(30) << "       Query count 1:" << setw(8) << stat.queryCnt1 << endl;
  cout << setw(30) << "       Query count 2:" << setw(8) << stat.queryCnt2 << endl;
  cout << setw(30) << "       Query count 3:" << setw(8) << stat.queryCnt3 << endl;
  cout << setw(30) << "       Query count 4:" << setw(8) << stat.queryCnt4 << endl;
  cout << setw(30) << "       Query count 5:" << setw(8) << stat.queryCnt5 << endl;
  cout << setw(30) << "      Query count 5+:" << setw(8) << stat.queryCnt5p << endl;


  cout << "-------------------Incorrect Queries--------------------" << endl;
  printResp(vecKV);

  return 0;
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

void updateCnt(int queryCnt) {
  switch (queryCnt) {
    case 1:
      stat.queryCnt1++;
      break;
    case 2:
      stat.queryCnt2++;
      break;
    case 3:
      stat.queryCnt3++;
      break;
    case 4:
      stat.queryCnt4++;
      break;
    case 5:
      stat.queryCnt5++;
      break;
    default:
      stat.queryCnt5p++;
  }
}

