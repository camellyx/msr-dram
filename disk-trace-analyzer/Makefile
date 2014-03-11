SRCDIR=src
OBJDIR=objs

SRCS=$(SRCDIR)/analyzer.cpp
OBJS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(OBJS) analyzer

.PHONY: clean

analyzer: $(OBJS)
	$(CXX) -o $@ $^

$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) analyzer
