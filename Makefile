library_name=libtd.so

example_name=example

source_dir=./source/td

include_dir=./include/td

library:
	g++ \
	$(source_dir)/IOSAL.cpp \
	$(source_dir)/LinuxOSAL.cpp \
	$(source_dir)/ConsoleOutput.cpp \
	$(source_dir)/PlainFileOutput.cpp \
	$(source_dir)/Td.cpp \
	$(source_dir)/HTMLFileOutput.cpp \
	-Werror -Iinclude -shared -fPIC -o $(library_name)
	
example:
	g++ $(source_dir)/example.cpp -Werror -Iinclude -ltd -o $(example_name) -L.
	
all: library example
	
clean:
	rm -f $(library_name)
	rm -f $(example_name)

install:
	cp $(include_dir)/td.h /usr/include/
	cp $(library_name) /usr/lib/

	
uninstall:
	rm /usr/include/td.h
	rm /usr/lib/$(library_name)
	