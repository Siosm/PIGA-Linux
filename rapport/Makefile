all: buildmute

clean:
	rm -f *~ *.aux *.blg *.ilg *.log *.nls *.toc *.bbl *.idx *.lof *.nlo *.out 2> /dev/null

build:
	pdflatex document.tex

buildmute:
	pdflatex document.tex > /dev/null

svg2pdf:
	inkscape -z -D --file=images/bell-lp.svg --export-pdf=images/bell-lp.pdf --export-width=100
	inkscape -z -D --file=images/biba.svg --export-pdf=images/biba.pdf --export-width=100
	inkscape -z -D --file=images/Logo_SELinux.svg --export-pdf=images/Logo_SELinux.pdf --export-width=100
	inkscape -z -D --file=images/modele_generale_en.svg --export-pdf=images/modele_generale_en.pdf --export-width=100
	inkscape -z -D --file=images/piga_evol.svg --export-pdf=images/piga_evol.pdf --export-width=100

glossary:
	makeindex document.nlo  -s nomencl.ist -o document.nls

biblio:
	bibtex document.aux

full: clean
	make buildmute
	make buildmute
	make buildmute
	make glossary
	make biblio
	make buildmute
	make buildmute
	make buildmute

final: svg2pdf
	make full
	# You need pdftk in order to remove the second page from this document
	pdftk document.pdf cat 1 3-end output document_ready.pdf

slides:
	pdflatex soutenance.tex > /dev/null

svg2pdf_slides:
	inkscape -z -D --file=images/piga_evol.svg --export-pdf=images/piga_evol.pdf --export-width=100

slides_final: svg2pdf_slides
	make slides && make slides && make slides

