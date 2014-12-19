out=libtd.so

all:
	gcc source/td/td.c -Iinclude -shared -fPIC -o $(out)

clean:
	rm -f $(out)

install:
	cp -r include/td /usr/include/
	cp $(out) /usr/lib/
