.PHONY: default all
default: all
all: docs

README.html: README.rst
#	pandoc -s \
#		-t html \
#		-f rst \
#		--toc \
#		--mathjax \
#		README.rst \
#		-o README.html
	rst2html.py \
		--no-generator \
		--no-source-link \
		--strip-comments \
		--math-output=MathJax \
		README.rst \
		README.html

docs: README.html
