all: buildmute

clean:
	rm *~  2> /dev/null || echo ""

build:
	pdflatex document.tex

buildmute:
	pdflatex document.tex > /dev/null

svg2pdf:
	inkscape -z -D --file=images/bell-lp.svg --export-pdf=images/bell-lp.pdf --export-width=100
	inkscape -z -D --file=images/biba.svg --export-pdf=images/biba.pdf --export-width=100

glossary:
	makeindex document.nlo  -s nomencl.ist -o document.nls

final: clean svg2pdf
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	makeindex document.nlo  -s nomencl.ist -o document.nls
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	# You need pdftk in order to remove the second page from this document
	pdftk document.pdf cat 1 3-end output document_ready.pdf

