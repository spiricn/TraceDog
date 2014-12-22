out=libtd.so

all:
	gcc source/td/td.c -Iinclude -shared -fPIC -o $(out)
	
clean:
	rm -f $(out)
	rm -f ./example

example:
	gcc source/td/example.c -Iinclude -o example -ltd

install:
	cp -r include/td /usr/include/
	cp $(out) /usr/lib/
