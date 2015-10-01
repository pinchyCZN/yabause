#include <windows.h>
#include "resource.h"

HANDLE thread1obj;

static int fps_counter=0;
extern unsigned char *dispbuffer;
#define BUF_WIDTH 704
#define BUF_HEIGHT 512
int draw_fbuffer(HDC hdc,RECT *rect)
{
	BITMAPINFOHEADER bmi;
	if(hdc==0)
		return 0;
	memset(&bmi,0,sizeof(BITMAPINFOHEADER));
	bmi.biBitCount=32;
	bmi.biWidth=704;
	bmi.biHeight=512;
	bmi.biPlanes=1;
	//bmi.biSizeImage=704*512*4;
	//bmi.biXPelsPerMeter=12;
	//bmi.biYPelsPerMeter=12;
	bmi.biSize=sizeof(BITMAPINFOHEADER);

	if(dispbuffer){
		int w,h;
		w=rect->right;
		h=rect->bottom;
		if(w<0)
			w=0;
		if(h<0)
			h=0;
		/*
		SetDIBitsToDevice(hdc,0,0,704,512,
			0,0, //src xy
			0,512, //startscan,scanlines
			dispbuffer,
			(BITMAPINFO*)&bmi,DIB_RGB_COLORS);
		*/
		
		StretchDIBits(hdc,
			0,0, //x,y destination
			w,h, //w,h destination
			0,BUF_HEIGHT, //x,y source
			BUF_WIDTH,-BUF_HEIGHT, //w,h source
			dispbuffer,&bmi,DIB_RGB_COLORS,SRCCOPY);
	}
	SetBkColor(hdc,0);
	SetTextColor(hdc,0xFFFFFF);
	{
		RECT rect={0};
		char str[20];
		int len;
		_snprintf(str,sizeof(str),"%i",fps_counter++);
		len=strlen(str);
		rect.right=100;
		rect.bottom=100;
		DrawText(hdc,str,len,&rect,DT_LEFT);
	}
	return 0;
}
int render_thread()
{
	if(thread1obj==0)
		return 0;
	while(TRUE){
		DWORD result=WaitForSingleObject(thread1obj,INFINITE);
		if(WAIT_OBJECT_0==result){
			while(TRUE){
				tick_emu();
			}
		}
	}
}

LRESULT CALLBACK dialogproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	static int timer=0;
	static HANDLE hgrippy=0;
	switch(msg){
	case WM_INITDIALOG:
		init_conf("C:\\EMU\\games\\RS\\023 Radiant Silvergun (J).cue");
		timer=SetTimer(hwnd,1337,33,NULL);
		create_grippy(hwnd,&hgrippy);
		SetEvent(thread1obj);
		break;
	case WM_COMMAND:
		switch(LOWORD(wparam)){
		case IDC_INITEMU:
			SetEvent(thread1obj);
			break;
		case IDOK:
		case IDCANCEL:
			EndDialog(hwnd,0);
			break;
		}
		break;
	case WM_SIZE:
		move_grippy(hwnd,hgrippy);
		resize_maindlg(hwnd);
		break;
	case WM_TIMER:
		{
			//InvalidateRect(GetDlgItem(hwnd,IDC_FBUFFER),NULL,0);
			InvalidateRect(hwnd,NULL,0);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			RECT rect={0};
			GetClientRect(hwnd,&rect);
			rect.right-=10;
			rect.bottom-=50;
			if(rect.right<0)
				rect.right=0;
			if(rect.bottom<0)
				rect.bottom=0;
			hdc=BeginPaint(hwnd,&ps);
			draw_fbuffer(hdc,&rect);
			EndPaint(hwnd,&ps);
		}
		break;
	}
	return 0;
}
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow )
{
	thread1obj=CreateEvent(NULL,FALSE,FALSE,"THREAD1");
	if(thread1obj){
		_beginthread(render_thread,0,NULL);
	}
	else
		MessageBox(NULL,"Cant create thread object","ERROR",MB_OK|MB_SYSTEMMODAL);

	DialogBoxParam(hInstance,(LPCTSTR)IDD_MAINDLG,NULL,dialogproc,0);
}