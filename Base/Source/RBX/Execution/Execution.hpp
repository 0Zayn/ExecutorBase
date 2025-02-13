#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <array>

#include <Luau/Compiler.h>
#include <Luau/BytecodeBuilder.h>
#include <Luau/BytecodeUtils.h>
#include <Luau/Bytecode.h>

#include <lstate.h>
#include <lapi.h>
#include <lualib.h>
#include <lgc.h>

#include <zstd/zstd.h>
#include <zstd/xxhash.h>

#include <RBX/RBX.hpp>
#include <Update/Engine.hpp>

class CBytecodeEncoder : public Luau::BytecodeEncoder {
    inline void encode(uint32_t* Data, size_t Count) override {
        for (auto i = 0u; i < Count;) {
            auto& Opcode = *(uint8_t*)(Data + i);
            i += Luau::getOpLength(LuauOpcode(Opcode));
            Opcode *= 227;
        }
    }
};

class CExecution {
public:
    void SetProtoCaps(Proto* Parent, uintptr_t* Caps);

    lua_State* NewThread(lua_State* L);
    void Send(lua_State* L, std::string Source, bool Compile = true, int Yield = 0);

private:
    std::string CompileScript(const std::string Source);
    CBytecodeEncoder Encoder;
};

inline auto Execution = std::make_unique<CExecution>();