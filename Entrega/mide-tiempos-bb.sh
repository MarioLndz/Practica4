#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bb.dat

i=1
while [ "$i" -le 15 ]
do
	# Generamos los puntos	
	./generador $i data_bb.txt
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bb.txt >> dist_bb.dat

	echo "Terminado $i" 
	
	i=$(( $i + 1 ))
done
