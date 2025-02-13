#include "Scheduler.hpp"

using namespace RBX;

using JobOriginalVF = uintptr_t(__fastcall*)(uintptr_t A1, uintptr_t A2, uintptr_t A3);

static JobOriginalVF OriginalVF = {};
static std::vector<std::string> ScriptQueue;

void CScheduler::UpdateJobs() {
    Jobs.clear();

    uintptr_t JobsStart = *(uintptr_t*)(Address + Update::TaskScheduler::JobsStart);
    uintptr_t JobsEnd = *(uintptr_t*)(Address + Update::TaskScheduler::JobsStart + sizeof(void*));

    for (auto i = JobsStart; i < JobsEnd; i += 0x10) {
        uintptr_t Job = *(uintptr_t*)i;
        if (!Job) continue;

        std::string* JobName = reinterpret_cast<std::string*>(Job + Update::TaskScheduler::JobName);
        if (JobName && JobName->length() > 0) Jobs.push_back(Job);
    }
}

uintptr_t CScheduler::GetJobByName(const std::string& Name) {
    for (auto Job : Jobs) {
        if (*(std::string*)(Job + Update::TaskScheduler::JobName) == Name)
            return Job;
    }

    return 0;
}

uintptr_t CScheduler::GetScriptContext() {
    auto Children = *(uintptr_t*)(GetDataModel() + Update::DataModel::Information::Children);
    auto End = *(uintptr_t*)(Children + sizeof(uintptr_t));

    for (auto i = *(uintptr_t*)(Children); i < End; i += sizeof(uintptr_t) * 2) {
        auto String = *(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(i)+Update::DataModel::Information::ClassDescriptor) + Update::DataModel::Information::ClassName);

        if (*(int32_t*)(String + Update::DataModel::Information::ClassDescriptor) >= 16u) 
            String = *(uintptr_t*)(String);

        if (std::string((char*)(String)) == "ScriptContext") 
            return *(uintptr_t*)(i);
    }

    return 0;
}

uintptr_t CScheduler::GetRenderView() {
    return *(uintptr_t*)(GetJobByName("RenderJob") + Update::TaskScheduler::RenderView);
}

uintptr_t CScheduler::GetVisualEngine() {
    return *(uintptr_t*)(GetRenderView() + Update::DataModel::Information::Parent);
}

uintptr_t CScheduler::GetDataModel() {
    uintptr_t Padding = *(uintptr_t*)(GetRenderView() + Update::DataModel::Padding);
    return *(uintptr_t*)(Padding + Update::DataModel::Instance);
}

uintptr_t CScheduler::Cycle(uintptr_t A1, uintptr_t A2, uintptr_t A3) {
    lua_State* L = Manager->GetLuaState();
    if (!L) return OriginalVF(A1, A2, A3);

    if (!ScriptQueue.empty()) {
        std::string Script = ScriptQueue.front();
        ScriptQueue.erase(ScriptQueue.begin());

        if (!Script.empty())
            Execution->Send(L, Script);
    }

    return OriginalVF(A1, A2, A3);
}

void CScheduler::HookJob(const std::string& Name) {
    uintptr_t Job = GetJobByName(Name);
    if (!Job) return;

    void** VTable = new void* [25]();
    memcpy(VTable, *(void**)Job, sizeof(uintptr_t) * 25);

    OriginalVF = (JobOriginalVF)VTable[2];
    VTable[2] = Cycle;

    *(void**)Job = VTable;
}

void CScheduler::ScheduleScript(const std::string& Script) {
    ScriptQueue.push_back(Script);
}

void CScheduler::Initialize() {
    Address = RBX::GetTaskScheduler();
    UpdateJobs();
}