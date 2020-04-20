/* CRÉDITOS - DISCORD: SCHURTZ#3214
SCHURTZ "LEONARDO AMARO"
DARKRATOS
M47Z
*/
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "C:\ProgramData\WindowsIncludes\http.h"
#include <cassert>
#include "C:\ProgramData\WindowsIncludes\utils.h"
#pragma comment(lib, "requests.lib")

#include <iostream>

using namespace std;

#include "imgui.h"
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif


#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"              // warning : warning: macro is not used                         // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant                  // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"           // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                      // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // [__GNUC__ >= 6] warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))


#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)


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



static int messagebox = 0;
bool endloop = false;
char* tipo;
char* status;



Requests::post_data data_login;
Requests::request* request_login;


void ImGui::ShowLoader(bool* p_open) {

    // IMGUI FLAGS

    ImGuiWindowFlags window_flags = 0;

    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;


    ImGui::SetNextWindowPos(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(495, 320), ImGuiCond_FirstUseEver);

   // FORM
    if (!ImGui::Begin("Loader C++ BY LEONARDO AMARO", p_open, window_flags)) {
        ImGui::End();
        return;
    }
    
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12); 

    if (ImGui::BeginTabBar("Loader Tab"), ImGuiTabBarFlags_AutoSelectNewTabs) {
        static int logado = 0;
        int length;
        const char * msgdesc;

        // SE NÃO ESTIVER LOGADO ABRE A TAB DE LOGIN
        if (logado == 1 & 1)
        {
            //RENDERIZAÇÃO DA TAB LOGIN
            if (ImGui::BeginTabItem("Login"))
            {
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 125);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                    ImGui::Text("Realize o login para continuar");
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
                }

                // TEXTBOX USER
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
                ImGui::Text("Usuario:");
                ImGui::SameLine();
                static char usuariotext[128] = "";
                ImGui::InputText(" ", usuariotext, IM_ARRAYSIZE(usuariotext));

                // TEXTBOX PASS
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
                ImGui::Text("Senha:  ");
                ImGui::SameLine();
                static char senhatext[128] = "";
                ImGui::InputTextWithHint("  ", "", senhatext, IM_ARRAYSIZE(senhatext), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);

                //CHECK BOX SAVE LOGIN
                {
                    static bool check = true;
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 70);
                    ImGui::Checkbox("Salvar Login", &check);
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 135);
                }

                // VARIAVEL MSG BOX E A DESCRIÇÃO DA MSG
                static int message_box = 0;

                if (ImGui::Button("Entrar"))
                {
                    // ENVIANDO REQUEST PARA O SERVIDOR PARA REALIZAÇÃO DO LOGIN
                     data_login = { {"username", usuariotext}, {"password", senhatext} };
                    request_login = Requests::post("https://YOUR-PATH/loader/login.php", data_login);

                    // LOGIN INCORRETO - ABRE MSG 
                    if (!strcmp(request_login->text.c_str(), "incorrect"))
                    {
                        if (logado == NULL)
                            message_box++;
                            msgdesc = "Password or user wrong";
                    }

                    // LOGIN CORRETO ABRE TAB LOGADO
                    else if (!strcmp(request_login->text.c_str(), "true"))
                    {
                        if (logado == NULL)
                            logado++;
                    }
  
                }

                // CASO A FUNÇÃO DE MENSAGEM SEJA CHAMADA EXIBE MENSAGEM
                if (message_box & 1)
                {
                    ImGui::OpenPopup("HardHax Message");

                    if (ImGui::BeginPopupModal("HardHax Message", NULL)) {
                        ImGui::Text(msgdesc);

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 100);
                        if (ImGui::Button("  OK  ")) {
                            ImGui::CloseCurrentPopup();
                            message_box = 0;
                        }
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);

                        ImGui::EndPopup();
                    }
                }
                ImGui::EndTabItem();
            }
        }
        // CASO ESTEJA LOGADO ABRE AS TABS DE CHEAT
        else {
            // TAB DE CHEATS
            if (ImGui::BeginTabItem("Cheats")) {
                // LISTVIEW IMGUI
                ImGui::Text("Selecione o cheat desejado:");
                ImGui::Columns(5, "cheat_columns");
                ImGui::Separator();
                ImGui::Text("ID"); ImGui::NextColumn();
                ImGui::Text("Jogo"); ImGui::NextColumn();
                ImGui::Text("Criador"); ImGui::NextColumn();
                ImGui::Text("Tipo"); ImGui::NextColumn();
                ImGui::Text("Status"); ImGui::NextColumn();
                ImGui::Separator();

                std::vector<char*> tablejogos;
                std::vector<char*> tablecriador;
                std::vector<char*> tabletipo;
                std::vector<char*> tablestatus;

                // QUANTIDADE DE CHEATS - CASO FOR FAZER REMOTO BASTA REALIZAR O METHOD POST E DESENVOLVER O PHP PARA TRATAR O DADO ASSIM COMO O SISTEMA DE LOGIN ACIMA
                length = 4;
                                                                                                                     
                static int selected = -1;

                // ADICIONA OS ATRIBUTOS AO LIST VIEW
                for (int i = 0; i < length; i++) {
                    char label[32];
                    sprintf(label, "%04d", i);
                    if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                        selected = i;

                    bool hovered = ImGui::IsItemHovered();
                    ImGui::NextColumn();
                    ImGui::Text("NOME DO JOGO"); ImGui::NextColumn();
                    ImGui::Text("NOME DO CRIADOR"); ImGui::NextColumn();
                    ImGui::Text("TIPO DE CHEAT"); ImGui::NextColumn();
                    ImGui::Text("STATUS"); ImGui::NextColumn();
                }

                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::TreePop();

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 200);

                if (ImGui::Button("Carregar Cheat")) {
                    // AS SELECTS ABAIXOS DETECTAM O CHEAT SELECIONADO PARA CHAMAR A FUNÇÃO DE INJECT
                    if (selected == 0) {                                          
                        std::cout << "SELECIONADO O CHEAT 1 - REALIZAR FUNCAO DE INJECT" << std::endl;      
                    }
                    else if (selected == 1) {
                        std::cout << "SELECIONADO O CHEAT 2 - REALIZAR FUNCAO DE INJECT" << std::endl;
                    }
                    else if (selected == 2) {
                        std::cout << "SELECIONADO O CHEAT 3 - REALIZAR FUNCAO DE INJECT" << std::endl;
                    }
                    else if (selected == 3) {
                        std::cout << "SELECIONADO O CHEAT 4 - REALIZAR FUNCAO DE INJECT" << std::endl;
                    }
                    else if (selected == 4) {
                        std::cout << "SELECIONADO O CHEAT 5 - REALIZAR FUNCAO DE INJECT" << std::endl;
                    }
                }                          
            }
        }
        ImGui::EndTabBar();
    }

    
    /* THEMA DO IMGUI */

    ImGuiStyle* style = &ImGui::GetStyle();

    style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style->WindowPadding = ImVec2(15, 8);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 0.9f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.463f, 0.034f, 0.963f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
    
    ImGui::End(); 
}

#endif
