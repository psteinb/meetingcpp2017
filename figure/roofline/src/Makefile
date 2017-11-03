
TARGETS=matrix_unroll_example

LIKWID_ROOT?=${HOME}/software/likwid/4.2.1/

ifneq ($(wildcard $(LIKWID_ROOT)/.*),)
	TARGETS+=matrix_unroll_example_markerapi
endif

all: $(TARGETS)

%: %.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) $(LDFLAGS) $< -o $@

matrix_unroll_example_markerapi : matrix_unroll_example_markerapi.cpp
	$(CXX) $< -std=c++11 $(CXXFLAGS) $(LDFLAGS) -DLIKWID_PERFMON -I$(LIKWID_ROOT)/include -L$(LIKWID_ROOT)/lib -o $@ -llikwid

%.s: %.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) -S -fverbose-asm $(LDFLAGS) $< 
