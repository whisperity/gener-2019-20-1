.PHONY: default all
default: all
all: libs docs

libs: boost
boost:
	bash -c './get-libs.sh'

PANDOC := $(shell command -v pandoc 2>/dev/null)
RST2HTML := $(shell command -v rst2html.py 2>/dev/null)

docs: README.html
README.html: README.rst
ifdef RST2HTML
	rst2html.py \
		--no-generator \
		--no-source-link \
		--strip-comments \
		--math-output=MathJax \
		README.rst \
		README.html
else ifdef PANDOC
	pandoc -s \
		-t html \
		-f rst \
		--toc \
		--mathjax \
		README.rst \
		-o README.html
else
	$(error "No suitable RST->HTML converter 'pandoc' or 'rst2html.py' found!")
endif

.PHONY: clean
clean:
	rm -f README.html
