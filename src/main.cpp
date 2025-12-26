#include "notepad.hpp"
#include "window.hpp"
#include "graphics.hpp"

// Data
constexpr ImVec4 clear_color = ImVec4(0.01f, 0.01f, 0.02f, 0.00f);

class App
{
public:
    App(std::span<const char*> args)
    {
        ImGui_ImplWin32_EnableDpiAwareness();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        io.Fonts->AddFontFromFileTTF("SFMono-Bold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic()); //

        if (!args.empty()) {
            notepad.sFileName = args[0];
            notepad.ToolsUI.OpenFile(notepad.sFileName, &notepad.sBufferText);
        }
    }
    ~App()
    {
        ImGui::DestroyContext();
    }

public:
    int Start()
    {
        while (!render_flags) {
            if (int ret = window.PollMessages()) {
                return ret;
            }
            Frame();
        }
        return 0;
    }

    void Frame()
    {
        ImGuiIO& io = ImGui::GetIO();

        // Start the Dear ImGui frame//
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        notepad.styleImGui();
        ImGui::Begin("Hello", NULL, ImGuiWindowFlag);
        //
        ImGui::SetNextWindowContentSize({ 300, 100 });
        if (ImGui::BeginPopup("#SaveFile", ImGuiWindowFlags_NoTitleBar)) {
            if (!notepad.bIsFileSaved) {
                ImGui::SetCursorPosX(20);
                ImGui::Text("Save text to File");
                ImGui::Separator();
                ImGui::SetCursorPosX(20);
                ImGui::InputText("FileName", &notepad.sFileName);
                if (notepad.sFileName.empty()) {
                    ImGui::SetCursorPosX(20);
                    ImGui::Text("FileName  is Empty!");
                }
                if (notepad.sBufferText.empty()) {
                    ImGui::SetCursorPosX(20);
                    ImGui::Text("Data: file empty!");
                }
                if (!notepad.sBufferText.empty() && !notepad.sFileName.empty()) {
                    ImGui::SetCursorPosX(20);
                    if (ImGui::Button("Save", { 100, 30 })) {
                        notepad.ToolsUI.SaveFile(notepad.sFileName, notepad.sBufferText);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            if (notepad.bIsFileSaved) {
                static float fTimer = 0;
                fTimer += 200 * io.DeltaTime;
                if (fTimer >= 40) {
                    ImGui::CloseCurrentPopup();
                    fTimer               = 0;
                    notepad.bIsFileSaved = false;
                }
                notepad.ToolsUI.textCenter(std::format("file: {} Saved!!", notepad.sFileName).c_str(), { 300, 100 });
                ImGui::Text(std::format("file: {} Saved!!", notepad.sFileName).c_str());
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopup("#NewFile", ImGuiWindowFlags_NoTitleBar)) {
            ImGui::SetCursorPosX(20);
            ImGui::Text("Create New File");
            ImGui::Separator();
            ImGui::SetCursorPosX(20);
            ImGui::InputText("FileName", &notepad.sFileName);
            if (notepad.sFileName.empty()) {
                ImGui::SetCursorPosX(20);
                ImGui::Text("FileName  is Empty!");
            }
            ImGui::SetCursorPosX(20);
            if (ImGui::Button("Create", { 100, 30 })) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Open File", { 100, 30 })) {
                notepad.sFileName = notepad.data.OpenFileDialog();
                notepad.ToolsUI.OpenFile(notepad.sFileName, &notepad.sBufferText);
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        //
        notepad.commands();
        notepad.NUI.WellcomeFrame();
        notepad.ScanFilesUI();
        if (i64ItemIdClicked == 1 && i64MenuSelected == 1) {
            ImGui::OpenPopup("FileEdit");
        }

        ImGui::BeginMainMenuBar();

        ImGui::SetCursorPosX(20);
        ImGui::Text("Help - [F1] file: %s", notepad.sFileName.c_str());
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetItemRectMax().x + 20);
        ImGui::Text("size: %lld", notepad.sBufferText.size());

        ImGui::EndMainMenuBar();

        notepad.NUI.fileEdit();

        auto [w, h]        = window.GetSize();
        ImVec2 MenuBarSize = ImGui::GetWindowSize();
        ImGui::SetWindowSize({ float(w), float(h) });
        ImGui::SetWindowPos({ 0, 26 });
        // notepad.ToolsUI.ItemCenterX((fWindowSize.x), fWindowSize); ImGui::SetCursorPosY(-4);
        ImGui::SetCursorPosY(-4);
        ImGui::SetCursorPosX(-1);
        ImGui::PushStyleColor(ImGuiCol_Border, notepad.rgba(0, 0, 0, 0));
        ImGui::InputTextMultiline("text", &notepad.sBufferText, { float(w), float(h - 30) });
        ImGui::PopStyleColor();
        ImGui::TextColored(notepad.rgba({ 100, 110, 246, 255 }), "size: %lld", notepad.sBufferText.size());
        ImGui::End();

        glViewport(0, 0, w, h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        gfx.Present();
    }

private:
    bool               render_flags     = false;
    int64_t            i64MenuSelected  = 0;
    int64_t            i64ItemIdClicked = 0;
    ImDrawListSplitter JEApp;

    Window   window{ 1280, 1024 };
    Graphics gfx{ window.GetHWND() };
    Notepad  notepad;
};

int main(int argc, char** argv)
{
    std::span<const char*> args{
        const_cast<const char**>(argv + 1),
        size_t(argc - 1)
    };
    return App(args).Start();
}
