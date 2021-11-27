// Standard Includes -----------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include <Application.h>
#include <Bitmap.h>
#include <View.h>
#include <Window.h>
// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Decode.h"
#include "Bitmap.h"
#include "Game.h"
#include "Graphics.h"
#include "PakFile.h"
#include "Storage.h"
#include "VMCode.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------
//HWND hWnd;
VMContext vm;
BWindow *window;

class TestWindow : public BWindow {
public:
	TestWindow();
	virtual void MessageReceived(BMessage *message);
	virtual bool QuitRequested();
	
private:
	BView *fView;
	BBitmap *fBitmap;
};


TestWindow::TestWindow()
	:
	BWindow(BRect(100, 100, 420, 300), "window", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS)
{
	fView = new BView(Bounds(), "view", B_FOLLOW_ALL, B_WILL_DRAW);
	AddChild(fView);
	window = this;
	
	char *folder = "/boot/Kyrandia2/";
	char *room = "INHOME";//"cost1_sh";*/"misc_emc";
	char path[256];

	strcpy(path, folder);
	strcat(path, room);
	strcat(path, ".PAK");
	
	storage->AddArchive(path);
	storage->AddArchive("/boot/Kyrandia2/OTHER.PAK");

	strcpy(path, room);
	strcat(path, ".EMC");
	uint8 *code = storage->LoadResource(path);
	strcpy(path, room);
	strcat(path, ".CPS");
	GBitmap *bitmap = storage->LoadBitmap(path);
	strcpy(path, room);
	strcat(path, ".COL");
	GPalette *palette = storage->LoadPalette(path);

	BWindow *hWnd = window;
	graphics = new Graphics(hWnd);

	if (bitmap)
		graphics->SetBitmap(bitmap);
	if (palette)
		graphics->SetPalette(palette);
	graphics->DrawBackground();

	Font *font = storage->LoadFont("8FAT.FNT");
	if (font)
		graphics->SetFont(font);

	if (!palette)
	{
		storage->AddArchive("/boot/Kyrandia2/INHOME.PAK");
		palette = storage->LoadPalette("INHOME.COL");
		graphics->SetPalette(palette);
	}

	if (code)
	{
		vm.Load(code);

		printf("Setup ---------------------------------------------\n");
		vm.Run(SCENE_SETUP);
		printf("OnDraw --------------------------------------------\n");
		vm.Push(1);
		vm.Run(ONDRAW_EVENT);
	}
	
}


void
TestWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		default:
			graphics->Draw(fView);
			BWindow::MessageReceived(message);
			break;
	}
}


bool
TestWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

class TestApp : public BApplication {
public:
	TestApp();
	virtual void ReadyToRun();
	virtual void Pulse();	
};

TestApp::TestApp()
	:BApplication("application/x-vnd.Test")
{
	SetPulseRate(200000);
}


void
TestApp::ReadyToRun()
{
	(new TestWindow())->Show();
}


void
TestApp::Pulse()
{
	printf("Pulse\n");
	BWindow *window = WindowAt(0);
	BView *view = window->FindView("view");
	window->Lock();
	graphics->Draw(view);
	window->Unlock();
}

