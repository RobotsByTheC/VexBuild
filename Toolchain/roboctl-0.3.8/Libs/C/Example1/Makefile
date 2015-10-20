
LOCALBASE       ?= /usr/local

print_status: print_status.c
	cc -o print_status -I${LOCALBASE}/include print_status.c \
	    -L${LOCALBASE}/lib -llegoctl -lusb -lbluetooth

clean:
	rm -f print_status
