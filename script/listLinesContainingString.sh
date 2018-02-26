# Make sure that there is a path to search
if [ -z "$1" ] ; then
	echo Expected directory path containing files to search as fist parameter
	exit 1
fi

# Make sure that there is a search string
if [ -z "$2" ] ; then
	echo Expected string to search for as second parameter
	exit 1
fi

# Check if an output file path was supplied
[ -z "$3" ] ; outputPathSupplied=$?
if [ $outputPathSupplied -eq 0 ] ; then
	printf "No output file path supplied as third parameter; will echo instead\n\n"
fi

occuranceCount=$(grep --include=*.cpp --include=*.h -ir "$2" "$1"/ | wc -l) # Count how many lines the string occurs in (ignore case, search recursively )
lines=$(grep --include=*.cpp --include=*.h -iHnr "$2" "$1"/) # Get the lines the string occurs in ((ignore case, search recursively, include file name and line ))
result="Count = ""$occuranceCount\n\n$lines"

# Echo result or print it to output file
if [ $outputPathSupplied -eq 1 ] ; then
	printf "$result" > "$3"
else
	echo "$result" 
fi