SRCDIR=src
OBJDIR=objs

SRCS=$(wildcard $(SRCDIR)/*.cpp)
#SRCS=\
#     $(SRCDIR)/analyzer.cpp \
#     $(SRCDIR)/analyzer1.cpp \
#     $(SRCDIR)/predictor.cpp \
#     $(SRCDIR)/predictor1.cpp

OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

TRGSRC=$(wildcard *.cpp)
TRG=$(patsubst %.cpp,%,$(TRGSRC))

all: $(OBJS) $(TRG)

.PHONY: clean

%: %.cpp $(OBJS)
	$(CXX) -o $@ $^

run: $(TRG)
	./analyzer -t cello -i /media/sf_cello99-traces/tesla.hpl.hp.com/opensource/cello99/ds-bzip2/cello99-write-trace.txt

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/*.h
	$(CXX) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(TRG)
