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
CPPFLAGS=-I${UNITY}
CXXFLAGS=-g

test_mdlparse: test_mdlparse.o ${UNITY}/unity.c util.o table.o tree.o token.o
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: test
test: test_mdlparse
	./$<

include $(wildcard $(DEPFILES))
