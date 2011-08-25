
rm INDEX/STATUS.txt
rm INDEX/files.txt
rm INDEX/filesmag.txt
rm INDEX/donecrawl

for i in a b c d e f g h i j k l m n o p q r s t u v w x y z
	do
		file="INDEX/"$i"/*"
		rm -f $file
	done
