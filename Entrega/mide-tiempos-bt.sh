#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bt.dat

i=1
while [ "$i" -le 15 ]
do
	# Generamos los puntos	
	./generador $i data_bt.txt
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bt.txt >> dist_bt.dat

	echo "Terminado $i" 
	
	i=$(( $i + 1 ))
done
