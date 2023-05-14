#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bb.dat

i=100
while [ "$i" -le 3000 ]
do
	# Generamos los puntos	
	./generador $i
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bb.txt >> dist_bb.dat

	echo "Terminado $i" 
	
	i=$(( $i + 100 ))
done
