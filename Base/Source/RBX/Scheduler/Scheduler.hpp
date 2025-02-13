#pragma once

#include <iostream>
#include <vector>

#include <Execution/Execution.hpp>
#include <Update/Engine.hpp>
#include <Manager.hpp>

namespace RBX {
    class CScheduler {
    private:
        uintptr_t Address = 0;
        std::vector<uintptr_t> Jobs;

    public:
        void UpdateJobs();
        uintptr_t GetJobByName(const std::string& Name);

        uintptr_t GetScriptContext();
        uintptr_t GetRenderView();
        uintptr_t GetVisualEngine();
        uintptr_t GetDataModel();

        static uintptr_t Cycle(uintptr_t A1, uintptr_t A2, uintptr_t A3);

        void HookJob(const std::string& Name);
        void ScheduleScript(const std::string& Script);

        void Initialize();
    };

    inline auto Scheduler = std::make_unique<CScheduler>();
}