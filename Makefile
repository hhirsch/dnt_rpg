#  DccNiTghtmare is public domain. Do whatever you want with this code.

all: buildparts

buildparts:
	cd ./src && ${MAKE} all;

clean:
	cd ./src && ${MAKE} clean;
