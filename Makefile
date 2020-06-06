DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

COMPILE.cxx = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

%.o : %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(COMPILE.cxx) $(OUTPUT_OPTION) $<

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
$(DEPFILES):

all: test

UNITY=${HOME}/programacion/Unity/src
CPPFLAGS=-I$(UNITY)
UNITY_CPP=$(UNITY)/unity.c 
CXXFLAGS=-g -std=c++1z

OBJS=util.o table.o tree.o token.o dataset.o 

PROGRAMS=test_mdlparse.o test_dataset.o test_table.o

test_mdlparse: test_mdlparse.o $(UNITY_CPP) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_dataset: test_dataset.o $(UNITY_CPP) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test_table: test_table.o $(UNITY_CPP) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: test clean
test: test_mdlparse test_dataset test_table
	#./test_mdlparse
	#./test_dataset
	./test_table

clean:
	rm -rf $(OBJS) $(PROGRAMS)

include $(wildcard $(DEPFILES))
