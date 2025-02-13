#pragma once

#include <filesystem>
#include <iostream>

struct lua_State;

class CManager {
private:
	lua_State* L;

public:
	lua_State* GetLuaState() { return this->L; }
	void SetLuaState(lua_State* L) { this->L = L; }
};

inline auto Manager = std::make_unique<CManager>();