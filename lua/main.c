// include psp stuff
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

// include commons stuff
#include "../common/callback.h"

// include lua stuff
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "lua_lib.h"


// configure PSP stuff
#define VERS    1
#define REVS    0

PSP_MODULE_INFO("LUATEST", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER | THREAD_ATTR_VFPU); 

// make printing easier on us
#define printf pspDebugScreenPrintf


static void stackDump(lua_State *L) {
	int i=lua_gettop(L);
	printf(" ----------------  Stack Dump ----------------\n" );
	while(i) {
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING:
			printf("%d:`%s'\n", i, lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%d: %s\n",i,lua_toboolean(L, i) ? "true" : "false");
		break;
		case LUA_TNUMBER:
			printf("%d: %g\n",  i, lua_tonumber(L, i));
		break;
		default: printf("%d: %s\n", i, lua_typename(L, t)); break;
		}
		i--;
	}
	printf("--------------- Stack Dump Finished ---------------\n" );
}

const char* scriptPath = "ms0:/PSP/GAME/test/script.lua";

int main(int argc, char** argv)
{
	// basic init
	setupExitCallback();
	pspDebugScreenInit();
	
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	sceDisplayWaitVblankStart();
	pspDebugScreenSetXY(0, 0);
	
	// script location
	//const char* scriptPath = "umd0:/script.lua";
	
	// init Lua, its libraries, and do a dry run for initialization
	lua_State *L = lua_lib_newstate();
	
	printf("New state success\n");
	
	// cache the file in lua registry
	int status = 0;
	status = luaL_loadfile(L, scriptPath);
	if(status != 0) {
		// break for alerting we have a compilation error
		stackDump(L);
		while(isRunning()) {
			sceDisplayWaitVblankStart();
			printf("\nerror code %d: %s\n", status, lua_tostring(L, -1));
		}
		sceKernelExitGame();
	}
	
	printf("Cached and loaded successfully\n");
	
	int lua_script = luaL_ref(L, LUA_REGISTRYINDEX);
	printf("Got it from registry\n");
	
	// run main logic, call script, check for errors
	printf("Liftoff\n");
	lua_rawgeti(L, LUA_REGISTRYINDEX, lua_script); // get script into stack
	printf("Rawgot\n");
	status = lua_pcall(L, 0, 0, 0); // run the script
	printf("Ran script, done\n");
	if(status != 0) {
		// alerting that we have a runtime error
		stackDump(L); // a dirty stack with error string
		printf("\n%s\n", lua_tostring(L, -1)); // print last push, ie the error
		lua_pop(L, 1); // pop the error arg
		stackDump(L); // a clean stack
		while(isRunning()) {
			sceDisplayWaitVblankStart();
			printf("Runtime error\n");
		}
	}
	
	printf("Friendly and Fresh\n");
	
	// clean up
	lua_close(L);
	sceKernelExitGame();
	return 0;
}

