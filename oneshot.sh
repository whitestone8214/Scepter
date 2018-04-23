#!/bin/bash

# To build: ./oneshot.sh build
# To install: ./oneshot.sh install [root-directory]
# To link: ./oneshot.sh link [root-directory]
# To unlink: ./oneshot.sh unlink
# To uninstall: ./oneshot.sh uninstall


_versionScepter="1"
_editionScepter="1"
_destinationScepter="packages/scepter/${_versionScepter}-${_editionScepter}"

if (test "$1" = "build"); then
	rm -f scepter
	
	gcc $(pkg-config --cflags --libs no8177) scepter.c -o scepter
elif (test "$1" = "install"); then
	$0 uninstall || exit 1
	
	rm -rf $2/${_destinationScepter} || exit 1
	mkdir -p $2/${_destinationScepter} || exit 1
	
	cp -f scepter $2/${_destinationScepter} || exit 1
	chmod +s $2/${_destinationScepter}/scepter || exit 1
	
	cp -f scepter.ptn /etc || exit 1
elif (test "$1" = "link"); then
	$0 unlink || exit 1
	
	ln -sf $2/${_destinationScepter}/scepter /bin || exit 1
elif (test "$1" = "unlink"); then
	rm -f /bin/scepter || exit 1
elif (test "$1" = "uninstall"); then
	rm -rf $2/${_destinationScepter} || exit 1
	
fi
