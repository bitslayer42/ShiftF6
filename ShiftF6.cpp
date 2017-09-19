#include<windows.h>
#include<stdio.h>
#include<winuser.h>
#include<windowsx.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <string.h>
#include <time.h>
#include <Lmcons.h>

#define BUFSIZE 80


int SendKeystrokes( const TCHAR *const);

//int main(void)
//{
//   HWND stealth; /*creating stealth (window is not visible)*/
//   AllocConsole();
//   stealth=FindWindowA("ConsoleWindowClass",NULL);
//   ShowWindow(stealth,0); //hide window
//
//   int t=listenForF6();
//
//   return t;
//}  

bool keydown(int key)
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

int listenForF6()
{
    while(1)
    {
        Sleep(10);/*to prevent 100% cpu usage*/

		if(keydown(VK_F6) && keydown(VK_SHIFT))	//Shift F6: send message "Electronically signed by onejtw on Wed Mar 30 11:35:53 AM"    
        {
			struct tm newtime;
			char am_pm[] = "AM";
			__time64_t long_time;
			char timebuf[26];
			errno_t err;
			char sendStr[100];

			// Get time as 64-bit integer.
			_time64( &long_time ); 
			// Convert to local time.
			err = _localtime64_s( &newtime, &long_time ); 
			if (err)
			{
				printf("Invalid argument to _localtime64_s.");
				exit(1);
			}
			if( newtime.tm_hour >= 12 )        // Set up extension. 
					strcpy_s( am_pm, sizeof(am_pm), "PM" );
			if( newtime.tm_hour > 12 )        // Convert from 24-hour 
					newtime.tm_hour -= 12;    // to 12-hour clock. 
			if( newtime.tm_hour == 0 )        // Set hour to 12 if midnight.
					newtime.tm_hour = 12;

			// Convert time to an ASCII representation. 
			err = asctime_s(timebuf, 26, &newtime);
			if (err)
			{
			   printf("Invalid argument to asctime_s.");
			   exit(1);
			}

			//get username
			char username[UNLEN+1];
			DWORD username_len = UNLEN+1;
			GetUserName(username, &username_len);

			//construct string to send
			printf( "Electronically signed by %s on %.19s %s\n", username, timebuf, am_pm );
			sprintf_s(sendStr, "Electronically signed by %s on %.19s %s\n", username, timebuf, am_pm );

			int retvar = SendKeystrokes(sendStr);
			Sleep(500);
        }
		else if(keydown(VK_F6))	//F6: send time "11:35 AM"  
		{
			struct tm newtime;
			char am_pm[] = "AM";
			__time64_t long_time;
			char timebuf[26];
			std::string timebuf2;
			errno_t err;
			char sendStr[100];

			// Get time as 64-bit integer.
			_time64( &long_time ); 
			// Convert to local time.
			err = _localtime64_s( &newtime, &long_time ); 

			if( newtime.tm_hour > 12 )        // Set up extension. 
					strcpy_s( am_pm, sizeof(am_pm), "PM" );
			if( newtime.tm_hour > 12 )        // Convert from 24-hour 
					newtime.tm_hour -= 12;    // to 12-hour clock. 
			if( newtime.tm_hour == 0 )        // Set hour to 12 if midnight.
					newtime.tm_hour = 12;

			// Convert time to an ASCII representation. 
			err = asctime_s(timebuf, 26, &newtime);

			timebuf2 = timebuf;
			timebuf2 = timebuf2.substr(11,5);
			strcpy_s (timebuf, timebuf2.c_str());

			//construct string to send
			printf( "%s %s\n", timebuf, am_pm );
			sprintf_s(sendStr, "%s %s\n", timebuf, am_pm );

			int retvar = SendKeystrokes(sendStr);
			Sleep(500);
		}
 
    }                  

    return EXIT_SUCCESS;                            
}                                                


int SendKeystrokes( const TCHAR *const text ) 
{
    UINT i, character_count, keystrokes_to_send, keystrokes_sent = 0;

    character_count = strlen( text );
    keystrokes_to_send = character_count * 2;

	INPUT input;
	for( i = 0; i < character_count; ++i )
    {
		//send keydown: 
		input.type = INPUT_KEYBOARD;
		input.ki.time = 0;
		input.ki.dwExtraInfo = GetMessageExtraInfo();
		input.ki.wScan = text[ i ]; 
		input.ki.dwFlags = KEYEVENTF_UNICODE;
		input.ki.wVk = 0; //VK_MENU;
		keystrokes_sent += SendInput( 1, &input, sizeof( INPUT ) );
		//Sleep(2);

		//send keyup: 
		input.type = INPUT_KEYBOARD;
		input.ki.time = 0;
		input.ki.dwExtraInfo = GetMessageExtraInfo();
		input.ki.wScan = text[ i ]; 
		input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		input.ki.wVk = 0; //VK_MENU;
		keystrokes_sent += SendInput( 1, &input, sizeof( INPUT ) );
		Sleep(5);
	}


    return keystrokes_sent == keystrokes_to_send;
}



//keylogger based on http://www.ccodechamp.com/c-program-of-keylogger-or-keystroke-logger-c-codechamp/
//send keystrokes from http://stackoverflow.com/questions/2113950/how-to-send-keystrokes-to-a-window
//date to string from: https://msdn.microsoft.com/en-us/library/a442x3ye.aspx