int main()
{
	TestApp app;
	app.Run();
	return 0;
}
#if 0
//------------------------------------------------------------------------------
void Draw(HDC hDC, RECT rect)
{
	graphics->Draw(hDC);
}
//------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//case WM_ACTIVATE:
		//	return 0;

		case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		{
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			return 0;
		}
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		{
			printf("Click ---------------------------------------------\n");
			if ((HIWORD(lParam) >> 1) > 140)
			{
				game->Drop();
			}
			else
			{
				vm.SetReg(1, LOWORD(lParam) >> 1);
				vm.SetReg(2, HIWORD(lParam) >> 1);
				vm.SetReg(4, game->Holding());
				vm.Run(ONCLICK_EVENT);
			}
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;
		}
		case WM_MOVE:
			return 0;

		case WM_SIZE:
		{
			return 0;
		}
		
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			BeginPaint(hWnd, &paint);
			Draw(paint.hdc, paint.rcPaint);
			EndPaint(hWnd, &paint);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		case WM_ERASEBKGND:
			return 1;

		case WM_KEYDOWN:
		{
			return 0;
		}
		case WM_CHAR:
		{
			return 0;
		}
		case WM_KEYUP:
		{
			return 0;
		}
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case CMD_FILE_OPEN:
					printf("open\n");
					break;
				case CMD_FILE_EXIT:
					PostQuitMessage(0);
					break;
				default:
				{
					if (wParam >= 4000 && wParam <= 4017)
					{
						printf("Ordr%2d --------------------------------------------\n", wParam - 4000);
						vm.Run(wParam - 4000);
					}
					if (wParam >= 5000 && wParam <= 5999)
					{
						const char *name = storage->ArchiveAt(0)->NameAt(wParam - 5000);
						
						printf("Loading %s\n", name);
						int32 len = strlen(name);

						if (name[len - 3] == 'C' && name[len - 2] == 'O' &&
							name[len - 1] == 'L')
						{
							GPalette *palette = storage->LoadPalette(name);

							graphics->SetPalette(palette);
							delete palette;
						}
						else if (name[len - 3] == 'C' && name[len - 2] == 'P' &&
							name[len - 1] == 'S')
						{
							GBitmap *bitmap = storage->LoadBitmap(name);
							
							if (bitmap->Palette())
								graphics->SetPalette(bitmap->Palette());

							graphics->DrawBitmap(bitmap);

							HDC hDC = GetDC(hWnd);
							graphics->Draw(hDC);
							ReleaseDC(hWnd, hDC);

							delete bitmap;
						}
						else if (name[len - 3] == 'E' && name[len - 2] == 'M' &&
							name[len - 1] == 'C')
						{
							uint8 *code = storage->LoadResource(name);
							
							VMContext context(code);
							context.Run(0);
						}
						else if (name[len - 3] == 'F' && name[len - 2] == 'N' &&
							name[len - 1] == 'T')
						{
							Font *font = storage->LoadFont(name);
							graphics->SetFont(font);
							graphics->DrawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 10, 10, 0);
							graphics->DrawString("abcdefghijklmnopqrstuvwxyz", 10, 20, 0);
							delete font;
						}
						else if (name[len - 3] == 'M' && name[len - 2] == 'S' &&
							name[len - 1] == 'C')
						{
							GBitmap *bitmap = storage->LoadBitmap(name);

							graphics->DrawBitmap(bitmap);
							delete bitmap;
						}
						else if (name[len - 3] == 'V' && name[len - 2] == 'O' &&
							name[len - 1] == 'C')
						{
							Sound *sound = storage->LoadSound(name);

							//delete sound;
						}
						else if ((name[len - 3] == 'W' && name[len - 2] == 'S' &&
							name[len - 1] == 'A') || name[len - 3] == 'S' &&
							name[len - 2] == 'H' && name[len - 1] == 'P')
						{
							Movie *movie = storage->LoadMovie(name);

							if (movie->Palette())
								graphics->SetPalette(movie->Palette());

							HDC hDC = GetDC(hWnd);

							for (int32 i = 0; i < movie->CountFrames(); i++)
							{
								int32 width, height;
								uint8 *bits = movie->GetFrame(i, &width, &height);
								graphics->DrawBackground();
								graphics->DrawBits(bits, width, height, 0, 0);

								graphics->Draw(hDC);
								
								Sleep(33);
							}

							ReleaseDC(hWnd, hDC);

							delete movie;
						}
					}
					break;
				}
			}
			break;

			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
