/* 

   win_main.cpp

   Author: Saroj Rai @ CharichaSoftwares 
   Created On: Wednesday, 16 September 2020.

*/

// %Log:$ //
//

#ifndef UNICODE
#define UNICODE
#endif


#include <windows.h>
// #include <windowsx.h>
#include <strsafe.h>
#include "ad_bs_convert.h"
#include "icon_resources.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wPARAM, LPARAM lParam);

// Main Window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 300


// Setting Up Fonts
HFONT g_hFont;
HFONT g_hTitleFont;

void LoadFonts(HWND);

// Menus
#define FILE_MENU_NEW 12
#define FILE_MENU_EXIT 22
#define HELP_MENU 32
#define ABOUT_MENU 42

HMENU g_hMenu;
void AddMenus(HWND);



// Controls
#define COLUMN 110
#define GAP 21
#define MARGIN 90
#define WIDGET_WIDTH 100
#define WIDGET_HEIGHT 25

#define CONVERT_AD_TO_BS 20
#define CONVERT_BS_TO_AD 21


HWND g_hADYear;
HWND g_hADMonth;
HWND g_hADDay;

HWND g_hBSYear;
HWND g_hBSMonth;
HWND g_hBSDay;

HWND g_hLogoWin;		// Image window 

void AddControls(HWND);

// Static Text
HWND g_hInstructText;

// DateDisplay
HWND g_hADDisplay;
HWND g_hBSDisplay;



// EditControl Procedure
WNDPROC g_wEditProc;
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




void ConvertUpdateADBS();
void ConvertUpdateBSAD();
void UpdateEditMyDate();
void AddValueEditMyDate(HWND hWnd, int value);
int CheckAndUpdateFixRange(HWND hWnd, int min, int max);



// Images
HICON g_hIcon; 			// our application icon
HBITMAP g_hLogoBitmap;
void LoadImages();




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow){

    const wchar_t CLASS_NAME[] = L"Quick ADBS Date Converter";
    g_hIcon = LoadIconW(hInstance,
			MAKEINTRESOURCE(IDI_WINLOGO));
			// L"ADBS.png_icon.ico");

    WNDCLASSEX wc = { };

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MY_ICON));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    // wc.hIconSm = (HICON)LoadImage(hInstance,
    // 			   MAKEINTRESOURCE(5),
    // 			   IMAGE_ICON,
    // 			   GetSystemMetrics(SM_CXSMICON),
    // 			   GetSystemMetrics(SM_CXSMICON),
    // 			   LR_DEFAULTCOLOR);
    

    RegisterClassEx(&wc);


    HWND hWnd = CreateWindowEx(0,	// optional window style
			       CLASS_NAME,
			       L"Quick ADBS Date Converter",
			       (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) ^ WS_MAXIMIZEBOX ,

			       // size and position
			       CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,


			       NULL, // parent window
			       NULL, // menu
			       hInstance, // Instance Handle
			       NULL	// additional application data
			       );

    if(!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);


    // Run the message loop
    MSG msg = { };
    while(GetMessage(&msg, NULL, 0, 0)){
	if(IsDialogMessage(hWnd, &msg) == 0) { 
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
    }

    return 0;

}


