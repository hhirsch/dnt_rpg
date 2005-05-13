# these are the subsystems to visit
SUBSYSTEMS = gui engine app

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
