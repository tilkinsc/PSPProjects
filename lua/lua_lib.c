#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#include "lua_lib.h"

#include "../common/callback.h"
#include "../common/ui.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#define printf pspDebugScreenPrintf

extern int lua_print(lua_State *L)
{
	int arg_len = lua_gettop(L);
	int n;
	for (n=1; n <= arg_len; n++)
		printf("%s%s", lua_tostring(L, n), "\t");
	printf("\n");
	return 0;
}

extern int lua_isRunning(lua_State* L)
{
	lua_pushboolean(L, isRunning());
	return 1;
}

extern int lua_setDebugScreen(lua_State* L)
{
	int arg_len = lua_gettop(L);
	pspDebugScreenSetXY(lua_tonumber(L, 1), lua_tonumber(L, 2));
	return 0;
}

extern int lua_vblank(lua_State* L)
{
	sceDisplayWaitVblankStart();
	return 0;
}

extern int lua_getJX(lua_State* L)
{
	lua_pushinteger(L, (int) getJX());
	return 1;
}

extern int lua_getJY(lua_State* L)
{
	lua_pushinteger(L, (int) getJY());
	return 1;
}

extern int lua_pollPad(lua_State* L)
{
	pollPad();
	return 0;
}

extern int lua_pollLatch(lua_State* L)
{
	pollLatch();
	return 0;
}

extern int lua_isKeyDown(lua_State* L)
{
	int arg_len = lua_gettop(L);
	
	int arg1 = luaL_checkinteger(L, 1);
	
	lua_pushboolean(L, isKeyDown(arg1));
	return 1;
}

extern int lua_isKeyUp(lua_State* L)
{
	int arg_len = lua_gettop(L);
	
	int arg1 = luaL_checkinteger(L, 1);
	
	lua_pushboolean(L, isKeyUp(arg1));
	return 1;
}

extern int lua_isKeyHold(lua_State* L)
{
	int arg_len = lua_gettop(L);
	
	int arg1 = luaL_checkinteger(L, 1);
	
	lua_pushboolean(L, isKeyHold(arg1));
	return 1;
}


lua_State* lua_lib_newstate()
{
	lua_State* L;
	L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_register(L, "print", lua_print);
	lua_register(L, "isRunning", lua_isRunning);
	lua_register(L, "setDebugScreen", lua_setDebugScreen);
	lua_register(L, "vblank", lua_vblank);
	
	
	lua_register(L, "pollPad", lua_pollPad);
	lua_register(L, "pollLatch", lua_pollLatch);
	
	lua_register(L, "getJX", lua_getJX);
	lua_register(L, "getJY", lua_getJY);
	
	lua_register(L, "isKeyDown", lua_isKeyDown);
	lua_register(L, "isKeyUp", lua_isKeyUp);
	lua_register(L, "isKeyHold", lua_isKeyHold);
	
	return L;
}


