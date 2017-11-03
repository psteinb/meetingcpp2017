
all: vector_unroll_example vector_unroll_example2 matrix_unroll_example

%: %.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) $(LDFLAGS) $< -o $@
