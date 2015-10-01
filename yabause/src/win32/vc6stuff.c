#include "resource.h"

#define CHAR_BIT 8
unsigned short _byteswap_ushort(unsigned short value) 
{ 
    return (value >> CHAR_BIT) | (value << CHAR_BIT); 
}
unsigned long _byteswap_ulong(unsigned long value) 
{ 
    return _byteswap_ushort((unsigned short)value) << (2 * CHAR_BIT) | 
           _byteswap_ushort((unsigned short)(value >> (2 * CHAR_BIT))); 
} 

int YuiErrorMsg(char *str)
{
	return 0;
}

//int vnsprintf(char *
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef	_GNU_H_WINDOWS32_SOCKETS
	typedef struct addrinfo {
	  int             ai_flags;
	  int             ai_family;
	  int             ai_socktype;
	  int             ai_protocol;
	  size_t          ai_addrlen;
	  char            *ai_canonname;
	  struct sockaddr  *ai_addr;
	  struct addrinfo  *ai_next;
	} ADDRINFOA, *PADDRINFOA;
#endif

typedef int  (WSAAPI * getaddrinfo_ptr_t)  (const char *, const char* , const struct addrinfo *, struct addrinfo **);
typedef void (WSAAPI * freeaddrinfo_ptr_t) (struct addrinfo*);
getaddrinfo_ptr_t getaddrinfo_ptr;
freeaddrinfo_ptr_t freeaddrinfo_ptr;

void freeaddrinfo(struct addrinfo *a)
{
	freeaddrinfo_ptr(a);
}
int getaddrinfo(const char *a, const char*b , const struct addrinfo *c, struct addrinfo **d)
{
	return getaddrinfo_ptr(a,b,c,d);
}
int vc6_load_freeaddrinfo()
{
	HMODULE hWsock;
	hWsock = LoadLibraryA("ws2_32");
	if(hWsock){
		getaddrinfo_ptr = (getaddrinfo_ptr_t)GetProcAddress(hWsock, "getaddrinfo");
		freeaddrinfo_ptr = (freeaddrinfo_ptr_t)GetProcAddress(hWsock, "freeaddrinfo");
	}
	return 0;
}

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

LRESULT CALLBACK dialogproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	static int timer=0;
	static HANDLE hgrippy=0;
	switch(msg){
	case WM_INITDIALOG:
		test_init();
		timer=SetTimer(hwnd,1337,10,NULL);
		create_grippy(hwnd,&hgrippy);
		break;
	case WM_COMMAND:
		switch(LOWORD(wparam)){
		case IDC_INITEMU:
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
			tick_emu();
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
	DialogBoxParam(hInstance,(LPCTSTR)IDD_MAINDLG,NULL,dialogproc,0);
}