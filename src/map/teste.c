#include "map.h"
int main( int argc, char ** argv ){
	Map * mapa = new(Map);
	(*mapa).open("../data/mapas/arquivo.map");
/*	(*mapa).run_test();*/
        delete(mapa);
	return(0);
}
