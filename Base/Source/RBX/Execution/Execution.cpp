#include "Execution.hpp"

lua_State* CExecution::NewThread(lua_State* L) {
    if (L == nullptr || L->tt != LUA_TTHREAD)
        return nullptr;

    luaC_checkGC(L);
    luaC_threadbarrier(L);

    int StackCount = lua_gettop(L);
    if (StackCount > 0)
        lua_pop(L, StackCount);

    lua_State* L1 = luaE_newthread(L);
    if (L1 == nullptr)
        return nullptr;

    if (L1->tt != LUA_TTHREAD)
        return nullptr;

    setthvalue(L, L->top, L1);
    incr_top(L);

    global_State* G = L->global;
    if (G->cb.userthread != nullptr)
        G->cb.userthread(L, L1);

    return L1;
}

std::string CExecution::CompileScript(const std::string Source) {
    std::string Bytecode = Luau::compile(Source, { 2, 1, 2 }, { true, true }, &Encoder);

    size_t DataSize = Bytecode.size();
    size_t MaxSize = ZSTD_compressBound(DataSize);
    std::vector<char> Buffer(MaxSize + 8);

    memcpy(Buffer.data(), "RSB1", 4);
    memcpy(Buffer.data() + 4, &DataSize, sizeof(DataSize));

    size_t CompressedSize = ZSTD_compress(Buffer.data() + 8, MaxSize, Bytecode.data(), DataSize, ZSTD_maxCLevel());
    size_t TotalSize = CompressedSize + 8;

    uint32_t Key = XXH32(Buffer.data(), TotalSize, 42);
    uint8_t* KeyBytes = (uint8_t*)&Key;

    for (size_t i = 0; i < TotalSize; ++i) Buffer[i] ^= KeyBytes[i % 4] + i * 41;

    return std::string(Buffer.data(), TotalSize);
}

void CExecution::Send(lua_State* L, std::string Source, bool Compile, int Yield) {
    if (Source.empty() || !L) return;

    if (Yield > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(Yield));

    lua_State* Thread = NewThread(L);
    if (!Thread) return;

    luaL_sandboxthread(Thread);
    RBX::SetThreadIdentity((uintptr_t)Thread, 8);
    RBX::SetThreadCapabilities((uintptr_t)Thread, MAX_CAPABILITIES);

    lua_getglobal(Thread, "Instance");
    lua_getfield(Thread, -1, "new");
    lua_pushstring(Thread, "LocalScript");

    if (lua_pcall(Thread, 1, 1, 0) != LUA_OK) {
        lua_pop(Thread, lua_gettop(Thread));
        return;
    }

    lua_ref(Thread, -1);
    lua_setglobal(Thread, "script");
    lua_pop(Thread, 1);

    std::string Script = Compile ? CompileScript(Source) : Source;
    int LoadResult = RBX::LuaVM__Load(Thread, &Script, "@Base", 0);

    if (LoadResult == LUA_OK) {
        if (Closure* Cl = clvalue(luaA_toobject(Thread, -1))) {
            if (Proto* P = Cl->l.p) {
                uintptr_t ScriptContext = RBX::Scheduler->GetScriptContext();
                uintptr_t ContextObject = RBX::GetContextObject(ScriptContext + Update::ContextObject, MAX_CAPABILITIES);

                RBX::SetProtoCapabilities(P, ContextObject);
            }
        }

        RBX::Task__Defer(Thread);
    }
    else {
        std::string Error = luaL_optstring(Thread, -1, "Unknown error occurred");
        lua_getglobal(Thread, "warn");
        lua_pushlstring(Thread, Error.c_str(), Error.size());
        lua_call(Thread, 1, 0);
    }

    lua_pop(Thread, 1);
}
