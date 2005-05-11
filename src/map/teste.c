#include "map.h"
int main( int argc, char ** argv ){
	Map * mapa = new(Map);
	(*mapa).open();
	(*mapa).run_test();
	return(0);
}
