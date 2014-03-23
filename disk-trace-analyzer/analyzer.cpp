#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <getopt.h>

#include "src/parser.h"
#include "src/logger.h"
using namespace std;

FILE *fTrace;
int (*parseTrace)(FILE *fTrace, TraceRecord &record);

void usage(char *program);
void parse_args(int argc, char **argv);

int main(int argc, char **argv) {
  parse_args(argc, argv);
  TraceRecord record;
  Logger logger;
  int day = 1;
  while (parseTrace(fTrace, record)) {
    if (record.w != 'T') continue;
    //printRecord(record);
    //if (record.t > 3600.0*24*day) {
    if (record.t > 3600.0*day) { // faster debug
      logger.finishDay(day);
      day++;
    }
    logger.addToLog(record);
  }
}

void usage(char *program) {
  printf("Usage: %s [options]\n"
      "  analyze a disk trace \n"
      "\n"
      "Program Options:\n"
      "  -t  --type cello|ascii   Type of input trace\n"
      "  -i  --input <file>       Use <file> instead of generated dataset\n"
      "  -?  --help               This message\n", program);
}

void parse_args(int argc, char **argv) {
  int opt;

  // Default
  fTrace = stdin;
  parseTrace = NULL;

  static struct option long_opts[] = {
    {"type", 1, 0, 't'},
    {"input", 1, 0, 'i'},
    {"help", 0, 0, '?'},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "t:i:?h", long_opts, NULL)) != EOF) {
    switch (opt) {
      case 't':
        if (strcmp(optarg, "cello") == 0) {
          parseTrace = &parseTraceCello;
        } else if (strcmp(optarg, "ascii") == 0) {
          // TODO: implement ascii
          printf("ERROR: Unimplemented type: %s\n", optarg);
          exit(EXIT_SUCCESS);
        }else {
          printf("ERROR: Unknown trace type: %s\n", optarg);
          exit(EXIT_SUCCESS);
        }
        break;
      case 'i':
        fTrace = fopen(optarg, "r");
        if (fTrace == NULL) {
          printf("ERROR: Cannot open file: %s\n", optarg);
          exit(EXIT_SUCCESS);
        }
        break;
      case '?':
      default:
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (parseTrace == NULL) {
    printf("ERROR: Unspecified trace type\n");
    usage(argv[0]);
    exit(EXIT_SUCCESS);
  }
}
