#include <iostream>

#include "../header Files/globals.hh"
#include "../header Files/menu.hh"

#include "../header Files/font/tahoma.h"
#include "../header Files/font/icons.h"

#include "../header Files/imgui/imgui.h"

#include "../header Files/functions/configManger.h"
#include "../header Files/functions/utils.h"

#include "../header Files/modules/teleport.h"
#include "../header Files/functions/colored_cout.h"
#include "../header Files/modules/esp.h"

using namespace ImGui;

char xCord_cstr[32];
char yCord_cstr[32];
char zCord_cstr[32];

int selectedOption = 0;
const char* options[] = { "Config 1", "Config 2", "Config 3", "Config 4", "Config 5" };

ImVec4 pickedColorWindowBg = ImVec4(30 / 255.0f, 30 / 255.0f, 36 / 255.0f, 255 / 255.0f);
float cornerRadius = 3.0f;


static void HelpMarker(const char* desc);
void CenterText(const char* text);

void ui::renderMenu() {
    if (!globals.active) return;

    ImGui::SetNextWindowPos(ImVec2(window_pos.x, window_pos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y));
    ImGui::SetNextWindowBgAlpha(1.0f);
    

    ImGui::Begin(window_title, &globals.active, window_flags);
    {
        BeginChild("##buttons", ImVec2(100, 280));
        {

            float spacing = 10.0f;
            // Calculate the total height occupied by the buttons
            float totalButtonHeight = 80 * 3 + spacing * 2; // Assuming 4 buttons with 20-pixel spacing

            // Calculate the vertical offset to center the buttons
            float offsetY = (270 - totalButtonHeight) * 0.5f;

            // Set the cursor position to the calculated offset
            SetCursorPosY(offsetY);

            SetCursorPosX(20);
            if (Button(" \n\n " ICON_FA_PLANE " \n ##1", ImVec2(60, 60))) globals.page = 0;
            SetCursorPosY(offsetY + spacing + 60);

            SetCursorPosX(20);
            if (Button(" \n\n " ICON_FA_EYE " \n ##2", ImVec2(60, 60))) globals.page = 1;
            SetCursorPosY(offsetY + (spacing * 2) + 60 + 60);

            SetCursorPosX(20);
            if (Button(" \n\n " ICON_FA_COGS " \n ##3", ImVec2(60, 60))) globals.page = 2;
            SetCursorPosY(offsetY + (spacing * 3) + (60 * 3));

            SetCursorPosX(20);
            if (Button(" \n\n " ICON_FA_MAP_PIN " \n ##3", ImVec2(60, 60))) globals.page = 3;
        }
        EndChild();
        SameLine();
        SetCursorPosX(112); 
        BeginChild("##pages", ImVec2(480, 280));
        {
            if (globals.page == 0)
            {

                Indent(5);
                SetCursorPosY(5);
                
                CenterText(" ");
                CenterText(" Teleport Menu ");
                CenterText(" ");
                Separator();

                InputText("X", xCord_cstr, 20);
                InputText("Y", yCord_cstr, 20);
                InputText("Z", zCord_cstr, 20);

                CenterText(" ");
                NextColumn();
                if (Button("Teleport")) {

                    try {
                        float zCord_cstr_f = std::stof(zCord_cstr);
						float yCord_cstr_f = std::stof(yCord_cstr);
						float xCord_cstr_f = std::stof(xCord_cstr);

                       

                        teleportToLocation(zCord_cstr_f, xCord_cstr_f, yCord_cstr_f);
                    }
                    catch (const std::invalid_argument& e) {
                        ShowDebugInfo("Invalid argument");
                    }
                    catch (const std::out_of_range& e) {
                        ShowDebugInfo("Out of range");
                    }
                }
                
                SameLine();

                if (Button("Get Current Location")) getCurrentLocation();
                Columns(1);

                ImGui::Unindent();
            }
            if (globals.page == 1)
            {
                Indent(5);
                SetCursorPosY(5);
                CenterText(" ");
                CenterText(" ESP ");
                CenterText(" ");

                Separator();

                if (ImGui::Checkbox("ESP", &modules.isEspOn)) {
                    std::cout << clr::blue << "ESP: " << modules.isEspOn << std::endl;
                }
                Separator();
                if (ImGui::Checkbox("Show Enemys", &modules.showEnemys)) {
                    std::cout << clr::blue << "Show Enemys: " << modules.showEnemys << std::endl;
                }
                if (ImGui::Checkbox("Show Friendlys", &modules.showFriendlys)) {
                    std::cout << clr::blue << "Show Friendlys: " << modules.showFriendlys << std::endl;
                }
                Separator();
                if (ImGui::Checkbox("2D Boxs", &modules.showBoxs)) {
                    std::cout << clr::blue << "2D Boxs: " << modules.showBoxs << std::endl;
                }
                if (ImGui::Checkbox("Health", &modules.showHealth)) {
                    std::cout << clr::blue << "Show Health: " << modules.showHealth << std::endl;
                }

                Columns(1);

                ImGui::Unindent();
            }
            if (globals.page == 2)
            { // config
                Indent(5);

                SetCursorPosY(5);
                CenterText(" ");
                CenterText(" Settings ");
                CenterText(" ");
                Separator();

                if (Button("Debug Console")) {
                    toggleConsole();
                }
                SameLine();
                if (Button("Placeholder / Tests")) {
                    modules.isCrosshairOn = !modules.isCrosshairOn;
                }

                Separator();

                CenterText("");
                CenterText(" Config ");
                CenterText(" ");

                ImGui::Combo("Select an option", &selectedOption, options, IM_ARRAYSIZE(options));

                if (Button("Save Config")) {

                    HWND console = GetConsoleWindow();

                    json configData =
                    {
                        {"console", IsWindowVisible(console) ? "true" : "false"},
                        {"backgroundColor", ImVec4ToStr(pickedColorWindowBg) },
                        {"roundedCorners", std::to_string(cornerRadius)}
                    };

					saveConfig(configData, std::to_string(selectedOption + 1));
                }
                SameLine();
                if (Button("Load Config")) {
                    loadConfig(std::to_string(selectedOption + 1));
                }
                ImGui::Unindent();
            }
            if (globals.page == 3)
            { // Colors
                Indent(5);

                SetCursorPosY(5);
                CenterText(" ");
                CenterText(" Color Picker ");
                CenterText(" ");
                Separator();

                ImGui::Text("Window Bg");
                ImGui::SameLine();
                if (ImGui::ColorEdit4("#1", (float*)&pickedColorWindowBg)) {
                    ApplyCustomColors(pickedColorWindowBg, ImGuiCol_WindowBg);
                }
                ImGui::Text("Rounded Corners");
                ImGui::SameLine();
                if (ImGui::SliderFloat("##RoundedCorners", &cornerRadius, 0.0f, 20.0f)) {
                    ApplyRoundedCorners(cornerRadius);
                }
            }
        }

        EndChild();

        BeginChild("##Social", ImVec2(584, 75));
        {
            /*
            * 
            * Did not know what to put here :3
            * Can be used for subpages eg.
            * globals.subPage = 1;
            * 
            float spacing = 10.0f;

            // Calculate the total width occupied by the buttons and spacing
            float totalWidth = 3 * 60 + 2 * spacing;

            // Calculate the horizontal offset to center the buttons
            float offsetX = (584 - totalWidth) * 0.5f;

            // Set the cursor position to the calculated offset
            SetCursorPosX(offsetX);
            SetCursorPosY(7);

            if (Button(" \n\n " ICON_FA_COMMENTS " \n ##1", ImVec2(60, 60))) {}
                // Handle button click for button 1

            SameLine(offsetX + 60 + spacing); // Use SameLine to align the next button next to the previous one
            if (Button(" \n\n " ICON_FA_COMMENTS " \n ##2", ImVec2(60, 60))) {}
                // Handle button click for button 2

            SameLine(offsetX + 2 * (60 + spacing)); // Use SameLine to align the next button next to the previous one
            if (Button(" \n\n " ICON_FA_COMMENTS " \n ##3", ImVec2(60, 60))) {}
                // Handle button click for button 3
                */
        }
        EndChild();
    }

    ImGui::End();
}

