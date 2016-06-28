# BTW, Huffman e Run-length isolados
# Huffman + Run Lenght (estudar melhor ordem)
# BTW =>Huffman
# BTW => RunLength
# BTW => RunLength => Huffman  (Estudar melhor ordem)

#tamanho dos blocos: 100,500,1000,10000

BASESDIR=./Bases/

BASESDIR_DNA=$(BASESDIR)/dna/
RESULTSDIR_DNA=$(BASESDIR_DNA)results/

BASESDIR_ENG=$(BASESDIR)/english/
RESULTSDIR_ENG=$(BASESDIR_ENG)results/

BASESDIR_XML=$(BASESDIR)/xml/
RESULTSDIR_XML=$(BASESDIR_XML)results/

ITERATE=./iterate.sh

all: 
	echo ola
runXML:
	sh $(ITERATE) $(BASESDIR_XML)
	cd $(RESULTSDIR_XML) ; \
		wc -c * > sizes.txt

runENG:
	sh $(ITERATE) $(BASESDIR_ENG)
	cd $(RESULTSDIR_ENG) ; \
		wc -c * > sizes.txt

runDNA:
	sh $(ITERATE) $(BASESDIR_DNA)
	cd $(RESULTSDIR_DNA) ; \
		wc -c * > sizes.txt



