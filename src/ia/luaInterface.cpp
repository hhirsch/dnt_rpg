#include "luaInterface.h"

/*************************************************************
 *                         Constructor                       *
 *************************************************************/
luaInterface::luaInterface()
{
  module = "";
  error = 0;
  state = luaL_newstate();   // Open Lua
  luaL_openlibs(state); // Loads all LUA libraries
}

/*************************************************************
 *                          Destructor                       *
 *************************************************************/
luaInterface::~luaInterface()
{
  lua_close(state);
}

/*************************************************************
 *                         loadModule                        *
 *************************************************************/
void luaInterface::loadModule(string m)
{
  module = m;
  error = luaL_dofile(state, module.c_str());
  printf("error = %d\n", error);
  if (error == 0)
  {
    printf("Module %s loaded succesfully\n", module.c_str());
  }
}

/*************************************************************
 *                            getTop                         *
 *************************************************************/
int luaInterface::getTop()
{
  return lua_gettop(state);
}

/*************************************************************
 *                             pop                           *
 *************************************************************/
void luaInterface::pop(int n)
{
  lua_pop(state, n);
}

/*************************************************************
 *                          getGlobal                        *
 *************************************************************/
void luaInterface::getGlobal(string name)
{
  lua_getglobal(state, name.c_str());
}

/*************************************************************
 *                          getField                         *
 *************************************************************/
void luaInterface::getField(int index, string field)
{
  lua_getfield(state, index, field.c_str());
}

/*************************************************************
 *                            call                           *
 *************************************************************/
void luaInterface::call(int numOfArgs, int numOfReturns)
{
  lua_call(state, numOfArgs, numOfReturns);
}

/*************************************************************
 *                             push                          *
 *************************************************************/
void luaInterface::push(string s)
{
  lua_pushstring(state, s.c_str());
}

/*************************************************************
 *                             push                          *
 *************************************************************/
void luaInterface::push(int i)
{
  lua_pushinteger(state, i);
}

/*************************************************************
 *                          getValue                         *
 *************************************************************/
string luaInterface::getValue()
{
  return(lua_tostring(state, -1));
}

