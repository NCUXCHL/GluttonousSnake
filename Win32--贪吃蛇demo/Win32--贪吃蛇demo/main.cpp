#include<Windows.h>//windowsϵͳͷ�ļ�
#include <stdio.h>
#include <conio.h>  //���ڿ����������ҵļ�
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include<time.h>
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib")   //tranparent(͸���ģ�//ΪTransparentBlt��������׼����

/****************����ȫ�ֱ���*********/

#define width 40			   //һ�еĸ���
#define height 25              //һ�еĸ���
#define number 1000          
#define WND_WIDTH 1200         //���ڵĿ�
#define WND_HEIGHT 780        //���ڵĸ�


int x = 27;      //��ʼ����ͷx����
int y = 15;      //��ʼ����ͷy����

int x1 = 18;       //��ʼ����βx1����
int y1 = 15;       //��ʼ����βy1����
int length = 10;   //��һ��ʼ�ĳ���

HDC g_hdc, g_memdc, g_bufdc;    //���ڵ��豸���  �ڴ��豸���
HBITMAP g_bmp;          //��λͼ

struct snake{//��������
	int x;
	int y;
};
typedef struct snake SNA;
int c = 77;
SNA s1;              //������ʱ����ͷλ��
SNA food;         //ʳ���λ��
//��ʼ���ߵ�״̬
int c1;     //������ʱ����ͷλ��
SNA sna1[number];
wchar_t str[100];
int startState = 0;
int dieNum = 0;
int getUpNum=0;

DWORD   g_tPre, g_tNow;    //��ȡ��ʱ��

wchar_t text[8][100];           //�洢��������ֵ��ַ�������
int g_iTextNum = 0;             //���ֵ�����

HBITMAP g_snakeBody, g_hGameOver, g_snakeHead, g_white, g_wall, g_food, g_backmap,g_start;

/*****************************************ȫ�ֱ���*****************/

