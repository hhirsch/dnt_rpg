#  DccNiTghtmare is public domain. Do whatever you want with this code.

SUBSYSTEMS = src

# type 'make all' to visit and build subsystems
all: buildparts

buildparts:
	for dir in ${SUBSYSTEMS}; do \
	   (cd $$dir; ${MAKE} all); \
	done

clean:
	for dir in $(SUBSYSTEMS);do \
	(cd $$dir; ${MAKE} clean); \
	done
