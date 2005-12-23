# DccNiTghtmare is public domain. Do whatever you want with this code.
 
SUBSYSTEMS = gui etc map ia engine app

# type 'make all' to visit and build subsystems
all: buildparts

buildparts:
	@printf "Making DccNiTghtmare...\n"
	@for dir in ${SUBSYSTEMS}; do \
	   (echo "Making $$dir";echo " ";cd $$dir; ${MAKE} all;echo " "); \
	done
	@printf "\e[0;32mAll Done.\e[0;0m\n"

clean:
	@printf "Cleaning DccNiTghmare Bin Files...\n"
	@for dir in $(SUBSYSTEMS);do \
	(cd $$dir; ${MAKE} clean); \
	done
	@printf "\e[0;32mAll Done.\e[0;0m\n"

