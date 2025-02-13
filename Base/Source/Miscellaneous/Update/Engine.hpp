/* Updated for version-080ad6451df24461 */

#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>

#define REBASE(x) x + (uintptr_t)GetModuleHandle(nullptr)
#define MAX_CAPABILITIES (uintptr_t)(0x200000000000003FLL | 0x3FFFFFF00LL)

struct lua_State;
struct Proto;

namespace Update {
    const uintptr_t GetTaskScheduler = REBASE(0x2D7B8A0); // Done

    const uintptr_t GetGlobalStateForInstance = REBASE(0xD72200); // Done
    const uintptr_t DecryptState = REBASE(0xB34EA0); // Done

    const uintptr_t GetContextObject = REBASE(0xBBAAB0); // Done
    const uintptr_t SetProtoCapabilities = REBASE(0xC52590); // Done

    const uintptr_t LuaVM__Load = REBASE(0xB37E80); // Done
    const uintptr_t Task__Defer = REBASE(0xF0C9A0); // Done

    const uintptr_t ContextObject = 0x648; // Done

    namespace ExtraSpace {
        const uintptr_t Identity = 0x30;
        const uintptr_t Capabilities = 0x48;
        const uintptr_t Script = 0x50;
        const uintptr_t Actor = 0x58;
    }

    namespace TaskScheduler {
        const uintptr_t JobsStart = 0x198;
        const uintptr_t JobName = 0x90;

        const uintptr_t ScriptContextJob = 0x1E8;
        const uintptr_t RenderView = 0x218;
    }

    namespace ScriptContext {
        const uintptr_t GlobalState = 0x120;
        const uintptr_t DecryptState = 0x88;
    }

    namespace DataModel {
        const uintptr_t Padding = 0x118;
        const uintptr_t Instance = 0x1A8;

        const uintptr_t PlaceId = 0x170;
        const uintptr_t ScriptContext = 0x3C0;

        namespace Information {
            const uintptr_t ClassDescriptor = 0x18;
            const uintptr_t ClassName = 0x8;

            const uintptr_t Children = 0x70;
            const uintptr_t Parent = 0x10;
        }
    }
}

namespace RBX {
    using TGetTaskScheduler = uintptr_t(__fastcall*)();
    inline auto GetTaskScheduler = (TGetTaskScheduler)Update::GetTaskScheduler;

    using TGetGlobalStateForInstance = uintptr_t(__fastcall*)(uintptr_t, uintptr_t*, uintptr_t*);
    inline auto GetGlobalStateForInstance = (TGetGlobalStateForInstance)Update::GetGlobalStateForInstance;

    using TDecryptState = lua_State*(__fastcall*)(uintptr_t);
    inline auto DecryptState = (TDecryptState)Update::DecryptState;

    using TGetContextObject = uintptr_t(__fastcall*)(uintptr_t, uintptr_t);
    inline auto GetContextObject = (TGetContextObject)Update::GetContextObject;

    using TSetProtoCapabilities = void(__fastcall*)(Proto*, uintptr_t);
    inline auto SetProtoCapabilities = (TSetProtoCapabilities)Update::SetProtoCapabilities;

    using TLuaVM__Load = int(__fastcall*)(lua_State*, void*, const char*, int);
    inline auto LuaVM__Load = (TLuaVM__Load)Update::LuaVM__Load;

    using TTask__Defer = int(__fastcall*)(lua_State*);
    inline auto Task__Defer = (TTask__Defer)Update::Task__Defer;

    inline __int64 GetThreadIdentity(uintptr_t L) {
        uintptr_t Userdata = *(uintptr_t*)(L + 0x78);
        return *(__int64*)(Userdata + Update::ExtraSpace::Identity);
    }

    inline void SetThreadIdentity(uintptr_t L, uintptr_t Identity) {
        uintptr_t Userdata = *(uintptr_t*)(L + 0x78);
        *(__int64*)(Userdata + Update::ExtraSpace::Identity) = Identity;
    }

    inline void SetThreadCapabilities(uintptr_t L, uintptr_t Capabilities) {
        uintptr_t Userdata = *(uintptr_t*)(L + 0x78);
        *(__int64*)(Userdata + Update::ExtraSpace::Capabilities) = Capabilities;
    }
}