#include<Windows.h>//windows系统头文件
#include <stdio.h>
#include <conio.h>  //用于控制上下左右的键
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include<time.h>
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib")   //tranparent(透明的）//为TransparentBlt（）函数准备的

/****************定义全局变量*********/

#define width 40			   //一行的个数
#define height 25              //一列的个数
#define number 1000          
#define WND_WIDTH 1200         //窗口的宽
#define WND_HEIGHT 780        //窗口的高


int x = 27;      //初始化蛇头x坐标
int y = 15;      //初始化蛇头y坐标

int x1 = 18;       //初始化蛇尾x1坐标
int y1 = 15;       //初始化蛇尾y1坐标
int length = 10;   //蛇一开始的长度

HDC g_hdc, g_memdc, g_bufdc;    //窗口的设备句柄  内存设备句柄
HBITMAP g_bmp;          //空位图

struct snake{//定义数组
	int x;
	int y;
};
typedef struct snake SNA;
int c = 77;
SNA s1;              //用于暂时储存头位置
SNA food;         //食物的位置
//初始化蛇的状态
int c1;     //用于暂时储存头位置
SNA sna1[number];
wchar_t str[100];
int startState = 0;
int dieNum = 0;
int getUpNum=0;

DWORD   g_tPre, g_tNow;    //获取的时间

wchar_t text[8][100];           //存储着输出文字的字符串数组
int g_iTextNum = 0;             //文字的字数

HBITMAP g_snakeBody, g_hGameOver, g_snakeHead, g_white, g_wall, g_food, g_backmap,g_start;

/*****************************************全局变量*****************/

