PANDOC ?= pandoc

# Pandoc filters.
FILTERS = $(wildcard tools/filters/*.py)

all : index.htm

tools/filters/linkTable : tools/filters/linkTable.hs
	ghc $<

index.htm : slides.md 
	$(PANDOC) -s --template=pandoc-revealjs.template -t revealjs -o $@ -V transition=slide --section-divs --filter tools/filters/columnfilter.py $<

clean :
	rm -f links.md index.htm?
