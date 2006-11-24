#include "particles.h"

particles::particles(Map* map)
{
   actualMap = map;
}

particles::~particles()
{
   actualMap = NULL;
}

