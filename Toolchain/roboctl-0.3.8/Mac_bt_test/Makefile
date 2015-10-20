
bt_test:    bt_test.o
	cc -o bt_test bt_test.o -framework IOBluetooth -framework Carbon

bt_test.o:  bt_test.c
	cc -c -Wall bt_test.c

clean:
	rm -f bt_test *.o