void updateCords(float _xCord, float _yCord, float _zCord) {
    xCord = _xCord;
	yCord = _yCord;
	zCord = _zCord;

    std::string xCord_s = std::to_string(xCord);
    std::string yCord_s = std::to_string(yCord);
    std::string zCord_s = std::to_string(zCord);

    strcpy_s(xCord_cstr, xCord_s.c_str());
    strcpy_s(yCord_cstr, yCord_s.c_str());
    strcpy_s(zCord_cstr, zCord_s.c_str());
}

void ui::init(LPDIRECT3DDEVICE9 device) {
    dev = device;

    // style
    ui::RunStyle();


    // start threads
   

    if (window_pos.x == 0) {
        RECT screen_rect{};
        GetWindowRect(GetDesktopWindow(), &screen_rect);
        screen_res = ImVec2(float(screen_rect.right), float(screen_rect.bottom));
        window_pos = (screen_res - window_size) * 0.5f;

        // init images here
    }

	// load config
    try {
        loadConfig("1");
    }
    catch (std::string ex) {
		std::cout << clr::red << "Error while trying to load config '1' : " << ex << std::endl;
    }
    std::cout << clr::green << "Loaded Config. Now Getting Location" << std::endl;

    ImGuiStyle& style = ImGui::GetStyle();

    pickedColorWindowBg = style.Colors[ImGuiCol_WindowBg];
    cornerRadius = style.WindowRounding;

    getCurrentLocation();
}

void ui::RunStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_TitleBgActive] = ImColor(0, 0, 0, 155);
    style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 36, 255); // Dark blue-black background
    style.Colors[ImGuiCol_ChildBg] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_Button] = ImColor(50, 50, 50, 255); // Dark gray button color
    style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0, 255);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(70, 70, 70, 255); // Lighter gray on hover
    style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255); // White checkmark
    style.Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 205);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(35, 35, 35, 205);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(50, 50, 50, 205); // Darker frame on active
    style.Colors[ImGuiCol_SliderGrab] = ImColor(50, 50, 50, 255); // Dark gray slider grab
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(70, 70, 70, 255); // Lighter gray slider grab on active
    style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255); // White text color

    // Set rounded corners
    style.WindowRounding = cornerRadius;
    style.FrameRounding = cornerRadius;
    style.PopupRounding = cornerRadius;
    style.ScrollbarRounding = cornerRadius;
    style.GrabRounding = cornerRadius;
    style.TabRounding = cornerRadius;
    style.ChildRounding = cornerRadius;
    //style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
}

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void CenterText(const char* text)
{
    if (text == nullptr)
        return;

    ImGui::Spacing();
    ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(text).x / 2));
    ImGui::Text(text);
    ImGui::Spacing();
}
void ApplyCustomColors(const ImVec4& color, ImGuiCol_ whatToChange)
{
    ImGuiStyle& style = ImGui::GetStyle();

    if(whatToChange == ImGuiCol_WindowBg) style.Colors[ImGuiCol_WindowBg] = color;
}
void ApplyRoundedCorners(float radius)
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = radius;
    style.FrameRounding = radius;
    style.PopupRounding = radius;
    style.ScrollbarRounding = radius;
    style.GrabRounding = radius;
    style.TabRounding = radius;
    style.ChildRounding = radius;

}