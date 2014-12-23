library_name=libtd.so

example_name=example

source_dir=./source/td

include_dir=./include/td

library:
	gcc $(source_dir)/td.c -Werror -Iinclude -shared -fPIC -o $(library_name)
	
example:
	gcc $(source_dir)/example.c -Werror -Iinclude -ltd -o $(example_name)
	
all: library example
	
clean:
	rm -f $(library_name)
	rm -f $(example_name)

install:
	cp $(include_dir)/td.h /usr/include/
	cp $(library_name) /usr/lib/
