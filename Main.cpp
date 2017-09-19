#include "resource.h"
#include "ShiftF6.h"

/*variables*/
UINT WM_TASKBAR = 0;
HWND Hwnd;
HMENU Hmenu;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = TEXT("Shift F6-electronic signature");
char szClassName[ ] = "Shift F6-electronic signature";

DWORD WINAPI myThread(LPVOID lpParameter);

/*procedures  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void minimize();
void restore();
void InitNotifyIconData();



int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hIconSm = LoadIcon (GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    Hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               szClassName,       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               520,                 /* The programs width */
               225,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );
  
			static HWND hwndLabel1 = CreateWindow("STATIC","Shift F6",
			WS_VISIBLE | WS_CHILD | SS_CENTER,
			220, 10, 55, 20, //x,y,w,h
			Hwnd,NULL,hThisInstance,NULL);
			
			static HWND hwndLabel2 = CreateWindow("STATIC"," Utility program to help fill and sign forms.",
			WS_VISIBLE | WS_CHILD | SS_LEFT,
			10, 35, 490, 20,
			Hwnd,NULL,hThisInstance,NULL);

			static HWND hwndLabel3 = CreateWindow("STATIC"," Press Shift F6 to send a message to the current text field like:",
			WS_VISIBLE | WS_CHILD | SS_LEFT,
			10, 55, 490, 20,
			Hwnd,NULL,hThisInstance,NULL);

			static HWND hwndLabel4 = CreateWindow("STATIC"," \"Electronically signed by userid on Mon Apr 04 12:00 PM\"",
			WS_VISIBLE | WS_CHILD | SS_LEFT,
			10, 75, 490, 20,
			Hwnd,NULL,hThisInstance,NULL);

			static HWND hwndLabel5 = CreateWindow("STATIC"," Press F6 to send the current time.",
			WS_VISIBLE | WS_CHILD | SS_LEFT,
			10, 95, 490, 20,
			Hwnd,NULL,hThisInstance,NULL);

			static HWND hwndLabel6 = CreateWindow("STATIC","Copyright 2016 Community CarePartners.",
			WS_VISIBLE | WS_CHILD | SS_CENTER,
			10, 120, 490, 20,
			Hwnd,NULL,hThisInstance,NULL);

	/*Initialize the NOTIFYICONDATA structure only once*/
    InitNotifyIconData();
    /* Make the window visible on the screen */
    ShowWindow (Hwnd, nCmdShow); 
	minimize() ;

	//start second thread to run Shift F6 code
	unsigned int myCounter = 0;
	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, myThread, &myCounter, 0, &myThreadID);
	

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }


    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    if ( message==WM_TASKBAR && !IsWindowVisible( Hwnd ) )
    {
        minimize();
        return 0;
    }

    switch (message)                  /* handle the messages */
    {
    case WM_ACTIVATE:
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
        break;
    case WM_CREATE:

        ShowWindow(Hwnd, SW_HIDE);
        Hmenu = CreatePopupMenu();
        AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT,  TEXT( "Quit" ) );

        break;

    case WM_SYSCOMMAND:
        /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter 
		are used internally by the system. To obtain the correct result when testing the value of wParam, 
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/ 
		
		switch( wParam & 0xFFF0 )  
        {
        case SC_MINIMIZE:
        case SC_CLOSE:  
            minimize() ;
            return 0 ;
            break;
        }
        break;

        
        // Our user defined WM_SYSICON message.
    case WM_SYSICON:
    {

        switch(wParam)
        {
        case ID_TRAY_APP_ICON:
        SetForegroundWindow(Hwnd);

            break;
        }


        if (lParam == WM_LBUTTONUP)
        {

            restore();
        }
        else if (lParam == WM_RBUTTONDOWN) 
        {
            // Get current mouse position.
            POINT curPoint ;
            GetCursorPos( &curPoint ) ;
			SetForegroundWindow(Hwnd);

            // TrackPopupMenu blocks the app until TrackPopupMenu returns

            UINT clicked = TrackPopupMenu(Hmenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hwnd,NULL);


            
            SendMessage(hwnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                PostQuitMessage( 0 ) ;
            }
        }
    }
    break;

    // intercept the hittest message..
    case WM_NCHITTEST:
    {
       UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
        if(uHitTest == HTCLIENT)
            return HTCAPTION;
        else
            return uHitTest;
    }

    case WM_CLOSE:

        minimize() ;
        return 0;
        break;

    case WM_DESTROY:

        PostQuitMessage (0);
        break;

    }

    return DefWindowProc( hwnd, message, wParam, lParam ) ;
}


void minimize()
{
    // hide the main window
    ShowWindow(Hwnd, SW_HIDE);
}


void restore()
{
    ShowWindow(Hwnd, SW_SHOW);
}

void InitNotifyIconData()
{

    memset( &notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = Hwnd;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
    //notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(NULL),      MAKEINTRESOURCE(ICO1) ) ;
	
	notifyIconData.hIcon = (HICON)::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1), IMAGE_ICON, 16, 16, LR_SHARED);
    //strncpy(notifyIconData.szTip, szTIP, sizeof(szTIP));
}

DWORD WINAPI myThread(LPVOID lpParameter)
{
	//Install listener to background waiting for user to press F6 or Shift F6
	int retval = listenForF6();
	return retval;
}

//http://www.dreamincode.net/forums/topic/258176-putting-a-program-icon-in-the-system-tray-with-c/