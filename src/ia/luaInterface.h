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

/*! The Interface of the Engine with LUA Scripts */
class luaInterface
{
  public:
    /*! Constructor */
    luaInterface();
    /*! Desctructor */
    ~luaInterface();

    /*! Load a Lua Module to the interface
     * \param m -> filename of the module to load */
    void loadModule(string m);

    /*! Get the top of the stack
     * \return top of the stack */
    int getTop();
    void pop(int n);
    void getGlobal(string name);
    void getField(int index, string field);
    void call(int numOfArgs, int numOfReturns);
    void push(string s);
    void push(int i);
    string getValue();

  private:
    int error;          /**< Last Error on Lua */
    lua_State* state;   /**< The state of the Lua */
    string module;      /**< FileName of the loaded module */

};

#endif
