#ifndef _luaInterface_h
#define _luaInterface_h

extern "C"
{
  #include "lua.hpp"
  #include "lauxlib.h"
  #include "lualib.h"
}

#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#include "../engine/personagens.h"

void playPlanning(string planning, character* dude);

#endif