VOID GamePaint(HWND hwnd,SNA *sna);
VOID Message_Insert(wchar_t* str);
void option(SNA *sna);
void Food(SNA *sna);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM Iparam);
//������
int WINAPI WinMain(HINSTANCE hlnstance, //��ǰʵ�����
	HINSTANCE hInstance,    //ǰʵ�����
	LPSTR IpCmdLine,        //�����е�ָ��
	int nShowCmd)            //��ʾ�ĸ�ʽͬ
{
	//WNDCLASS   window;Ҳ�Ǵ��ڵĽṹ�壬��������ǰ�İ汾�ģ�
	//���ڵĽṹ��
	WNDCLASSEX window = { 0 };     //window��������Ҫ��һ������
	SNA sna[number];
	
	
	g_snakeBody = (HBITMAP)LoadImage(NULL, L"./res/����.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_snakeHead = (HBITMAP)LoadImage(NULL, L"./res/��ͷ.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_white     = (HBITMAP)LoadImage(NULL, L"./res/�հ�.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_wall      = (HBITMAP)LoadImage(NULL, L"./res/ǽ.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_food = (HBITMAP)LoadImage(NULL, L"./res/food.bmp", IMAGE_BITMAP, 30, 30, LR_LOADFROMFILE);
	g_backmap = (HBITMAP)LoadImage(NULL, L"./res/backmap.bmp", IMAGE_BITMAP, 1200, 750, LR_LOADFROMFILE);
	g_hGameOver = (HBITMAP)LoadImage(NULL, L"./res/gameOver.bmp", IMAGE_BITMAP, 1373, 523, LR_LOADFROMFILE);//��Ϸ������ͼƬ
	g_start = (HBITMAP)LoadImage(NULL, L"./res/��ʼ��Ϸ.bmp", IMAGE_BITMAP, 1200, 706, LR_LOADFROMFILE);//��Ϸ��ʼ��ͼƬ
	//int sizeof (int);//sizeof��һ�������
	
	srand((unsigned)time(NULL));/*������*/
	Food(sna);
	int h;
	int w;
	SNA temp;

	int i = x1, j = y;       //һ��ʼ��λ�� ������18--27��������15
	char str[height][width] = { ' ' };

	window.cbSize = sizeof(WNDCLASSEX);//�������ڴ�����Ҫ���ڴ��С
	window.cbClsExtra = 0;        //����ĸ����ڴ棨������û���ô��ˣ�ֱ��Ϊ0��
	window.cbWndExtra = 0;         //���ڵĸ����ڴ� ��������û���ô��ˣ�ֱ��Ϊ0��
	window.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//�����Ļ�ˢ���ͱ�������ɫ
	//GetStockObject0--����Ԥ����ı���ˢ��
	/****************************
	#define WHITE_BRUSH         0//��ɫ
	#define LTGRAY_BRUSH        1
	#define GRAY_BRUSH          2
	#define DKGRAY_BRUSH        3
	#define BLACK_BRUSH         4
	#define NULL_BRUSH          5
	#define HOLLOW_BRUSH        NULL_BRUSH
	*****************************/
	window.hCursor = NULL;      //�ı�������ʽ,��ʽ�н�ͼʱ����ʽ������ʱ����ʽ���ȵȡ�����
	window.hIcon = NULL;        //���ͼ��
	window.hInstance = hInstance;// HINSTANCE hlnstance, //��ǰʵ�����,�൱��id
	window.style = CS_HREDRAW | CS_VREDRAW;     //���ڵĴ�С��ͨ��ˮƽ����ֱ����ı䴰�ڵĴ�С
	window.lpszMenuName = NULL;             //�Լ�ȥ����
	window.lpszClassName = L"for our dreams";
	window.lpfnWndProc = WndProc;     //���ڵĹ��̺���

	//ע�ᴰ��
	RegisterClassEx(&window);

	//��������
	//�����࣬���ڱ��⣬���ڷ���Լ����ڵĳ�ʼ��λ�ü���С����ѡ�ģ���
	HWND hwnd = CreateWindow(L"for our dreams",               //�����࣬
		L"������Ϸ-̰����",        //���ڱ���
		WS_OVERLAPPEDWINDOW,                     //���ڷ��
		CW_USEDEFAULT, CW_USEDEFAULT,             //���ڵĳ�ʼλ��
		WND_WIDTH, WND_HEIGHT,                     //���ڵĴ�С
		NULL, NULL, hInstance, NULL);                //CW_USEDEFAULTĬ�ϵ�

	g_hdc = GetDC(hwnd);
	g_memdc = CreateCompatibleDC(g_hdc); //Create Compatible DC
	g_bufdc = CreateCompatibleDC(g_hdc);
	g_bmp = CreateCompatibleBitmap(g_hdc, WND_WIDTH, WND_HEIGHT);

	//1��g_bmp --> memdc(��λͼ-->�ڴ��豸�� 
	SelectObject(g_memdc, g_bmp);    //�յ�λͼѡ��memdc

	HFONT hfont;
	hfont = CreateFont(20, 0, 0, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, L"΢���ź�");//��������Ĵ�С����ʽ  700��ʹ�����֣�20������Ĵ�С��
	SelectObject(g_memdc, hfont);
	SetBkMode(g_memdc, 0);//ʹ1000/1000͸����

	//4,��ʾ���� ���´���
	ShowWindow(hwnd, nShowCmd);      //��ʾһ������
	UpdateWindow(hwnd);

	mciSendString(L"open ./res/̰���߱�������.mp3 alias music", 0, 0, 0); //�������������ַ�����Ϣ
	mciSendString(L"play music", 0, 0, 0);

	

	for (i = 0; i < length; i++)
	{
		sna[i].y = y;
		sna[i].x = x1 + i;
	}

	//�ȴ��û���ָ���Ϣ
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
			//GetTickCount�����������ڻ�ȡ���ڵ�ʱ�䣨��ȷ�����룩���� 2018 1 23 21 03 10
			if (g_tNow - g_tPre >= 80)    //80msһ��  һ֡ͼ�� g_frame=0   //��x֡--��x1֡
			{
				GamePaint(hwnd, sna);//�ڲ��ϵ���ͼ��
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
		
		//�ڶ�������
		SelectObject(g_bufdc, g_backmap);
		//����������
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
					//�ڶ�������
					SelectObject(g_bufdc, g_white);
					//����������
					//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
					TransparentBlt(g_memdc, w * 30, h * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				}
			}
			for (i = 0; i < length; i++)
			{
				//str[sna[i].y][sna[i].x] = '*';
				//�ڶ�������
				SelectObject(g_bufdc, g_snakeBody);
				//����������
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, sna[i].x * 30, sna[i].y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}
			for (j = 0; j < height; j++)
			{
				//str[j][0] = '*';
				//�ڶ�������
				SelectObject(g_bufdc, g_wall);
				//����������
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, 0, j * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				//str[j][width - 1] = '*';
				//�ڶ�������
				SelectObject(g_bufdc, g_wall);
				//����������
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, (width - 1) * 30, j * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}

			for (j = 0; j < width; j++)
			{
				//str[0][j] = '*';
				//�ڶ�������
				SelectObject(g_bufdc, g_wall);
				//����������
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, j * 30, 0, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
				//str[height - 1][j] = '*';
				//�ڶ�������
				SelectObject(g_bufdc, g_wall);
				//����������
				//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
				TransparentBlt(g_memdc, j * 30, (height - 1) * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			}
			//str[sna[length - 1].y][sna[length - 1].x] = '&';
			//�ڶ�������
			SelectObject(g_bufdc, g_snakeHead);
			//����������
			//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
			TransparentBlt(g_memdc, sna[length - 1].x * 30, sna[length - 1].y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			//str[food.y][food.x] = '&';
			//�ڶ�������
			SelectObject(g_bufdc, g_food);
			//����������
			//BitBlt(g_memdc, 200, 350, 90, 99, g_bufdc, 0, 0, SRCCOPY);
			TransparentBlt(g_memdc, food.x * 30, food.y * 30, 30, 30, g_bufdc, 0, 0, 30, 30, RGB(0, 0, 0));
			
			//��ӡϵͳ��Ϣ�ַ�
			SetTextColor(g_memdc, RGB(255, 0, 0));
			for (int i = 0; i < g_iTextNum; i++)
			{
				TextOut(g_memdc, 540, 60 + i * 18, text[i], wcslen(text[i]));
			}

			//��4����memdc --> hdc
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
					swprintf_s(str, L"ϵͳ��Ϣ��������Ѽ�����Ѿ��ɳ���%dһ�Σ�����",getUpNum);
					Message_Insert(str);

					Food(sna);
				}
			}
		}
else if (startState==1&&dieNum>=5)
{
	//��ͼ   ��Ϸ������ͼƬ
	SelectObject(g_bufdc, g_hGameOver);
	//͸�����ַ�
	BitBlt(g_memdc, 45, 45, 686, 523, g_bufdc, 686, 0, SRCAND);
	BitBlt(g_memdc, 45, 45, 686, 523, g_bufdc, 0, 0, SRCPAINT);
	//��4����memdc --> hdc
	BitBlt(g_hdc, 0, 0, 1200, 750, g_memdc, 0, 0, SRCCOPY);
}

else 
{
	//�ڶ�������
	SelectObject(g_bufdc, g_start);
	//����������
	BitBlt(g_memdc, 0, 0, 1200, 706, g_bufdc, 0, 0, SRCCOPY);
	//��4����memdc --> hdc
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
	swprintf_s(str, L"ϵͳ��Ϣ�����Ѿ�������%d�Σ�����������%d�λ��Ḵ��",dieNum,5-dieNum);
	Message_Insert(str);
}
	g_tPre = GetTickCount();  //���ڻ�ȡ���ڵ�ʱ�䣨��ȷ�����룩���� 2019 1 23 21 03 10
}
// ���ڹ��̺�������û��Ĳ�ָͬ��������ͬ�ķ�Ӧ

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM Iparam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN: //����������
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
	//  while (!kbhit());        //��ʼ
	/*
	if (_kbhit())
	{
	if (_getch() == 224)//(ch1==224&&ch2==80)��ʾ����һ�����µķ��������

	/*��Ϊ ���� �������� �ͱ��һЩ��չ��
	ʹ��getch()���ȷ���һ��224����ʹ��һ��getch()��ʱ���صĲ���ɨ����
	�ó���
	�ϼ���224 72
	�¼���224 80
	�����224 75
	�Ҽ���224 77
	*/
	if (::GetAsyncKeyState(VK_UP) & 0x8000)
	{
		c = 72;
		//printf("UP\n"); // �Ұ���������������ݵ����� ���õ�ʱ��define�� UP��ʹ�����ֵ�Ϳ�����
		for (i = length - 2; i >= 0; i--)
		{
			sna[length - 2 - i] = sna[length - 1 - i];
		}
		sna[length - 1].y--;
		// str[sna[length-1].y][sna[length-1].x]='*';
		//��ʾϵͳ��Ϣ
		swprintf_s(str, L"ϵͳ��Ϣ��������up��������");
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
		swprintf_s(str, L"ϵͳ��Ϣ��������down��������");
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
		swprintf_s(str, L"ϵͳ��Ϣ��������left��������");
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
		swprintf_s(str, L"ϵͳ��Ϣ��������right��������");
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
//����
//1��g_bmp --> memdc(��λͼ-->�ڴ��豸��
SelectObject(g_memdc, g_bmp);    //�յ�λͼѡ��memdc
//�ڶ��� ͼƬ��bufdc
SelectObject(g_bufdc, g_snakeBody);  //ͼƬѡ��memdc
//������   bufdc-->memdc
BitBlt(g_memdc,            //Ŀ���豸
0, 0, 1024, 739,       //Ŀ���豸�ķ�Χ
g_bufdc,               //Դ�豸
0, 0,                  //Դ�豸�е���ʼֵ
SRCCOPY);              //����
//4,memdc --> hdc
BitBlt(g_hdc, 0, 0, 1024, 739, g_memdc, 0, 0, SRCCOPY);

SelectObject(g_bufdc, g_snakeBody);
TransparentBlt(g_memdc, 200, 350, 30, 30, g_bufdc,  0, 0, 30, 30, RGB(0, 0, 0));
}*/
