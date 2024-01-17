CC=gcc
CFLAGS=-I.
DEPS = func.h
OBJ = GetComm.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rfid-testtool: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(OBJ)