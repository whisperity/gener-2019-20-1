.PHONY: default
default:
	echo "What to do?"

docs:
	pandoc -s \
		-t html \
		-f rst \
		--toc \
		--mathjax \
		README.rst \
		-o README.html
