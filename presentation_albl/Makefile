all: buildmute

clean:
	rm -f *~  2> /dev/null

build:
	pdflatex soutenance.tex

buildmute:
	pdflatex soutenance.tex > /dev/null

svg2pdf:
	inkscape -z -D --file=piga_before_after.svg --export-pdf=piga_evol.pdf --export-width=100

final:
	rm -f *~  2> /dev/null
	pdflatex soutenance.tex
	pdflatex soutenance.tex
	pdflatex soutenance.tex