// CONVERT_AD_TO_BS
int year_form = 1994;
int* converted = nullptr;
wchar_t year[100], month[100], day[100];

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

    switch(uMsg){
    case WM_CREATE:

	LoadFonts(hWnd);
	LoadImages();
	AddMenus(hWnd);
	AddControls(hWnd);
	UpdateEditMyDate();
	break;
    case WM_COMMAND:
	// UpdateEditMyDate();
	switch(wParam){
	case FILE_MENU_NEW:
	    MessageBeep(MB_OK);
	    break;
	case FILE_MENU_EXIT:
	    DestroyWindow(hWnd);
	    break;
	case HELP_MENU:
	    MessageBox(hWnd, L" Tool That lets you value your time by \n quickly converting AD to BS and\n vice-versa. \n\n- TAB(↹) to Change Field \n- Shift(⇧) + TAB(↹) to Change Field Backward \n- Arrow Key Up(↑) and Down(↓) to increment by one \n- Enter(↵) for Quick Result 'el pronto! Help. \n\n For More Info, Feedback, Bugs, Errors\n Contact Us At\n\n charichainstitute@gmail.com \n Belbari-11, Charicha Chowk ", L"Help", MB_OK); 
	    break;
	case ABOUT_MENU:
	    MessageBox(hWnd, L"\n\n Contact Us At\n\n charichainstitute@gmail.com \n Belbari-11, Charicha Chowk \n +977 981-7388966 \n\n Copyright © Charicha Softwares .", L"About", MB_OK); 
	    break;
	case CONVERT_AD_TO_BS:
	    ConvertUpdateADBS();
	    UpdateEditMyDate();	    
	    break;
	case CONVERT_BS_TO_AD:
	    ConvertUpdateBSAD();
	    UpdateEditMyDate();	    
	    break;
	case IDOK:		// enter on dialog box/edit box
	    {
		HWND c_hWnd = GetFocus();
		if(c_hWnd == g_hADYear  ||
		   c_hWnd == g_hADMonth ||
		   c_hWnd == g_hADDay) {
		    ConvertUpdateADBS();
		} else if(c_hWnd == g_hBSYear ||
			  c_hWnd == g_hBSMonth ||
			  c_hWnd == g_hBSDay){
		    ConvertUpdateBSAD();
		}
		UpdateEditMyDate();
	    }
	    break;
	} // wParam Switch 

	break;

    case WM_DESTROY:
	PostQuitMessage(0);
	break;
    case WM_CTLCOLORSTATIC:
	SetBkMode((HDC) wParam, TRANSPARENT);
	return (LRESULT) CreateSolidBrush(0xFFFFFF);
	break;
    case WM_PAINT:
	{
	    PAINTSTRUCT ps;
	    HDC hdc = BeginPaint(hWnd, &ps);

	    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

	    EndPaint(hWnd, &ps);
	}
	break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void AddMenus(HWND hWnd){
    g_hMenu = CreateMenu();

    HMENU hFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();
    HMENU hAboutMenu = CreateMenu();

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New");
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");

    AppendMenu(g_hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");
    AppendMenu(g_hMenu, MF_STRING, HELP_MENU, L"Help");
    AppendMenu(g_hMenu, MF_STRING, ABOUT_MENU, L"About");

    SetMenu(hWnd, g_hMenu);
}


void AddControls(HWND hWnd){

    int new_row = GAP + GAP/2;		// row to keep track/align items
    HWND hWndStatic; 
    
    hWndStatic= CreateWindowEx(0, L"static", L" AD to BS Converter \n (1944-2033)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row - WIDGET_HEIGHT/2, 
		   WIDGET_WIDTH * 2, WIDGET_HEIGHT + WIDGET_HEIGHT/2,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    g_hADDisplay = CreateWindowEx(0, L"static", L" AD to BS Converter", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE | SS_CENTER,
		   WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2, new_row - WIDGET_HEIGHT/2, 
		   WIDGET_WIDTH * 2, WIDGET_HEIGHT + WIDGET_HEIGHT/2,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(g_hADDisplay, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

  
    // Label text
    new_row = GAP * 3;
    hWndStatic = CreateWindowEx(0, L"static", L"Year(AD)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    hWndStatic = CreateWindowEx(0, L"static", L"Month", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);


    hWndStatic = CreateWindowEx(0, L"static", L"Day", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    // EditText
    new_row = GAP * 4;
    g_hADYear = CreateWindowEx(0, L"Edit", L"2020",
			       WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_NUMBER | WS_GROUP | WS_TABSTOP | ES_WANTRETURN,
			       WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row, 
			       WIDGET_WIDTH, WIDGET_HEIGHT,
			       hWnd,
			       NULL,
			       NULL,
			       NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hADYear, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    // Edit_CanUndo(g_hADYear);
    SendMessage(g_hADYear, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hADYear, EM_SETLIMITTEXT, 4, 0);
    SetFocus(g_hADYear);
    SendMessage(g_hADYear, WM_SETFONT, (WPARAM) g_hFont, NULL);
  
    g_hADMonth = CreateWindowEx(0, L"Edit", L"4",
				WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_NUMBER | WS_TABSTOP,
				WINDOW_WIDTH/2 - COLUMN - MARGIN - WIDGET_WIDTH/2, new_row, 
				WIDGET_WIDTH, WIDGET_HEIGHT,
				hWnd,
				NULL,
				NULL,
				NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hADMonth, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    SendMessage(g_hADMonth, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hADMonth, EM_SETLIMITTEXT, 2, 0); 
    SendMessage(g_hADMonth, WM_SETFONT, (WPARAM) g_hFont, NULL);
 
    g_hADDay = CreateWindowEx(0, L"Edit", L"14",
			      WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | ES_NUMBER | WS_TABSTOP,
			      WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2 , new_row, 
			      WIDGET_WIDTH, WIDGET_HEIGHT,
			      hWnd,
			      NULL,
			      NULL,
			      NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hADDay, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    SendMessage(g_hADDay, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hADDay, EM_SETLIMITTEXT, 2, 0);
    SendMessage(g_hADDay, WM_SETFONT, (WPARAM) g_hFont, NULL);
  

    // Convert Button
    hWndStatic = CreateWindowEx(0, L"Button", L"Convert",
		   WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | WS_TABSTOP,
		   WINDOW_WIDTH/2 + COLUMN - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   (HMENU) CONVERT_AD_TO_BS,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);


    // --------------------------------------------------------------------------------
    // BS to AD UI

    new_row = GAP * 6 + GAP/2;
    hWndStatic = CreateWindowEx(0, L"static", L" BS to AD Converter\n(2000-2090)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row - WIDGET_HEIGHT/2, 
		   WIDGET_WIDTH * 2, WIDGET_HEIGHT + WIDGET_HEIGHT/2,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    g_hBSDisplay = CreateWindowEx(0, L"static", L" 2070 Magh 2, Sanibar ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTERIMAGE | SS_CENTER,
				  WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2, new_row - WIDGET_HEIGHT/2, 
				  WIDGET_WIDTH * 2, WIDGET_HEIGHT + WIDGET_HEIGHT/2,
				  hWnd,
				  NULL,
				  NULL,
				  NULL);
    SendMessage(g_hBSDisplay, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);



  
    // Label text
    new_row = GAP * 8;
    hWndStatic = CreateWindowEx(0, L"static", L"Year(BS)", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);
    

    hWndStatic = CreateWindowEx(0, L"static", L"Month", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - COLUMN - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    hWndStatic = CreateWindowEx(0, L"static", L"Day", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER,
		   WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    // EditText
    new_row = GAP * 9;
    g_hBSYear = CreateWindowEx(0, L"Edit", L"2000",
			       WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | SS_CENTER | WS_GROUP | WS_TABSTOP,
			       WINDOW_WIDTH/2 - COLUMN * 2 - MARGIN - WIDGET_WIDTH/2, new_row, 
			       WIDGET_WIDTH, WIDGET_HEIGHT,
			       hWnd,
			       NULL,
			       NULL,
			       NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hBSYear, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    SendMessage(g_hBSYear, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hBSYear, EM_SETLIMITTEXT, 4, 0);
    SendMessage(g_hBSYear, WM_SETFONT, (WPARAM) g_hFont, NULL);
  
    g_hBSMonth = CreateWindowEx(0, L"Edit", L"9",
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | SS_CENTER | WS_TABSTOP,
				WINDOW_WIDTH/2 - COLUMN - MARGIN - WIDGET_WIDTH/2, new_row, 
				WIDGET_WIDTH, WIDGET_HEIGHT,
				hWnd,
				NULL,
				NULL,
				NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hBSMonth, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    SendMessage(g_hBSMonth, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hBSMonth, EM_SETLIMITTEXT, 2, 0);
    SendMessage(g_hBSMonth, WM_SETFONT, (WPARAM) g_hFont, NULL);
    

    g_hBSDay = CreateWindowEx(0, L"Edit", L"17",
			      WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | SS_CENTER | WS_TABSTOP,
			      WINDOW_WIDTH/2 - MARGIN - WIDGET_WIDTH/2, new_row, 
			      WIDGET_WIDTH, WIDGET_HEIGHT,
			      hWnd,
			      NULL,
			      NULL,
			      NULL);
    g_wEditProc = (WNDPROC) SetWindowLongPtr(g_hBSDay, GWLP_WNDPROC, (LONG_PTR)&EditProc);
    SendMessage(g_hBSDay, EM_CANUNDO, NULL, NULL);
    SendMessage(g_hBSDay, EM_SETLIMITTEXT, 2, 0);
    SendMessage(g_hBSDay, WM_SETFONT, (WPARAM) g_hFont, NULL);
    


    hWndStatic = CreateWindowEx(0, L"Button", L"Convert",
		   WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER | WS_TABSTOP,
		   WINDOW_WIDTH/2 + COLUMN - MARGIN -  WIDGET_WIDTH/2, new_row, 
		   WIDGET_WIDTH, WIDGET_HEIGHT,
		   hWnd,
		   (HMENU) CONVERT_BS_TO_AD,
		   NULL,
		   NULL);
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    // Instructions
    hWndStatic = CreateWindowEx(0, L"static", L"Quick ADBS Converter Tool",
		   WS_VISIBLE | WS_CHILD | SS_LEFT | SS_SUNKEN | SS_CENTERIMAGE,
		   WINDOW_WIDTH/2 + COLUMN * 2 - 116, GAP/2,
		   232, 24, 
		   hWnd,
		   NULL,
		   NULL,
		   NULL);    
    SendMessage(hWndStatic, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);
    g_hInstructText = CreateWindowEx(0, L"static", L"",
		   WS_VISIBLE | WS_CHILD | SS_LEFT | WS_BORDER,
		   WINDOW_WIDTH/2 + COLUMN * 2 - 116, GAP/2 + 20,
		   232, WINDOW_HEIGHT - 94,
		   hWnd,
		   NULL,
		   NULL,
		   NULL);
    wchar_t help_message[1024] = L" Tool That lets you value your time by \n quickly converting AD to BS and\n vice-versa. \n\n- TAB(↹) to Change Field \n- Arrow Key Up(↑) and Down(↓) to increment by one \n- Enter(↵) for Quick Result 'el pronto!";
    SetWindowText(g_hInstructText, help_message);
    SendMessage(g_hInstructText, WM_SETFONT, (WPARAM) g_hFont, NULL);

    // Logo Image Window
    g_hLogoWin = CreateWindowEx(0, L"static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP,
				WINDOW_WIDTH/2 + COLUMN * 2 - 32, WINDOW_HEIGHT/2 + 6,
				64, 64,
				hWnd,
				NULL,
				NULL,
				NULL);
    SendMessage(g_hLogoWin, STM_SETIMAGE,(WPARAM) IMAGE_BITMAP, (LPARAM) g_hLogoBitmap);
    

    HWND hCopyrightText = CreateWindowEx(0, L"static", L"Copyright © Charicha Softwares", WS_VISIBLE | WS_CHILD | SS_CENTER,
				WINDOW_WIDTH/2 + COLUMN * 2 - 128, WINDOW_HEIGHT/2 + 64,
				256, 20,
				hWnd,
				NULL,
				NULL,
				NULL);
    SendMessage(hCopyrightText, WM_SETFONT, (WPARAM) g_hTitleFont, NULL);

    
    

    // OutputDebugStringW(L"Hello");
}


LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

    switch(uMsg){
    case WM_KEYDOWN:
	switch(wParam){
	case VK_UP:
	    AddValueEditMyDate(hWnd, 1);
	    break;
	case VK_DOWN:
	    AddValueEditMyDate(hWnd, -1);
	    break;
	case 'A':
	    
	    if(GetKeyState(VK_CONTROL) & 0x8000){
		SendMessage(hWnd, EM_SETSEL, 0, -1);
	    }
	    break;
	case VK_RETURN:
	    // MessageBox(hWnd, L"A", L"A Pressed", MB_OK);
	    break;
	}
	break;
    }
    return CallWindowProc(g_wEditProc, hWnd, uMsg, wParam, lParam);
}


void LoadImages(){
    g_hLogoBitmap = (HBITMAP) LoadImageW(NULL, L"logo.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE | LR_LOADTRANSPARENT);
    // g_hLogo_Icon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(CHARICHA_LOGO));
}



void LoadFonts(HWND hWnd){
    // TODO: Later if this piece of shit goes onto Important Radar
    // g_hFont = (HFONT) GetStockObject(DEVICE_DEFAULT_FONT);

    // int AddFontResourceExA(
    // 			   LPCSTR name,
    // 			   DWORD  fl,
    // 			   PVOID  res
    // 			   );		       

    
    g_hFont = CreateFont(16, // int    cHeight,
			 0, // int    cWidth,
			 0, // int    cEscapement,
			 0, // int    cOrientation,
			 FW_DONTCARE, // int    cWeight,
			 FALSE, // DWORD  bItalic,
			 FALSE, // DWORD  bUnderline,
			 FALSE, // DWORD  bStrikeOut,
			 DEFAULT_CHARSET, // DWORD  iCharSet,
			 OUT_OUTLINE_PRECIS, // DWORD  iOutPrecision,
			 CLIP_DEFAULT_PRECIS, // DWORD  iClipPrecision,
			 CLEARTYPE_QUALITY, // DWORD  iQuality,
			 VARIABLE_PITCH, // DWORD  iPitchAndFamily,
			 TEXT("Bahnschrift") //LPCSTR pszFaceName
			 );
    g_hTitleFont = CreateFont(17, // int    cHeight,
			      0, // int    cWidth,
			      0, // int    cEscapement,
			      0, // int    cOrientation,
			      FW_BOLD, // int    cWeight,
			      FALSE, // DWORD  bItalic,
			      FALSE, // DWORD  bUnderline,
			      FALSE, // DWORD  bStrikeOut,
			      DEFAULT_CHARSET, // DWORD  iCharSet,
			      OUT_OUTLINE_PRECIS, // DWORD  iOutPrecision,
			      CLIP_DEFAULT_PRECIS, // DWORD  iClipPrecision,
			      CLEARTYPE_QUALITY, // DWORD  iQuality,
			      VARIABLE_PITCH, // DWORD  iPitchAndFamily,
			      TEXT("Bahnschrift") //LPCSTR pszFaceName
			      );

    SendMessage(hWnd, WM_SETFONT, (WPARAM) g_hFont, NULL);

}


// --------------------------------------------------------------------------------
//

void ConvertUpdateADBS(){
    GetWindowTextW(g_hADYear, year, 100);
    GetWindowTextW(g_hADMonth, month, 100);
    GetWindowTextW(g_hADDay, day, 100);


    if(converted != nullptr) free(converted);
    converted = ad_to_bs(_wtoi(year), _wtoi(month), _wtoi(day));
    if(converted != nullptr) { 
	_itow(converted[0], year, 10);
	SetWindowTextW(g_hBSYear, year);
	_itow(converted[1], month, 10);
	SetWindowTextW(g_hBSMonth, month);
	_itow(converted[2], day, 10);
	SetWindowTextW(g_hBSDay, day);
    }    
}

void ConvertUpdateBSAD(){
    GetWindowTextW(g_hBSYear, year, 100);
    GetWindowTextW(g_hBSMonth, month, 100);
    GetWindowTextW(g_hBSDay, day, 100);


    if(converted != nullptr) free(converted);
    converted = bs_to_ad(_wtoi(year), _wtoi(month), _wtoi(day));
    if(converted != nullptr) { 
	_itow(converted[0], year, 10);
	SetWindowTextW(g_hADYear, year);
	_itow(converted[1], month, 10);
	SetWindowTextW(g_hADMonth, month);
	_itow(converted[2], day, 10);
	SetWindowTextW(g_hADDay, day);
    }    
}

void UpdateEditMyDate(){
    char text[64];
    char display[64] = " ";


    // Update AD Display
    int ad_year = CheckAndUpdateFixRange(g_hADYear, AD_MIN, AD_MAX);
    int ad_month = CheckAndUpdateFixRange(g_hADMonth, 1, 12);
    int ad_day = CheckAndUpdateFixRange(g_hADDay, 1, get_max_ad_day(ad_year, ad_month));


    // TODO: this is should be refactored AFSAP 
    int* tmp = ad_to_bs(ad_year, ad_month, ad_day);

    StringCchCatA(display, 64, (STRSAFE_LPSTR) get_day_name(tmp[3]));
    StringCchCatA(display, 64, ", ");
    GetWindowTextA(g_hADYear, text, 64);
    StringCchCatA(display, 64, text);
    StringCchCatA(display, 64, " ");
    GetWindowTextA(g_hADMonth, text, 64);
    StringCchCatA(display, 64, (STRSAFE_LPSTR)(ad_month_name(ad_month)));
    StringCchCatA(display, 64, " ");
    GetWindowTextA(g_hADDay, text, 64);
    StringCchCatA(display, 64, text);
    SetWindowTextA(g_hADDisplay, display);

    // TODO: ASAP
    free(tmp);


    // Update BS Display
    // reset texts;
    char text2[64];
    char display2[64] = " ";

    int bs_year = CheckAndUpdateFixRange(g_hBSYear, BS_MIN, BS_MAX);
    int bs_month = CheckAndUpdateFixRange(g_hBSMonth, 1, 12);
    int bs_day = CheckAndUpdateFixRange(g_hBSDay, 1, get_max_bs_day(bs_year, bs_month));

    tmp = bs_to_ad(bs_year, bs_month, bs_day);

    StringCchCatA(display2, 64, (STRSAFE_LPSTR) get_day_name(tmp[3]));
    StringCchCatA(display2, 64, ", ");
    GetWindowTextA(g_hBSYear, text2, 64);
    StringCchCatA(display2, 64, text2);
    StringCchCatA(display2, 64, " ");
    GetWindowTextA(g_hBSMonth, text2, 64);
    StringCchCatA(display2, 64, (STRSAFE_LPSTR)(bs_month_name(bs_month)));
    StringCchCatA(display2, 64, " ");
    GetWindowTextA(g_hBSDay, text2, 64);
    StringCchCatA(display2, 64, text2);
    SetWindowTextA(g_hBSDisplay, display2);

}



void AddValueEditMyDate(HWND hWnd, int i_value){ // value = +1 or -1
    wchar_t text[10];
    int value = 0;
    GetWindowTextW(hWnd, text, 10);
    value = _wtoi(text) + i_value;
    _itow(value, text, 10);
    SetWindowTextW(hWnd, text);
    UpdateEditMyDate();
}

int CheckAndUpdateFixRange(HWND hWnd, int min, int max){
    wchar_t text[10];
    int value = 0;
    GetWindowTextW(hWnd, text, 100);
    value = _wtoi(text);
    if(value < min) value = min;
    if(value > max) value = max;
    _itow(value, text, 10);
    SetWindowTextW(hWnd, text);
    return value;
}



