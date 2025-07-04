#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "image_entity.h"
#include "gray_processor.h"
#include"blur_processor.h"           //模糊处理

#include <iostream>
#include <Windows.h>
#include <commdlg.h>

SDL_Window* window = nullptr;

std::vector<ImageEntity*> images;

//初始化断言
void init_assert(bool flag, const char* error_msg)
{
	if (flag) return;

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"程序启动失败", error_msg, window);
}

void open_file_dialog_win()
{
	char filename[4096] = { 0 };

	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = "Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.lpstrTitle = "选择图片";

	if (GetOpenFileNameA(&ofn))
	{
		char* ptr = filename;
		std::string folder = ptr;

		ptr += folder.length() + 1;

		// 判断是否是多选
		if (*ptr == '\0') {
			// 只选择了一个文件
			std::cout << "选择的文件路径是：" << folder << std::endl;
		}
		else {
			// 多选，解析每个文件名
			while (*ptr) {
				std::string full_path = folder + "\\" + ptr;
				std::cout << "选择的文件路径是：" << full_path << std::endl;
				images.push_back(new ImageEntity(full_path.c_str()));
				ptr += strlen(ptr) + 1;
			}
		}
	}
}

int main()
{
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
	BlurProcessor blur_p;//模糊处理

	SDL_Event event;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(u8"#version 130");

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
				if (ImGui::MenuItem(u8"导入"))
				{
					open_file_dialog_win();
				}
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
			image.add_processor(&blur_p);    //添加模糊处理器
			image.process_all();             //应用所有处理器

		}
		if (ImGui::Button(u8"二值化"))
		{

		}
		if (ImGui::Button(u8"边缘查找"))
		{

		}
		ImGui::End();

		ImGui::Begin(u8"图像窗口");
		ImGui::Image((void*)(intptr_t)image.get_texture_id(), ImVec2(image.get_surface()->w / 3, image.get_surface()->h / 3));
		for (ImageEntity* i : images)
		{
			ImGui::Image((void*)(intptr_t)i->get_texture_id(), ImVec2(i->get_surface()->w / 3, i->get_surface()->h / 3));
		}
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