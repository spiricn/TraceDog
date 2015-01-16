#!/bin/bash

main(){
	makec clean
	makec all
	LD_LIBRARY_PATH=`pwd` ./example
}

main "$@"
