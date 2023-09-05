#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include <stdlib.h>
#include <string.h>

lua_State *L;

/*
    Run a lua script by its path
*/
void runscript(char*name) {
    // Load and run a Lua script
    if (luaL_loadfile(L, name) || lua_pcall(L, 0, 0, 0)) {
        fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        exit(1);
    }
}

/*
    Temporary test function for lua
*/
int square(int x) {
    return x * x;
}

/*
    Bridge for lua talking to engine functions, its important that this
    is an int so lua can capture results onto its stack once this returns

    arg1: string of the function to call
    argX: variable based on what the function is
*/
int lua_bridge_call(lua_State* L){
    // get the function name
    const char* func = lua_tostring(L, 1);

    // begin checks for functions
    if(strcmp(func, "square") == 0){
        // call the function
        int x = lua_tointeger(L, 2);
        int result = square(x);
        // printf("result: %d\n", result);
        // push the result to the lua stack
        lua_pushinteger(L, result);
        // printf("pushed result to lua stack\n");
        return 1;
    }
    // TODO: extend lua interface API to actual engine functions
    // this will require some thoughts on where the game loop lives... 
    // how do we let lua hook into the state, does engine handle game loop
    // and repeated rendering, and lua can modify its model at runtime?

    return 0; // no valid function was supplied
}

void lua_init(){
    L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "lua_bridge_call", lua_bridge_call);
}

void lua_shutdown(){
    lua_close(L);
}