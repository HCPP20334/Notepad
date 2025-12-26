cmake_minimum_required(VERSION 3.10)

project(Notepad VERSION 1.0 LANGUAGES CXX)

add_executable(Notepad 
    "src/main.cpp"
    "src/imgui_tables.cpp"
    "src/imgui_widgets.cpp"
    "src/imgui_draw.cpp"
    "src/imgui_stdlib.cpp"
    "src/imgui.cpp"
    "src/imgui_impl_win32.cpp"
    "src/ImGuiBackend.cpp"
    "src/imgui_impl_opengl3.cpp"
    "src/window.cpp" 
    "src/graphics.hpp" 
)
target_link_libraries(Notepad 
    PRIVATE 
        opengl32
)
set_target_properties(Notepad PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
add_custom_command(TARGET Notepad POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/SFMono-Bold.ttf"
        $<TARGET_FILE_DIR:Notepad>
)