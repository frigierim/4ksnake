#include "include/global.h"
#include "include/config.h"
#include "startup/initterm.h"

extern "C" const int _fltused = 0;

static GAME_STRUCT game ={{0}};

//----------------------------------------------------------------------------

extern void process_frame(GAME_STRUCT *p_game, bool up, bool down, bool left, bool right);
extern void init_game(GAME_STRUCT *p_game);

int main(int argc, char **argv )
{   
    const BITMAPINFO bmi = {{sizeof(BITMAPINFOHEADER),SXRES,-SYRES,1,32,BI_RGB,0,0,0,0,0},{0,0,0,0}};

    SetProcessDpiAwarenessContext( DPI_AWARENESS_CONTEXT_SYSTEM_AWARE );

    int screen_x = (GetSystemMetrics(SM_CXFULLSCREEN) - IXRES) >> 1;
    int screen_y = (GetSystemMetrics(SM_CYFULLSCREEN) - IYRES) >> 1; 

    // Minimize all windows
    HWND lHwnd = FindWindow("Shell_TrayWnd",NULL);
    SendMessage(lHwnd,WM_COMMAND,MIN_ALL,0); 
    // Wait for the desktop to be clean, simulating big number crunching is happening :)
    Sleep(1500);

    HWND hWnd = CreateWindowEx( WS_EX_TOPMOST, WINDOW_TITLE, 0, WS_VISIBLE|WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, screen_x, screen_y, SXRES, SYRES, 0, 0, 0, 0 );
    HDC hDC = GetDC( hWnd );

    BOOL done = FALSE;
    init_game(&game);
    do
    {
        unsigned int start = timeGetTime();
        done = GetAsyncKeyState(VK_ESCAPE);

        process_frame(  &game, 
                        GetAsyncKeyState(VK_UP),
                        GetAsyncKeyState(VK_DOWN),
                        GetAsyncKeyState(VK_LEFT),
                        GetAsyncKeyState(VK_RIGHT));

	    StretchDIBits(hDC,screen_x, screen_y,IXRES,IYRES,0,0,SXRES,SYRES,game.buffer,&bmi,DIB_RGB_COLORS,SRCCOPY);
        Sleep(start + MS_PER_FRAME - timeGetTime());
    }while( !done);

    // Bring all back up again.
    SendMessage(lHwnd,WM_COMMAND,MIN_ALL_UNDO,0); 
    return 0;
}


extern "C" void __cdecl WinMainCRTStartup()
{
    
    // set up our minimal cheezy atexit table
    _atexit_init();

    // Call C++ constructors
    _initterm( __xc_a, __xc_z );

    //mainret = main( argc, _ppszArgv, 0 );
    int mainret = main(0, nullptr);

    _DoExit();

    ExitProcess(mainret);
}

extern "C" int __cdecl _purecall(void)  
{  
    return 0;  
}  

#pragma function(memset)
void * __cdecl memset(void *pTarget, int value, size_t cbTarget) {
	unsigned char *p = static_cast<unsigned char *>(pTarget);
	while (cbTarget-- > 0) {
		*p++ = static_cast<unsigned char>(value);
	}
	return pTarget;
}
