#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "image_entity.h"
#include "gray_processor.h"

#include <iostream>
#include <algorithm> // 包含 algorithm 头文件

SDL_Window* window = nullptr;
#include <string>
using namespace std;

//zzy

//初始化断言
void init_assert(bool flag, const char* error_msg)
{
    if (flag) return;

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"程序启动失败", error_msg, window);
}

int main()
{
    //坐落在那地极的Fashion Center
    init_assert(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), u8"SDL初始化失败");
    init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_Image初始化失败");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//开启双缓冲

    window = SDL_CreateWindow(u8"GraphEasy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    init_assert(window, u8"窗口创建失败");

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    init_assert(gl_context, u8"创建opengl上下文失败");
    SDL_GL_SetSwapInterval(1);//开启垂直同步

    ImageEntity image;
    image.load_from_file(u8"resources/test.png");

    GLuint texture = image.get_texture_id();

    GrayProcessor gray_p;

    SDL_Event event;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(u8"#version 130");

    // 添加图像拖动相关变量
    bool is_dragging = false;
    ImVec2 drag_start_pos;          // 鼠标拖动起始位置
    ImVec2 drag_start_image_pos;    // 拖动开始时图像的偏移位置
    ImVec2 image_pos(0.0f, 0.0f);  // 当前图像的偏移量
    float zoom = 1.0f;              // 缩放比例

    bool is_over = false;
    while (!is_over)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                is_over = true;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu(u8"文件"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu(u8"编辑"))
            {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin(u8"功能窗口");
        if (ImGui::Button(u8"灰度化"))
        {
            image.add_processor(&gray_p);
            image.process_all();
        }
        if (ImGui::Button(u8"模糊处理"))
        {

        }
        if (ImGui::Button(u8"二值化"))
        {

        }
        if (ImGui::Button(u8"边缘查找"))
        {

        }
        // 添加缩放控制
        ImGui::SliderFloat(u8"缩放", &zoom, 0.1f, 3.0f);   //滑块控件进行缩放
        ImGui::End();

        ImGui::Begin(u8"图像窗口", nullptr, ImGuiWindowFlags_NoMove);

        // 获取窗口信息用于鼠标交互
        ImVec2 window_pos = ImGui::GetCursorScreenPos();
        ImVec2 window_size = ImGui::GetContentRegionAvail();

        // 计算图像显示尺寸（应用缩放）
        ImVec2 image_size(
            image.get_surface()->w / 3.0f * zoom,
            image.get_surface()->h / 3.0f * zoom
        );

        // 计算图像初始居中位置
        ImVec2 center_pos(
            (window_size.x - image_size.x) * 0.5f,
            (window_size.y - image_size.y) * 0.5f
        );

        // 处理鼠标交互
        if (ImGui::IsWindowHovered())
        {
            // 鼠标滚轮缩放
            float wheel = ImGui::GetIO().MouseWheel;
            if (wheel != 0.0f)
            {
                zoom *= (wheel > 0) ? 1.1f : 0.9f;
                zoom = (zoom < 0.1f) ? 0.1f : (zoom > 3.0f) ? 3.0f : zoom; // 手动 clamp
            }

            // 鼠标左键拖动
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                is_dragging = true;
                drag_start_pos = ImGui::GetMousePos();
                drag_start_image_pos = image_pos; // 记录拖动前的图像位置
            }
        }

        // 鼠标左键释放时停止拖动
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
        {
            is_dragging = false;
        }

        // 拖动时计算鼠标位移，并直接应用到图像位置
        if (is_dragging && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImVec2 current_mouse_pos = ImGui::GetMousePos();
            ImVec2 mouse_delta = ImVec2(
                current_mouse_pos.x - drag_start_pos.x,
                current_mouse_pos.y - drag_start_pos.y
            );

            // 1:1 移动图像（直接加上鼠标位移）
            image_pos.x = drag_start_image_pos.x + mouse_delta.x;
            image_pos.y = drag_start_image_pos.y + mouse_delta.y;
        }

        // 最终图像位置 = 居中位置 + 拖动偏移
        ImVec2 final_image_pos(
            center_pos.x + image_pos.x,
            center_pos.y + image_pos.y
        );

        // 设置光标位置并显示图像
        ImGui::SetCursorPos(final_image_pos);
        ImGui::Image((void*)(intptr_t)image.get_texture_id(), image_size);

        ImGui::End();

        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    //退出程序
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}