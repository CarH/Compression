reset
set term x11
set title t
cc1 = "#99ffff"; cc2 = "#4671d5"; cc3 = "#ff0000"; cc4 = "#f36e00"; cc5 = "blue";
set auto x
set yrange [0:1.5]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic scale 0
set grid
# 2, 3, 4, 5 are the indexes of the columns; 'fc' stands for 'fillcolor'
#plot 'dna-bwt.dat' using 2:xtic(1) ti col fc rgb cc1, '' u 3 ti col fc rgb cc2, '' u 4 ti col fc rgb cc3, '' u 5 ti col fc rgb cc4, '' u 6 ti col fc rgb cc5
plot df using 2:xtic(1) ti col fc rgb cc1, '' u 3 ti col fc rgb cc2, '' u 4 ti col fc rgb cc3, '' u 5 ti col fc rgb cc4, '' u 6 ti col fc rgb cc5

#print in postscript format:
set term postscript enhanced color dashed "Helvetica" 14
set output outfile
replot
