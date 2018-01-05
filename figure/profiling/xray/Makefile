CXXFLAGS?=-O2 -fxray-instrument

all : vector_unroll_example

% : %.cpp
	${CXX} ${CXXFLAGS} -std=c++11 -o $@ $<
