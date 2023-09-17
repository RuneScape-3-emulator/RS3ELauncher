//
// Created by david on 2023-09-06.
//

#include "frame.h"
#include <imgui.h>
#include <thread>
#include "../ws/config_ws.h"
#include "../process/rs2client.h"
#include "../injection/injector.h"

void drawMainFrame()
{

    ImGui::Text("RuneScape 3 Emulator Launcher");

    char address[15] = { '1', '2', '7', '.', '0', '.', '0', '.', '1' };
    ImGui::InputText("RSPS IP Address", address, 15);
    if(ImGui::CollapsingHeader("RSA Keys"))
    {
        std::string rsa_key = "dafdafd564fad65f46das";
        std::string js5_rsa_key = "fa5d6s4fa56ds4f56ds4f";
        ImGui::InputText("Login RSA Key", rsa_key.data(), rsa_key.size());
        ImGui::InputText("JS5 RSA Key", js5_rsa_key.data(), js5_rsa_key.size());
    }
    ImGui::Separator();
    if(ImGui::Button("Launch"))
    {
        char path[] = R"(C:\ProgramData\Jagex\launcher\rs2client.exe)";
        RuneScapeInfo info = {};
        info.exePath = path;
        const PROCESS_INFORMATION& process_info = createProcess(info);
        inject(R"(D:\RS3EAgent\cmake-build-relwithdebinfo\RS3EAgent.dll)", process_info);

    }

}

