#include "luaInterface.h"

string locateAndGo(string object)
{
  printf("Locating the %s object...\n", object.c_str());
  //if(lookFor("bathroom"), go(x, y))
  return "done";
  // else
  // return "fail"
}

string doAction(string action)
{
  printf("Doing the %s action...\n", action.c_str());
  //if(action)
  return "done";
  // else
  // return "fail"
}

void idle()
{
  printf("Entering the idle state...");
}

int cppCalls(lua_State* state)
{
  string currentState = lua_tostring(state, -3);
  character* dude = (character*) lua_touserdata(state, -2);
  string args = lua_tostring(state, -1);
  string result;
  printf("currentState: %s\n", currentState.c_str());

  // Choses the action
  if(currentState == "locateAndGo")
  {
    result = locateAndGo(args);
  }
  else if(currentState == "doAction")
  {
    result = doAction(args);
  }
  else if(currentState == "idle")
  {
    // do nothing!
  }

  lua_pushstring(state, result.c_str());
  return 1;
  //if(fsmState =)
}

void playPlanning(string planning, character* dude)
{
  /* Open the Lua State Machine */
  lua_State* state = luaL_newstate();

  /* Open the Lua Libs */
  luaL_openlibs(state);

  /* Register the character as a variable into LUA, so it can pass it back as
   * an argument to the cppCalls function. 
   */
  lua_pushlightuserdata(state, (void*)dude);
  lua_setglobal(state, "dude");

  /* Register the cpp function to call the others */
  lua_register(state, "cppCalls", cppCalls);


  /* Plays the script */
  luaL_dofile(state, "../../data/ia/script/general/fsm.lua");
  luaL_dofile(state, planning.c_str());

  /* Closes the lua instance */
  lua_close(state);
  printf("Done running planning!\n");
}

