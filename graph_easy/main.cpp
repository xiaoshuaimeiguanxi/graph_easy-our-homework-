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
#include <Windows.h>
#include <commdlg.h>

SDL_Window* window = nullptr;

//��ʼ������
void init_assert(bool flag, const char* error_msg)
{
	if (flag) return;

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"��������ʧ��", error_msg, window);
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
	ofn.lpstrTitle = "ѡ��ͼƬ";

	if (GetOpenFileNameA(&ofn))
	{
		char* ptr = filename;
		std::string folder = ptr;

		ptr += folder.length() + 1;

		// �ж��Ƿ��Ƕ�ѡ
		if (*ptr == '\0') {
			// ֻѡ����һ���ļ�
			std::cout << "ѡ����ļ�·���ǣ�" << folder << std::endl;
		}
		else {
			// ��ѡ������ÿ���ļ���
			while (*ptr) {
				std::string full_path = folder + "\\" + ptr;
				std::cout << "ѡ����ļ�·���ǣ�" << full_path << std::endl;
				images.push_back(new ImageEntity(full_path.c_str()));
				ptr += strlen(ptr) + 1;
			}
		}
	}
}

int main()
{
	init_assert(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), u8"SDL��ʼ��ʧ��");
	init_assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), "SDL_Image��ʼ��ʧ��");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);//����˫����

	window = SDL_CreateWindow(u8"GraphEasy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);//��������
	init_assert(window, u8"���ڴ���ʧ��");

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);//����opengl������
	init_assert(gl_context, u8"����opengl������ʧ��");
	SDL_GL_SetSwapInterval(1);//������ֱͬ��

	ImageEntity image;//����ͼ��ʵ��
	image.load_from_file(u8"resources/test.png");//�ӱ��ؼ���ͼ��

	GLuint texture = image.get_texture_id();//...

	GrayProcessor gray_p;//�����Ҷȴ�����
	
	SDL_Event event;//������һ�����ڽ��պʹ���ϵͳ�¼��ĺ������ݽṹ

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();//����imgui������
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());//������������
	ImGui::StyleColorsDark();//��ɫ����

	//��ʼ��GUI�ĺ��
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(u8"#version 130");

	bool is_over = false;
	while (!is_over)
	{
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);//���¼�����imgui����
			if (event.type == SDL_QUIT)//�˳�
				is_over = true;
		}

		//�µ�һ֡
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar())//�������˵���
		{
			if (ImGui::BeginMenu(u8"�ļ�"))//�ļ��˵�ѡ��
			{
				if (ImGui::MenuItem(u8"����"))
				{
					open_file_dialog_win();
				}
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
			
			image.add_processor(&gray_p);//��ӻҶȴ�����
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
		ImGui::End();//UI����պ�

		ImGui::Begin(u8"ͼ�񴰿�");
		ImGui::Image((void*)(intptr_t)image.get_texture_id(), ImVec2(image.get_surface()->w / 3, image.get_surface()->h / 3));
		for (ImageEntity* i : images)
		{
			ImGui::Image((void*)(intptr_t)i->get_texture_id(), ImVec2(i->get_surface()->w / 3, i->get_surface()->h / 3));
		}
		ImGui::End();

		//ͼ�񴰿ڵ�End()ֻ������˸ô��ڵĶ��壬����������Ⱦ�����������������ͼ�񴰿����ڵ�����UIԪ�ػ��Ƶ���Ļ�Ĺؼ�����
		ImGui::Render();//����imGUI��������
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);//����opengl�ӿ�Ϊ��������

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//����������ɫ
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//��Ⱦimgui
		SDL_GL_SwapWindow(window);//����ǰ�󻺳���
	}


	//�˳�����
	ImGui_ImplOpenGL3_Shutdown();//�ر�imgui��ˣ���59 60 �ж�Ӧ
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();//����imgui�����ģ���Ӧ51��
	SDL_GL_DeleteContext(gl_context);//����opengl�����ģ�37��
	SDL_DestroyWindow(window);//���ٴ��ڣ�34

	//�˳�SDL_image��SDL
	IMG_Quit();
	SDL_Quit();

	return 0;
}