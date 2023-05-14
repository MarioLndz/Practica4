#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bb.dat

i=10
while [ "$i" -le 130 ]
do
	# Generamos los puntos	
	./generador $i
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bb.txt >> dist_bb.dat

	echo "Terminado $i" 
	
	i=$(( $i + 5 ))
done
