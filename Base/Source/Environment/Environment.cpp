#include "Environment.hpp"

void CEnvironment::Initialize(lua_State* L) {
	lua_newtable(L);
	lua_setglobal(L, "_G");

	lua_newtable(L);
	lua_setglobal(L, "shared");
}