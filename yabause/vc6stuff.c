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



LRESULT CALLBACK dialogproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
	switch(msg){
	case WM_INITDIALOG:
		test_init();
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

	}
	return 0;
}
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow )
{
	DialogBoxParam(hInstance,(LPCTSTR)IDD_MAINDLG,NULL,dialogproc,0);
}