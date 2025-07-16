
#include <Windows.h>
#include <gl/GL.h>

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"

float tx = 0.0f; // Global variable for translation x
float ty = 0.0f; // Global variable for translation y
float tSpeed = 0.1;
float r = 1; //red
float g = 1; //green
float b = 1; //blue
LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if(wParam == VK_RIGHT)
			{
			tx += tSpeed; // Move right
		} else if (wParam == VK_LEFT) {
			tx -= tSpeed; // Move left
		} else if (wParam == VK_UP) {
			ty += tSpeed; // Move up
		}
		else if (wParam == VK_DOWN) {
			ty -= tSpeed; // Move down
		}
		else if (wParam == 'R') { // Press the 'R' Key
			r = 1;
			g = 1;
			b = 1;
			//Reset ehite
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------
void p2Demo() {
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // Remove all transformation
	glTranslatef(0.5, -0.5, 0.0); // tx(0.5)right, ty(-0.5) down
	glRotatef(90, 0, 0, 1); // Rotate 90, ANTICLOCKWISE
	glScalef(0.5, 0.5, 0.5);// sx(.5),sy(0.5), half
	//Bottom go first, then only do the top

	glBegin(GL_TRIANGLES);
	glVertex2f(-0.05, 0);
	glVertex2f(0, 0.5);
	glVertex2f(0.05, 0);
	glEnd();

	glLoadIdentity();
	glScalef(0.5, 0.5, 0.5);// sx(.5),sy(0.5), half

	glBegin(GL_QUADS);
	glVertex2f(-0.5, 0);
	glVertex2f(-0.5, 0.5);
	glVertex2f(0.5, 0.5);
	glVertex2f(0.5, 0);
	glEnd();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}

void p2q1() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	if (rand() % 2) {
		r -= 0.00001;
	}
	else {
		r += 0.001;
	}
	g -= 0.001;
	glLoadIdentity(); // Remove all transformation
	glTranslatef(tx, ty, 0);//Translate x and y, tx & ty are global variables
	glBegin(GL_QUADS); //quads
	glColor3f(r, g, b);
		glVertex2f(-0.5, 0);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, 0.5);
		glVertex2f(0.5, 0);
	glEnd();
}

void p2q2() {

}

void p2q3() {

}

void p2q4() {

}

void display()
{
	p2q1();
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------