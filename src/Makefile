# DccNiTghtmare is public domain. Do whatever you want with this code.
 
SUBSYSTEMS = gui etc map ia engine app

# type 'make all' to visit and build subsystems
all: buildparts

buildparts:
	echo "Compilando DccNiTghtmare..."
	for dir in ${SUBSYSTEMS}; do \
	   (echo "Compilando $$dir";echo " ";cd $$dir; ${MAKE} all;echo " "); \
	done

clean:
	for dir in $(SUBSYSTEMS);do \
	(echo "Apagando $$dir";cd $$dir; ${MAKE} clean); \
	done
