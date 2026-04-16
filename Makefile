FILE=main.c
OUT=main


all: 
	cc -g -pthread $(FILE) -o $(OUT) && ./$(OUT)

compile:
	cc -g -pthread $(FILE) -o $(OUT)