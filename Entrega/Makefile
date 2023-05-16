# Nombre archivo: Makefile
# Uso: make
# Descripción:	Construye los tres binarios de nombres bt, bb y generador, respectivamente.

#------------------------------------------------------------------#

# Variable que indica el compilador que se va a utilizar
CC=g++

# Variable que indica el directorio del proyecto bb
BB= ./BB

# Variable que indica el directorio del proyecto bt
BT= ./BT

# Variable que indica el directorio del proyecto Generador
GENERADOR_DIR= ./Generador

#------------------------------------------------------------------#

all: bb bt generador

# ---------- Backtracking ----------
bb: $(BB)/bb.cpp
	$(CC) $(BB)/bb.cpp -o bb

# ---------- Branch&Bound ----------
bt: $(BT)/bt.cpp
	$(CC) $(BT)/bt.cpp -o bt

# ---------- Generador ----------
generador: $(GENERADOR_DIR)/generador.cpp
	$(CC) $(GENERADOR_DIR)/generador.cpp -o generador
