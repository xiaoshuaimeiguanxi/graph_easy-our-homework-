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

SDL_Window* window = nullptr;

//��ʼ������
void init_assert(bool flag, const char* error_msg)
{
	if (flag) return;

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��������ʧ��", error_msg, window);
}

int main()
{
	//�����һ��ע��Fashion Center
	init_assert(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), u8"SDL��ʼ��ʧ��");
	init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_Image��ʼ��ʧ��");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//����˫����

	window = SDL_CreateWindow(u8"GraphEasy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	init_assert(window, u8"���ڴ���ʧ��");

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	init_assert(gl_context, u8"����opengl������ʧ��");
	SDL_GL_SetSwapInterval(1);//������ֱͬ��

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
			if (ImGui::BeginMenu(u8"�ļ�"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"�༭"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::Begin(u8"���ܴ���");
		if (ImGui::Button(u8"�ҶȻ�"))
		{
			
			image.add_processor(&gray_p);
			image.process_all();
		}
		if (ImGui::Button(u8"ģ������"))
		{

		}
		if (ImGui::Button(u8"��ֵ��"))
		{

		}
		if (ImGui::Button(u8"��Ե����"))
		{

		}
		ImGui::End();

		ImGui::Begin(u8"ͼ�񴰿�");
		ImGui::Image((void*)(intptr_t)image.get_texture_id(), ImVec2(image.get_surface()->w / 3, image.get_surface()->h / 3));
		ImGui::End();

		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}


	//�˳�����
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}