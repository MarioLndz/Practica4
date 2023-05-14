#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bt.dat

i=10
while [ "$i" -le 200 ]
do
	# Generamos los puntos	
	./generador $i
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bt.txt >> dist_bt.dat

	echo "Terminado $i" 
	
	i=$(( $i + 5 ))
done
