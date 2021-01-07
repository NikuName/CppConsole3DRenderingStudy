#include <iostream>
#include <string>
#include <random>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <Eigen/Dense>
#include "Header.h"

using namespace std;

using Eigen::MatrixXd;
using Eigen::VectorXd;


Render::Render()
{
	consoleWindow = GetConsoleWindow();
	consoleDC = GetDC(consoleWindow);
	system("mode con: lines=45 cols=160");

	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void Render::setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void Render::cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	std::cout.flush();

	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
	SetConsoleCursorPosition(hOut, topLeft);
}

/*
*	���������� ȭ�鿡 ������
*/
void Render::Rendering()
{
	for (int i = 0; i < Singleton::world->objects->size(); i++)
	{
		MeshRenderer* MR = (*(Singleton::world->objects))[i]->GetComponent<MeshRenderer>();
		if(MR != nullptr)
			MR->Randering();
	}
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		tempConsoleLineData[y] = "";
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			const char* ch = Singleton::pixels[(y * SCREEN_WIDTH) + x].isEnable ? "��" : "��";
			tempConsoleLineData[y] += ch;
			Singleton::pixels[(y * SCREEN_WIDTH) + x].isEnable = false;
		}
		setCursorPosition(0, y);
		cout.write((tempConsoleLineData[y]).c_str(), (tempConsoleLineData[y]).size());
	}
	std::cout.flush();
}

void Render::lineBresenham(vector<float2>* lists, int x0, int y0, int x1, int y1) // �극���� �˰���
{
	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;
	if (dy < 0)
	{                                 // ���⸦ ���ó����
		dy = -dy;
		stepy = -1;
	}
	else
		stepy = 1;

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
		stepx = 1;
	dy <<= 1;                                     // dy*2 �� ���� �ǹ�(��Ʈ����)
	dx <<= 1;                                     // dx*2 �� ���� �ǹ�(��Ʈ����)
	lists->push_back({ (float)x0, (float)y0 });                         // ���� �޼ҵ�
	if (dx > dy) {
		int fraction = dy - (dx >> 1);     // dx>>1 �� dx/2�� ���� �ǹ�(��Ʈ����)
		while (x0 != x1) {
			if (fraction >= 0) {
				y0 += stepy;
				fraction -= dx;                 // fraction -= 2*dx �� ���� �ǹ�
			}
			x0 += stepx;
			fraction += dy;                     // fraction -= 2*dy �� ���� �ǹ�
			lists->push_back({ (float)x0, (float)y0 });
		}
	}
	else {
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) {
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			lists->push_back({ (float)x0, (float)y0 });
		}
	}
}

Render::~Render()
{

}