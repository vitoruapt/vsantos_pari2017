#! /bin/sh
#
# Script to generate cflow of C files
# transform them into vcg/dot (through cflow2vcg/cflow2dot) and displays with graphviz dot
#
# Had to the following installations:
#
#   sudo apt install cflow
#   Download cflow2vcg from http://cflow2vcg.sourceforge.net/
#     Unzip/untar/make and make install
#     Needed to install this one for documentation:
#     	sudo apt install docbook-to-man
#
#
#
     
#cflow --format=posix --omit-arguments \
      #--level-indent='0=\t' --level-indent='1=\t' \
      #--level-indent=start='\t' $* | cflow2vcg | xvcg -

#cflow --format=posix --omit-arguments \
#      --level-indent='0=\t' --level-indent='1=\t' \
#      --level-indent=start='\t' $* | cflow2dot | dot -Tps -o graph1.ps



#cflow --format=posix --omit-arguments \
#      --level-indent='0=\t' --level-indent='1=\t' \
#      --level-indent=start='\t' $* > graph1.cflow



#The main  (OK stable)
cflow  --format=posix --omit-arguments --level-indent='0=\t' --level-indent='1=\t' --level-indent=start='\t' main.c > A12codemain.cflow 

#The callbacks without "main" and
cflow  \
	-d 3\
	--format=posix --omit-arguments --level-indent='0=\t' --level-indent='1=\t' --level-indent=start='\t' callbacks.c > A12codecallbacksL3.cflow 

#The auxiliary without "main"
cflow  \
	-d 2 \
	--format=posix --omit-arguments --level-indent='0=\t' --level-indent='1=\t' --level-indent=start='\t' myf.c > A12codemyfL2.cflow 

#the da up to level3 (OK stable)
cflow   \
	-d 3 \
        --main=on_drawingarea1_expose_event\
	--format=posix --omit-arguments --level-indent='0=\t' --level-indent='1=\t' --level-indent=start='\t'\
        *.c > A12codedacallbackL3.cflow

#----------------------------------------------------------
#  Process the cflow files and generate the dot and utilamtely the graphic images

cFiles=*.cflow

#Remove some non-relevant "functions"
sed -i '/GTK_/ d' $cFiles
sed -i '/G_/ d'  $cFiles
sed -i '/printf/ d' $cFiles
sed -i '/g_print/ d' $cFiles
sed -i '/puts/ d' $cFiles
sed -i '/exit/ d' $cFiles

for f in *.cflow
do
	bName=`echo $f | sed 's/\.cflow$//'`
	cflow2dot < $f > $bName.dot
done

dFiles=*.dot

#Fine tune the dot files before rendering
#remove useless info
sed -i 's/\\n\\n:0//' $dFiles
#remove line number references
sed -i 's/\.c:[0-9]*/\.c/' $dFiles
#sed -i 's/shape=box/shape=box/' $dFiles

oFormat=svg   #ps,png,pdf, etc.
for f in *.dot
do
	bName=`echo $f | sed 's/\.dot$//'`
	dot -T$oFormat -Grankdir=LR -Nfontname=Helvetica -Nfontsize=10 $f -o $bName.$oFormat
done


mv *.svg images
rm *.dot *.cflow





