/* Updated for version-080ad6451df24461 */

#pragma once

#include <Windows.h>
#include <cmath>

#include "Core/Structure.hpp"

#define REBASE(x) x + (uintptr_t)GetModuleHandle(nullptr)

#define LUAU_COMMA_SEP ,
#define LUAU_SEMICOLON_SEP ;

#define LUAU_SHUFFLE3(s, a1, a2, a3) a2 s a1 s a3 // Done
#define LUAU_SHUFFLE4(s, a1, a2, a3, a4) a1 s a2 s a4 s a3 // Done
#define LUAU_SHUFFLE5(s, a1, a2, a3, a4, a5) a1 s a5 s a2 s a3 s a4 // Done
#define LUAU_SHUFFLE6(s, a1, a2, a3, a4, a5, a6) a2 s a4 s a3 s a1 s a5 s a6 // Done
#define LUAU_SHUFFLE7(s, a1, a2, a3, a4, a5, a6, a7) a7 s a2 s a6 s a3 s a5 s a4 s a1 // Done
#define LUAU_SHUFFLE8(s, a1, a2, a3, a4, a5, a6, a7, a8) a5 s a1 s a8 s a3 s a4 s a2 s a7 s a6 // Done
#define LUAU_SHUFFLE9(s, a1, a2, a3, a4, a5, a6, a7, a8, a9) a4 s a7 s a6 s a5 s a2 s a3 s a1 s a9 s a8 // Done

#define PROTO_MEMBER1_ENC VMValue3 // Done
#define PROTO_MEMBER2_ENC VMValue2 // Done
#define PROTO_DEBUGISN_ENC VMValue4 // Done
#define PROTO_TYPEINFO_ENC VMValue1 // Done
#define PROTO_DEBUGNAME_ENC VMValue3 // Done

#define LSTATE_STACKSIZE_ENC VMValue1 // Done
#define LSTATE_GLOBAL_ENC VMValue1 // Done

#define CLOSURE_FUNC_ENC VMValue3 // Done
#define CLOSURE_CONT_ENC VMValue4 // Done
#define CLOSURE_DEBUGNAME_ENC VMValue2 // Done

#define TABLE_MEMBER_ENC VMValue3 // Done
#define TABLE_META_ENC VMValue3 // Done

#define UDATA_META_ENC VMValue1 // Done

#define TSTRING_HASH_ENC VMValue2 // Done
#define TSTRING_LEN_ENC VMValue1 // Done

#define GSTATE_TTNAME_ENC VMValue1 // Done
#define GSTATE_TMNAME_ENC VMValue1 // Done

namespace Update {
    namespace LuaVM {
        const uintptr_t LuaO_NilObject = REBASE(0x4172E58); // Done
        const uintptr_t LuaH_DummyNode = REBASE(0x4172408); // Done
        const uintptr_t Luau_Execute = REBASE(0x233F140);// Done
    }
}