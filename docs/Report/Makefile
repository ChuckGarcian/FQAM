MAIN := flatex_path_probability_algo.tex
OUTDIR := output

all: 
	mkdir -p $(OUTDIR)
	pdflatex -output-directory=$(OUTDIR) -aux-directory=$(OUTDIR) -output-format=pdf -jobname=$(basename $@) $(MAIN)
	cp $(OUTDIR)/$(basename $@).pdf .

clean:
	rm -rf $(OUTDIR)
	rm -f $(MAIN:.tex=.pdf)