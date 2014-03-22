#include <iostream>
#include <cstdio>
#include <getopt.h>

#include "parser.h"
using namespace std;

FILE *fTrace = stdin;
int (*parseTrace)(FILE *fTrace, TraceRecord &record);

void usage(char *program);
void parse_args(int argc, char **argv);

int main(int argc, char **argv) {

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
        } else {
          printf("ERROR: Unknown trace type: %s\n", optarg);
          EXIT(EXIT_SUCCESS);
        }
      case 'i':
        fTrace = fopen(optarg, "r");
        if (fTrace == NULL) {
          printf("ERROR: Cannot open file: %s\n", optarg);
          EXIT(EXIT_SUCCESS);
        }
      case '?':
        usage(argv[0]);
        exit(EXIT_SUCCESS);
      default:
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
}
