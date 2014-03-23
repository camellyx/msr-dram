#ifndef _PARSER_H
#define _PARSER_H

struct TraceRecord {
                char w/*Is Write*/;
              double t/*Time Stamp*/;
        unsigned int d/*Device id*/;
                 int s/*Size*/;
  unsigned long long b/*Start Block*/;
};

void printRecord(TraceRecord &record);

int parseTraceCello(FILE *fTrace, TraceRecord &record);

#endif /*_PARSER_H*/
