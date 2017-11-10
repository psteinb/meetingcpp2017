CXXFLAGS += -std=c++11 -pthread
LDFLAGS += -I${BENCHMARK_ROOT}/include -L${BENCHMARK_ROOT}/lib

TARGETS=forloop_style forloop_style_with_args forloop_style_with_args_templated

help:
	echo "this makefile assumes that you have any gcc/clang compliant compiler installed"
	echo "and that libbenchmark at \$BENCHMARK_ROOT is available (tested with 1.3.0)"
	echo "for the report, you need an uptodate R installation available including dplyr, readr, ggplot2 and rmarkdown"
	echo "available targets:"
	echo "  build  .. compile & link the example binaries"
	echo "  run    .. run the benchmark which produces the input for the report"
	echo "  report .. generate the report using rmarkdown"

build : $(TARGETS)

% : %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< -o $@ ${BENCHMARK_ROOT}/lib/libbenchmark.a

loops_compared.csv : forloop_style_with_args_templated
	./$< --benchmark_filter=.*/134217728 --benchmark_repetitions=20 --benchmark_out_format=csv --benchmark_out=loops_compared.csv

run : loops_compared.csv


%.html : %.Rmd loops_compared.csv
	Rscript -e "rmarkdown::render('$<')"

%.pdf : %.Rmd loops_compared.csv
	Rscript -e "rmarkdown::render('$<','pdf_document')"

report : report.pdf report.html

clean :
	rm -v $(TARGETS)
	rm loops_compared.csv
	rm report.pdf report.html
