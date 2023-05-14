#!/bin/bash 
#echo "" >> salida.dat
printf "" > dist_bt.dat

i=100
while [ "$i" -le 3000 ]
do
	# Generamos los puntos	
	./generador $i
	printf "PUNTOS GENERADOS\t"
  	
	# Ejecutamos los puntos
	./bt data_bt.txt >> dist_bt.dat

	echo "Terminado $i" 
	
	i=$(( $i + 100 ))
done
