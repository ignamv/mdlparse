
all: test

UNITY=${HOME}/programacion/Unity/src

CPPFLAGS=-I${UNITY}

CXXFLAGS=-g

test_mdlparse: test_mdlparse.o ${UNITY}/unity.c mdlparse.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test_mdlparse
	./$<