VOID GamePaint(HWND hwnd,SNA *sna);
VOID Message_Insert(wchar_t* str);
void option(SNA *sna);
void Food(SNA *sna);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM Iparam);
//主函数
int WINAPI WinMain(HINSTANCE hlnstance, //当前实例句柄
	HINSTANCE hInstance,    //前实例句柄
	LPSTR IpCmdLine,        //命令行的指针
	int nShowCmd)            //显示的格式同
{
	//WNDCLASS   window;也是窗口的结构体，但是是以前的版本的，
	//窗口的结构体
	WNDCLASSEX window = { 0 };     //window是我们需要的一个窗口
	SNA sna[number];
	
	
	g_snakeBody = (HBITMAP)LoadImage(NULL, L"./res/蛇身.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_snakeHead = (HBITMAP)LoadImage(NULL, L"./res/蛇头.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_white     = (HBITMAP)LoadImage(NULL, L"./res/空白.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_wall      = (HBITMAP)LoadImage(NULL, L"./res/墙.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_food = (HBITMAP)LoadImage(NULL, L"./res/food.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_backmap = (HBITMAP)LoadImage(NULL, L"./res/backmap.bmp", IMAGE_BITMAP, 1200, 750, LR_LOADFROMFILE);
	g_hGameOver = (HBITMAP)LoadImage(NULL, L"./res/gameOver.bmp", IMAGE_BITMAP, 1373, 523, LR_LOADFROMFILE);//游戏结束的图片
	g_start = (HBITMAP)LoadImage(NULL, L"./res/开始游戏.bmp", IMAGE_BITMAP, 1200, 706, LR_LOADFROMFILE);//游戏开始的图片
	//int sizeof (int);//sizeof是一个运算符
	
	srand((unsigned)time(NULL));/*播种子*/
	Food(sna);
	int h;
	int w;
	SNA temp;

	int i = x1, j = y;       //一开始的位置 横坐标18--27，纵坐标15
	char str[height][width] = { ' ' };

	window.cbSize = sizeof(WNDCLASSEX);//窗口在内存中需要的内存大小
	window.cbClsExtra = 0;        //程序的附加内存（基本上没有用处了，直接为0）
	window.cbWndExtra = 0;         //窗口的附加内存 （基本上没有用处了，直接为0）
	window.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//背景的画刷，和背景的颜色
	//GetStockObject0--检锁预定义的备用刷子
	/****************************
	#define WHITE_BRUSH         0//白色
	#define LTGRAY_BRUSH        1
	#define GRAY_BRUSH          2
	#define DKGRAY_BRUSH        3
	#define BLACK_BRUSH         4
	#define NULL_BRUSH          5
	#define HOLLOW_BRUSH        NULL_BRUSH
	*****************************/
	window.hCursor = NULL;      //改变鼠标的样式,样式有截图时的样式。复制时的样式，等等。。。
	window.hIcon = NULL;        //像表，图标
	window.hInstance = hInstance;// HINSTANCE hlnstance, //当前实例句柄,相当于id
	window.style = CS_HREDRAW | CS_VREDRAW;     //窗口的大小，通过水平和竖直方向改变窗口的大小
	window.lpszMenuName = NULL;             //自己去定义
	window.lpszClassName = L"for our dreams";
	window.lpfnWndProc = WndProc;     //窗口的过程函数

	//注册窗口
	RegisterClassEx(&window);

	//创建窗口
	//窗口类，窗口标题，窗口风格，以及窗口的初始化位置及大小（可选的）。
	HWND hwnd = CreateWindow(L"for our dreams",               //窗口类，
		L"经典游戏-贪吃蛇",        //窗口标题
		WS_OVERLAPPEDWINDOW,                     //窗口风格
		CW_USEDEFAULT, CW_USEDEFAULT,             //窗口的初始位置
		WND_WIDTH, WND_HEIGHT,                     //窗口的大小
		NULL, NULL, hInstance, NULL);                //CW_USEDEFAULT默认的

	g_hdc = GetDC(hwnd);
	g_memdc = CreateCompatibleDC(g_hdc); //Create Compatible DC
	g_bufdc = CreateCompatibleDC(g_hdc);
	g_bmp = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	//1，g_bmp --> memdc(空位图-->内存设备） 
	SelectObject(g_memdc, g_bmp);    //空的位图选入memdc

	HFONT hfont;
	hfont = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"微软雅黑");//设置字体的大小和样式  700是使字体变粗，20是字体的大小，
	SelectObject(g_memdc, hfont);
	SetBkMode(g_memdc, 0);//使1000/1000透明化

	//4,显示窗口 更新窗口
	ShowWindow(hwnd, nShowCmd);      //显示一个窗口
	UpdateWindow(hwnd);

	mciSendString(L"open ./res/贪吃蛇背景音乐.mp3 alias music", 0, 0, 0); //向驱动晨发送字符串消息
	mciSendString(L"play music", 0, 0, 0);

	

	for (i = 0; i < length; i++)
	{
		sna[i].y = y;
		sna[i].x = x1 + i;
	}

	//等待用户的指令，消息
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_tNow = GetTickCount();  //2018 1 9 22 26 20 10
			//GetTickCount（）函数用于获取现在的时间（精确到毫秒）例如 2018 1 23 21 03 10
			if (g_tNow - g_tPre >= 80)    //80ms一次  一帧图像 g_frame=0   //第x帧--第x1帧
			{
				GamePaint(hwnd, sna);//在不断的贴图，
			}
		}
	}
			
	
	return 0;
}

VOID Food(SNA *sna)
{
	int i;
	do{
		food.y = rand() % (height-2) + 1;
		food.x = rand() % (width-2) + 1;
		for (i = 0; i<length; i++)
		{
			if (sna[i].y != food.y || sna[i].x != food.x)
			{
				return;
			}
		}
	} while (1);

}


