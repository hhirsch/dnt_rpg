#ifndef _luaInterface_h
#define _luaInterface_h

#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

extern "C"
{
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}

class luaInterface
{
  public:
    luaInterface();
    ~luaInterface();
    void loadModule(string m);
    int getTop();
    void pop(int n);

  private:
    int error;
    lua_State* state;
    string module;

};

#endif
