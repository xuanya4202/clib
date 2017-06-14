CC=gcc
LIB= -pthread
INC=
OUT=net
$OUT:
	$(CC) $(LIB) $(INC) main.c -o $(OUT)
clean:
	rm *.o -rf
	rm $(OUT) -rf
