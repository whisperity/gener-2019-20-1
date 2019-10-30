.PHONY: default all
default: all
all: docs

README.html: README.rst
	pandoc -s \
		-t html \
		-f rst \
		--toc \
		--mathjax \
		README.rst \
		-o README.html

docs: README.html
