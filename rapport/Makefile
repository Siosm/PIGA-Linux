all: buildmute

clean:
	rm -f *~  2> /dev/null

build:
	pdflatex document.tex

buildmute:
	pdflatex document.tex > /dev/null

svg2pdf:
	inkscape -z -D --file=images/bell-lp.svg --export-pdf=images/bell-lp.pdf --export-width=100
	inkscape -z -D --file=images/biba.svg --export-pdf=images/biba.pdf --export-width=100
	inkscape -z -D --file=images/Logo_SELinux.svg --export-pdf=images/Logo_SELinux.pdf --export-width=100

glossary:
	makeindex document.nlo  -s nomencl.ist -o document.nls

final:
	rm -f *~  2> /dev/null
	inkscape -z -D --file=images/bell-lp.svg --export-pdf=images/bell-lp.pdf --export-width=100
	inkscape -z -D --file=images/biba.svg --export-pdf=images/biba.pdf --export-width=100
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	makeindex document.nlo  -s nomencl.ist -o document.nls
	pdflatex document.tex > /dev/null
	pdflatex document.tex > /dev/null
	# You need pdftk in order to remove the second page from this document
	pdftk document.pdf cat 1 3-end output document_ready.pdf

slides:
	pdflatex soutenance.tex > /dev/null

slides_final:
	inkscape -z -D --file=images/piga_evol.svg --export-pdf=images/piga_evol.pdf --export-width=100
	pdflatex soutenance.tex > /dev/null
	pdflatex soutenance.tex > /dev/null
	pdflatex soutenance.tex > /dev/null

