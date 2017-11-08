PANDOC ?= pandoc

# Pandoc filters.
FILTERS = $(wildcard tools/filters/*.py)

all : index.htm

tools/filters/linkTable : tools/filters/linkTable.hs
	ghc $<

index.htm : slides.md
	$(PANDOC) -f markdown-grid_tables+pipe_tables+backtick_code_blocks+fenced_code_blocks+fenced_code_attributes+inline_code_attributes -s --template=pandoc-revealjs.template -t revealjs -o $@ --section-divs --filter tools/filters/notes.py --filter tools/filters/divclass.py --no-highlight $<

clean :
	rm -f links.md index.htm?

present :
	#use a local webserver to show the slides
	npm start
