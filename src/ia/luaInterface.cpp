#include "luaInterface.h"

luaInterface::luaInterface()
{
  module = "";
  error = 0;
  state = luaL_newstate();   // Open Lua
  luaL_openlibs(state); // Loads all LUA libraries
}

luaInterface::~luaInterface()
{
  lua_close(state);
}

void luaInterface::loadModule(string m)
{
  module = m;
  error = luaL_loadfile(state, module.c_str());
  if (error == 0)
  {
    printf("Module %s loaded succesfully\n", module.c_str());
  }
}

int luaInterface::getTop()
{
  return lua_gettop(state);
}

void luaInterface::pop(int n)
{
  lua_pop(state, n);
}
