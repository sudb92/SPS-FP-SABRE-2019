CC=gcc -O3
CXX=g++ -O3
RM=rm -f
INCLDIR=./include
SRCDIR=./src
CPPFLAGS=-g $(shell root-config --cflags) -I$(INCLDIR)
LDFLAGS=-g $(shell root-config --ldflags)
LDLIBS=$(shell root-config --libs)
SRCS=$(SRCDIR)/processRun.C
OBJS=$(subst .C,.o,$(SRCS))

all: processRun

processRun: $(OBJS)
	$(CXX) $(LDFLAGS) -o processRun $(OBJS) $(LDLIBS)

processRun.o: processRun.C

clean:
	$(RM) $(OBJS) 

distclean: clean
	$(RM) processRun

#Use commands below to profile the executable's performance
#g++ -O3 `root-config --cflags` --coverage -fprofile-arcs -fbranch-probabilities -o runner processRun.C `root-config --glibs`
#g++ -O3 `root-config --cflags` -o runner processRun.C `root-config --glibs`  
