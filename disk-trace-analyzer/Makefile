SRCDIR=src
OBJDIR=objs

SRCS=\
		 $(SRCDIR)/analyzer.cpp \
		 $(SRCDIR)/predictor.cpp \
		 $(SRCDIR)/predictor1.cpp

OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
TRG=$(patsubst $(SRCDIR)/%.cpp,%,$(SRCS))

all: $(OBJS) $(TRG)

.PHONY: clean

%: $(OBJDIR)/%.o $(OBJS)
	$(CXX) -o $@ $<

run: $(TRG)
	./analyzer combine.out

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(TRG)
