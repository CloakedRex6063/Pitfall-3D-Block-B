#include "loadScene.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "util/util.h"
#include "framework/gameInstance.h"
#include "managers/scene/sceneManager.h"
#include "managers/timer/timerManager.h"
#include "scene/game/easyScene.h"
#include "scene/game/hardScene.h"
#include "scene/game/mediumScene.h"
#include "scene/mainMenu/mainMenuScene.h"

namespace ImGui
{
    bool Spinner(const char* label, float radius, int thickness, const ImU32& color)
	{
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;
        
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 pos = window->DC.CursorPos;
        const ImVec2 size((radius )*2, (radius + style.FramePadding.y)*2);
        
        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;
        
        // Render
        window->DrawList->PathClear();

        constexpr int num_segments = 30;
        const int start = static_cast<int>(abs(ImSin(static_cast<float>(g.Time) * 1.8f)*(num_segments-5)));
        
        const float a_min = IM_PI*2.0f * static_cast<float>(start) / static_cast<float>(num_segments);
        constexpr float a_max = IM_PI*2.0f * (static_cast<float>(num_segments)-3) / static_cast<float>(num_segments);

        const ImVec2 centre = ImVec2(pos.x+radius, pos.y+radius+style.FramePadding.y);
        
        for (int i = 0; i < num_segments; i++) {
            const float a = a_min + (static_cast<float>(i) / static_cast<float>(num_segments)) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + static_cast<float>(g.Time) * 8) * radius,
                                                centre.y + ImSin(a + static_cast<float>(g.Time)*8) * radius));
        }

        window->DrawList->PathStroke(color, false, static_cast<float>(thickness));
        return true;
    }
}


void LoadScene::Init()
{
	Scene::Init();
    Util::GetGameInstance()->GetTimerManager()->CreateTimer("Load",2.f,false,[this] {bIsLoaded = true; });
}

int spinnerFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

void LoadScene::Render()
{
	Scene::Render();
	if (!bIsLoaded)
	{
        ImGui::SetNextWindowPos(ImVec2(1150,600));
        ImGui::Begin("Window",nullptr, spinnerFlags);
        ImGui::Spinner("Loading...", 50, 10, IM_COL32(255, 255, 255, 255));
        ImGui::End();
	}
    else
    {
        switch (type)
        {
        case Easy:
        	Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<EasyScene>());
            break;
        case Medium:
        	Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<MediumScene>());
            break;
        case Hard:
        	Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<HardScene>());
            break;
        case MainMenu:
			Util::GetGameInstance()->GetSceneManager()->SetCurrentScene(std::make_unique<MainMenuScene>());
			break;
        }
    }
}