//------------------------------------------------------------------------------
struct BuildViewMenuStruct {
	HMENU menu;
	int32 i;
};
//------------------------------------------------------------------------------
static bool BuildViewMenuCallback(PakFile *file, const char *name,
						   BuildViewMenuStruct *bvm)
{
	AppendMenu(bvm->menu, MF_ENABLED | MF_STRING, bvm->i++, name);
	return true;
}
//------------------------------------------------------------------------------
void BuildViewMenu(HWND hWnd)
{
	HMENU menu = GetMenu(hWnd);
	menu = GetSubMenu(menu, 1);
	DeleteMenu(menu, 0, MF_BYPOSITION);

	BuildViewMenuStruct bvm;

	bvm.menu = menu;
	bvm.i = 5000;

	storage->ArchiveAt(0)->EnumResources((EnumResourcesCallback)&BuildViewMenuCallback, &bvm);
}
//------------------------------------------------------------------------------
void BuildRunMenu(HWND hWnd)
{
	char string[128];
	HMENU menu = GetMenu(hWnd);
	menu = GetSubMenu(menu, 2);
	DeleteMenu(menu, 0, MF_BYPOSITION);

	for (int32 i = 0; i < 8; i++)
	{
		sprintf(string, "function %d", i);
		AppendMenu(menu, MF_ENABLED | MF_STRING, 4000 + i, string);
	}
	for (i = 8; i <= 17; i++)
	{
		sprintf(string, "animation %d", i - 8);
		AppendMenu(menu, MF_ENABLED | MF_STRING, 4000 + i, string);
	}
}
//------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
					 int nCmdShow)
{
	// Enable console
	AllocConsole();
	SetConsoleTitle("VICClient");
	freopen("CONIN$", "rb", stdin);
	freopen("CONOUT$", "wb", stdout);
	freopen("CONOUT$", "wb", stderr);

	// Register window class
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instance;
	wcex.hIcon			= LoadIcon(instance, MAKEINTRESOURCE(IDI_MAIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU);
	wcex.lpszClassName	= "Kyrandia";
	wcex.hIconSm		= NULL;

	RegisterClassEx(&wcex);

	// Create HWND
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
	RECT rect;

	rect.left = 1024 / 2 - 320;
	rect.top = 768 / 2 - 240;
	rect.right = rect.left + 640;
	rect.bottom = rect.top + 400;

	AdjustWindowRectEx(&rect, dwStyle, TRUE, dwExStyle);

	hWnd = CreateWindowEx(dwExStyle, "Kyrandia", "Kyrandia", dwStyle,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, instance, NULL);

	BuildRunMenu(hWnd);

	//
	char *folder = "D:/web/Kyrandia.fr/";
	char *room = "cost1_sh";//"cost1_sh";*/"misc_emc";
	char path[256];

	strcpy(path, folder);
	strcat(path, room);
	strcat(path, ".PAK");
	
	storage->AddArchive(path);
	storage->AddArchive("D:/web/Kyrandia.fr/other.pak");

	BuildViewMenu(hWnd);

	strcpy(path, room);
	strcat(path, ".EMC");
	uint8 *code = storage->LoadResource(path);
	strcpy(path, room);
	strcat(path, ".CPS");
	GBitmap *bitmap = storage->LoadBitmap(path);
	strcpy(path, room);
	strcat(path, ".COL");
	GPalette *palette = storage->LoadPalette(path);

	graphics = new Graphics(hWnd);

	if (bitmap)
		graphics->SetBitmap(bitmap);
	if (palette)
		graphics->SetPalette(palette);
	graphics->DrawBackground();

	Font *font = storage->LoadFont("8FAT.FNT");
	if (font)
		graphics->SetFont(font);

	if (!palette)
	{
		storage->AddArchive("D:/web/Kyrandia.fr/inhome.pak");
		palette = storage->LoadPalette("INHOME.COL");
		graphics->SetPalette(palette);
	}

	if (code)
	{
		vm.Load(code);

		printf("Setup ---------------------------------------------\n");
		vm.Run(SCENE_SETUP);
		printf("OnDraw --------------------------------------------\n");
		vm.Push(1);
		vm.Run(ONDRAW_EVENT);
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	//printf("Ordr3 ---------------------------------------------\n");
	//vm.Run(3);
	//printf("Ordr8 ---------------------------------------------\n");
	//vm.Run(8);
	//printf("Ordr9 ---------------------------------------------\n");
	//vm.Run(9);
	//printf("Ordr10 --------------------------------------------\n");
	//vm.Run(10);

	MSG msg;

	for (;;)
	{
		if (WaitMessage())
		{
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	delete graphics;
	delete storage;
	delete font;

	return 0;
}
//------------------------------------------------------------------------------
#endif
