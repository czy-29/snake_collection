//�����̰����

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <list>

#define WIDTH 20
#define HEIGHT 20

using namespace std;

bool CheckNode (int,int,bool); //����ͷ����β������ײ��� 
void SnakeMove (void); //�ߵ��ƶ� 

LRESULT CALLBACK WndProc (HWND,UINT,WPARAM,LPARAM); //���ڹ��̺��� 

list<RECT> m_snake; list<int> m_key; //���������Լ��������뻺�� 
int direction = VK_LEFT,score = 0; //��ǰ���򼰵÷� 
const int speed = 250; //�ٶ� 
bool is_quickmove = false; //Ŀǰ�Ƿ��ڿ����ƶ�֮�� 
RECT rc_node,food,rc; //����ʳ�Ｐ��Ļ���νṹ 
HBRUSH red,green,blue; //���������õ��Ļ�ˢ  

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int nCmdShow)

{
	RECT window; // 
	DWORD style = WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE; //�б��������б߿��йرհ�ť�ķ�� 
    HWND hwnd; //�����ھ�� 
    MSG messages; //
    WNDCLASSEX wincl; //

    wincl.hInstance = hInstance;
    wincl.lpszClassName = "CZY_Jerry";
    wincl.lpfnWndProc = WndProc;
    wincl.style = 0;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    
    if (!RegisterClassEx (&wincl))
        return 0;

	window.left = 0;
	window.right = 500;
	window.top = 0;
	window.bottom = 500;

	AdjustWindowRect (&window,style,false);
	
	srand ((unsigned) time (NULL)); //������������� 
	
	/*��������*/
    hwnd = CreateWindow (
           "CZY_Jerry",
           "�����̰����",
           style,
           0,
           0,
           window.right - window.left,
           window.bottom - window.top,
           NULL,
           NULL,
           hInstance,
           NULL
           );

	if (!hwnd)
	{
		MessageBox (hwnd,"�޷��������ڣ����������","ERROR",MB_OK | MB_ICONERROR);
		return 0;
	}
	
	SetWindowPos (hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); //�������ö� 
    ShowWindow (hwnd, nCmdShow); //��ʾ���� 
	UpdateWindow (hwnd); //���´��� 

    while (1)
    {
          if (GetMessage (&messages, NULL, 0, 0))
          {
        TranslateMessage(&messages); //������Ϣ 
        DispatchMessage(&messages); //���ô��ڹ��̺��� 
        }
        else break;
    }

    return messages.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc,mdc; //ʵ��DC������DC 
	char str[30] = ""; //
	bool flag = true;
	list<int>::iterator i_key; //�������뻺��ĵ����� 
	list<RECT>::iterator i_snake,i_temp; //����ĵ����� 
	HBITMAP hbmp; //
	PAINTSTRUCT ps; //

	switch (message)
	{
	case WM_CREATE: 
		m_snake.clear();//����������� 
		m_key.clear();//��ռ������뻺�� 
		
		/*��ʼ������ʳ����νṹ*/
		rc_node.top = 300;
		rc_node.bottom = rc_node.top + HEIGHT;

		for (int i=5;i>=0;i--)
		{
			rc_node.left = 300 + i * WIDTH;
			rc_node.right = rc_node.left + WIDTH;
			m_snake.push_front (rc_node);
		}

		m_key.push_back (VK_RIGHT);
		m_key.push_back (0);

		do
		{
			food.left = rand() % ((500 - WIDTH) / WIDTH) * WIDTH;
			food.top = rand() % ((500 - HEIGHT) / HEIGHT) * HEIGHT;
		}   while (CheckNode (food.left,food.top,true));

		food.right = food.left + WIDTH;
		food.bottom = food.top + HEIGHT;

        /*������ˢ*/
		red = CreateSolidBrush (RGB (255,0,0));
		green = CreateSolidBrush (RGB (0,255,0));
		blue = CreateSolidBrush (RGB (0,0,255));
		
		/*������ʱ��*/
		SetTimer (hwnd,1,speed,NULL);
		break;

	case WM_PAINT: //����������ʳ�� 
		hdc = BeginPaint (hwnd,&ps);
		mdc = CreateCompatibleDC (hdc);
		GetClientRect (hwnd,&rc);
		hbmp = CreateCompatibleBitmap (hdc,rc.right - rc.left,rc.bottom - rc.top);
		SelectObject (mdc,hbmp);
		FillRect (mdc,&rc,(HBRUSH) GetStockObject (WHITE_BRUSH));

		FillRect (mdc,&food,blue);

		i_snake = m_snake.begin();
		FillRect (mdc,&(*i_snake),red);
		i_snake++;

		for (;i_snake != m_snake.end();i_snake++)
			FillRect (mdc,&(*i_snake),green);

		BitBlt (hdc,0,0,rc.right - rc.top,rc.bottom - rc.top,mdc,0,0,SRCCOPY);

		EndPaint (hwnd,&ps);
		DeleteDC (mdc);
		DeleteObject (hbmp);
		break;

	case WM_TIMER:
		hdc = GetDC (hwnd);
		SnakeMove(); //�ƶ� 
        
		if (m_snake.begin() -> left == food.left && m_snake.begin() -> top == food.top)
		{
			do
			{
				food.left = rand() % ((500 - WIDTH) / WIDTH) * WIDTH;
				food.top = rand() % ((500 - HEIGHT) / HEIGHT) * HEIGHT;
			}   while (CheckNode (food.left,food.top,true));

			food.right = food.left + WIDTH;
			food.bottom = food.top + HEIGHT;
			score++;
		}
		else
        {
            i_temp = m_snake.end();
            i_temp--;
            m_snake.erase (i_temp);
            }

		m_key.push_back (0);
		
		/*����ײ�Լ����*/
		if (CheckNode (m_snake.begin() -> left,m_snake.begin() -> top,false))
		{
			if (is_quickmove) KillTimer (hwnd,2);
			else KillTimer (hwnd,1);

			sprintf (str,"��Ϸ������\n���ĵ÷֣�%d",score);
			MessageBox (hwnd,str,"��ʾ",MB_OK | MB_ICONASTERISK | MB_TOPMOST);

			DestroyWindow (hwnd);
			return 0;
		}
		else InvalidateRect (hwnd,NULL,false); //ǿ��ˢ�¿ͻ��� 

		ReleaseDC (hwnd,hdc); //�ͷ�DC 
		break;

	case WM_KEYDOWN:
         if ((int) wParam == VK_ESCAPE)
         {
                   DestroyWindow (hwnd);
                   return 0;
         }
		i_key = m_key.begin();
        
        /*�����ƶ�*/ 
		if (direction == (int) wParam)
		{
			if (! is_quickmove)
			{
				is_quickmove = true;
				KillTimer (hwnd,1);
				SetTimer (hwnd,2,speed/4,NULL);
			}
		}
		/*���������ƶ�*/
		else if (is_quickmove)
		{
			is_quickmove = false;
			KillTimer (hwnd,2);
			SetTimer (hwnd,1,speed,NULL);
		}
		
		//�ж������ƶ�һ��֮�ڣ����̰����������Ƿ����෴Ԫ�س��� 
		for (;i_key != m_key.end();i_key++)
		{
			if ((int) wParam != (*i_key)) flag = true;
			else
			{
				flag = false;
				break;
			}
		}

		if (flag) //û�� 
		{
			direction = (int) wParam;
			//m_key.erase (m_key.end());

			switch (direction)
			{
			case VK_UP:
				m_key.push_back (VK_DOWN);
				break;
			case VK_DOWN:
				m_key.push_back (VK_UP);
				break;
			case VK_LEFT:
				m_key.push_back (VK_RIGHT);
				break;
			case VK_RIGHT:
				m_key.push_back (VK_LEFT);
			}

			m_key.push_back (0);
		}
		break;

	case WM_KEYUP:
         /*���������ƶ�*/
		if (is_quickmove)
		{
			is_quickmove = false;
			KillTimer (hwnd,2);
			SetTimer (hwnd,1,speed,NULL);
		}
		break;

	case WM_CLOSE:
		DestroyWindow (hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage (0);
		return 0;
		
	default:
		return DefWindowProc (hwnd,message,wParam,lParam);
	}

	return 0;
}

bool CheckNode (int x,int y,bool flag) //��ײ��� 
{
	list<RECT>::iterator i_s = m_snake.begin();
	if (! flag)
	i_s++++++;

	for (;i_s != m_snake.end();i_s++)
		if (i_s -> left == x && i_s -> top == y) return true;

	return false;
}

void SnakeMove () //�����ƶ� 
{
	m_key.clear();

	switch (direction)
	{
	case VK_UP:
		if (rc_node.top <= 0)
		{
			rc_node.top = 500 - HEIGHT;
			rc_node.bottom = 500;
		}
		else
		{
			rc_node.top -= HEIGHT;
			rc_node.bottom -= HEIGHT;
		}

		m_key.push_back (VK_DOWN);
		break;

	case VK_DOWN:
		if (rc_node.bottom >= 500)
		{
			rc_node.top = 0;
			rc_node.bottom = HEIGHT;
		}
		else
		{
			rc_node.top += HEIGHT;
			rc_node.bottom += HEIGHT;
		}

		m_key.push_back (VK_UP);
		break;

	case VK_LEFT:
		if (rc_node.left <= 0)
		{
			rc_node.left = 500 - WIDTH;
			rc_node.right = 500;
		}
		else
		{
			rc_node.left -= WIDTH;
			rc_node.right -= WIDTH;
		}

		m_key.push_back (VK_RIGHT);
		break;

	case VK_RIGHT:
		if (rc_node.right >= 500)
		{
			rc_node.left = 0;
			rc_node.right = WIDTH;
		}
		else
		{
			rc_node.left += WIDTH;
			rc_node.right += WIDTH;
		}

		m_key.push_back (VK_LEFT);
	}

	m_snake.push_front (rc_node);
}
