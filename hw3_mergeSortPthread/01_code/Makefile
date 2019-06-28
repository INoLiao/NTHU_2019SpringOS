hw3 hw3_serial: hw3.c hw3_serial.c
	gcc hw3_serial.c -o hw3_serial.o
	gcc hw3.c -pthread -o hw3.o

test:
	./hw3.o testcase.txt output.txt

test_serial:
	./hw3_serial.o testcase.txt output.txt

clean:
	rm -f *.o
