
DIR=$1


for datafile in $DIR*.dat
do
	echo "path: "$datafile
	echo
	# echo "basename: "$(basename "$datafile" ".txt")
	# echo
	title=$(basename "$datafile" ".dat")

	gnuplot -e "df='${datafile}'; t='${title}'; outfile='${title}.ps'" gen-bar-chartc.gnuplot
	echo "gnuplot -e df="${datafile}"; t="${title}"; outfile="$title".ps gen-bar-chartc.gnuplot"
done