VOID GamePaint(HWND hwnd,SNA *sna)
{
	//if (g_frame == 10)
	//{
		
		int i, j, h, w;
		SNA temp;
		
		//第二个步骤
		SelectObject(g_bufdc, g_backmap);
		//第三个步骤
		BitBlt(g_memdc, 0, 0, 1200, 750, g_bufdc, 0, 0, SRCCOPY);

		

		if((0 < sna[length - 1].x) && (sna[length - 1].x < (width - 1)) && (0 < sna[length - 1].y) && (sna[length - 1].y < (height - 1))&&(startState==1)&&(dieNum<5))
		{
		    Sleep(350);
			s1 = sna[length - 1];

			for (h = 0; h < height; h++)
			{
				for (w = 0; w < width; w++)
				{
					//str[h][w] = ' ';
					//第二个步骤
					SelectObject(g_bufdc, g_white);
					//第三个步骤
					//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
					TransparentBlt(g_memdc, w * 30, h * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				}
			}
			for (i = 0; i < length; i++)
			{
				//str[sna[i].y][sna[i].x] = '*';
				//第二个步骤
				SelectObject(g_bufdc, g_snakeBody);
				//第三个步骤
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, sna[i].x * 30, sna[i].y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}
			for (j = 0; j < height; j++)
			{
				//str[j][0] = '*';
				//第二个步骤
				SelectObject(g_bufdc, g_wall);
				//第三个步骤
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, 0, j * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				//str[j][width - 1] = '*';
				//第二个步骤
				SelectObject(g_bufdc, g_wall);
				//第三个步骤
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, (width - 1) * 30, j * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}

			for (j = 0; j < width; j++)
			{
				//str[0][j] = '*';
				//第二个步骤
				SelectObject(g_bufdc, g_wall);
				//第三个步骤
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, j * 30, 0, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				//str[height - 1][j] = '*';
				//第二个步骤
				SelectObject(g_bufdc, g_wall);
				//第三个步骤
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, j * 30, (height - 1) * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}
			//str[sna[length - 1].y][sna[length - 1].x] = '&';
			//第二个步骤
			SelectObject(g_bufdc, g_snakeHead);
			//第三个步骤
			//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
			TransparentBlt(g_memdc, sna[length - 1].x * 30, sna[length - 1].y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			//str[food.y][food.x] = '&';
			//第二个步骤
			SelectObject(g_bufdc, g_food);
			//第三个步骤
			//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
			TransparentBlt(g_memdc, food.x * 30, food.y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			
			//打印系统消息字符
			SetTextColor(g_memdc, RGB(255, 0, 0));
			for (int i = 0; i < g_iTextNum; i++)
			{
				TextOut(g_memdc, 540, 60 + i * 18, text[i], wcslen(text[i]));
			}

			//第4步，memdc --> hdc
			BitBlt(g_hdc, 0, 0, 1200, 750, g_memdc, 0, 0, SRCCOPY);
			//printf("1*************************************************\n");
			//Sleep(500);

			temp = sna[0];
			for (i = length - 2; i >= 0; i--)
			{
				sna[length - 2 - i] = sna[length - 1 - i];
			}
			if (c == 72)
				sna[length - 1].y--;
			if (c == 80)
				sna[length - 1].y++;
			if (c == 75)
				sna[length - 1].x--;
			if (c == 77)
				sna[length - 1].x++;
			option(sna);
			//printf("%d",sna[i].y);
			for (i = 0; i < length; i++)
			{
				if ((sna[i].x == food.x) && (sna[i].y == food.y))
				{
					length++;
					for (i = 0; i <= length - 2; i++)
					{
						sna[length - 1 - i] = sna[length - 2 - i];
					}
					sna[0] = temp;
					getUpNum++;
					swprintf_s(str, L"系统消息：好厉害鸭，您已经成长了%d一次！！！",getUpNum);
					Message_Insert(str);

					Food(sna);
				}
			}
		}
else if (startState==1&&dieNum>=5)
{
	//贴图   游戏结束的图片
	SelectObject(g_bufdc, g_hGameOver);
	//透明遮罩法
	BitBlt(g_memdc, 45, 45, 686, 523, g_bufdc, 686, 0, SRCAND);
	BitBlt(g_memdc, 45, 45, 686, 523, g_bufdc, 0, 0, SRCPAINT);
	//第4步，memdc --> hdc
	BitBlt(g_hdc, 0, 0, 1200, 750, g_memdc, 0, 0, SRCCOPY);
}

else 
{
	//第二个步骤
	SelectObject(g_bufdc, g_start);
	//第三个步骤
	BitBlt(g_memdc, 0, 0, 1200, 706, g_bufdc, 0, 0, SRCCOPY);
	//第4步，memdc --> hdc
	BitBlt(g_hdc, 0, 0, 1200, 706, g_memdc, 0, 0, SRCCOPY);
}
	//}
if (!((0 < sna[length - 1].x) && (sna[length - 1].x < (width - 1)) && (0 < sna[length - 1].y) && (sna[length - 1].y < (height - 1))))
{
	dieNum++;
	for (i = 0; i < length; i++)
	{
		sna[i].y = y;
		sna[i].x = x1 + i;
	}
	swprintf_s(str, L"系统消息：您已经死亡了%d次！！！您还有%d次机会复活",dieNum,5-dieNum);
	Message_Insert(str);
}
	g_tPre = GetTickCount();  //用于获取现在的时间（精确到毫秒）例如 2019 1 23 21 03 10
}
// 窗口过程函数针对用户的不同指令做出不同的反应

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM Iparam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN: //鼠标左键按下
		if (startState==0)
		{
			int x = LOWORD(Iparam);
			int y = HIWORD(Iparam);

			if (x >= 568 && x <= 822 && y >= 531 && y <= 593)
			{
				startState = 1;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, message, wparam, Iparam);
}
VOID option(SNA *sna)
{
	int i;
	c1 = c;
	memcpy(sna1, sna, sizeof(SNA)*number);
	//  while (!kbhit());        //开始
	/*
	if (_kbhit())
	{
	if (_getch() == 224)//(ch1==224&&ch2==80)表示按了一个向下的方向键！！

	/*因为 对于 上下左右 和别的一些扩展键
	使用getch()会先返回一个224，再使用一次getch()这时返回的才是扫描码
	得出：
	上键：224 72
	下键：224 80
	左键：224 75
	右键：224 77
	*/
	if (::GetAsyncKeyState(VK_UP) & 0x8000)
	{
		c = 72;
		//printf("UP\n"); // 我帮你把上下左右数据调出来 你用的时候define成 UP来使用这个值就可以了
		for (i = length - 2; i >= 0; i--)
		{
			sna[length - 2 - i] = sna[length - 1 - i];
		}
		sna[length - 1].y--;
		// str[sna[length-1].y][sna[length-1].x]='*';
		//显示系统消息
		swprintf_s(str, L"系统消息：您按了up键！！！");
		Message_Insert(str);
	}
	if (::GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		c = 80;
		//printf("DOWN\n");
		for (i = length - 2; i >= 0; i--)
		{
			sna[length - 2 - i] = sna[length - 1 - i];
		}
		sna[length - 1].y++;
		swprintf_s(str, L"系统消息：您按了down键！！！");
		Message_Insert(str);
	}
	if (::GetAsyncKeyState(VK_LEFT) & 0x8000)
		//printf("LEFT\n");
	{
		c = 75;
		for (i = length - 2; i >= 0; i--)
		{
			sna[length - 2 - i] = sna[length - 1 - i];
		}
		sna[length - 1].x--;
		swprintf_s(str, L"系统消息：您按了left键！！！");
		Message_Insert(str);
	}

	if (::GetAsyncKeyState(VK_RIGHT) & 0x8000)
		//printf("RIGHT\n");
	{
		c = 77;
		for (i = length - 2; i >= 0; i--)
		{
			sna[length - 2 - i] = sna[length - 1 - i];
		}
		sna[length - 1].x++;
		swprintf_s(str, L"系统消息：您按了right键！！！");
		Message_Insert(str);
	}

	if ((s1.x == sna[length - 1].x) && (s1.y == sna[length - 1].y))
	{
		memcpy(sna, sna1, sizeof(SNA)*number);
		c = c1;
	}
	return;
}
VOID Message_Insert(wchar_t* str)
{
	if (g_iTextNum < 8)
	{
		swprintf_s(text[g_iTextNum], str);
		g_iTextNum++;
	}
	else
	{
		for (int i = 1; i < g_iTextNum; i++)
		{
			swprintf_s(text[i - 1], text[i]);
		}
		swprintf_s(text[g_iTextNum - 1], str);
	}
}

/*VOID GamePaint(HWND hwnd)
{
//步骤
//1，g_bmp --> memdc(空位图-->内存设备）
SelectObject(g_memdc, g_bmp);    //空的位图选入memdc
//第二步 图片到bufdc
SelectObject(g_bufdc, g_snakeBody);  //图片选入memdc
//第三步   bufdc-->memdc
BitBlt(g_memdc,            //目标设备
0, 0, 1024, 739,       //目标设备的范围
g_bufdc,               //源设备
0, 0,                  //源设备中的起始值
SRCCOPY);              //拷贝
//4,memdc --> hdc
BitBlt(g_hdc, 0, 0, 1024, 739, g_memdc, 0, 0, SRCCOPY);

SelectObject(g_bufdc, g_snakeBody);
TransparentBlt(g_memdc, 200, 350, 30, 30, g_bufdc,  0, 0, 30, 30, RGB(0, 0, 0));
}*/
