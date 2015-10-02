#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <fcntl.h>
#include "resource.h"

int show_console=0;
int move_console(int x,int y,int w,int h)
{
	char title[MAX_PATH]={0}; 
	GetConsoleTitle(title,sizeof(title));
	if(title[0]!=0){
		HWND hcon; 
		hcon=FindWindow(NULL,title);
		SetWindowPos(hcon,0,x,y,w,h,SWP_NOZORDER);
	}
	return 0;
}
int resize_console(int w,int h)
{
	HANDLE houtput;
	houtput=GetStdHandle(STD_OUTPUT_HANDLE);
	if(houtput){
		COORD coord;
		coord.X=w;
		coord.Y=h;
		SetConsoleScreenBufferSize(houtput,coord);
	}
	return 0;
}
void open_console()
{
	char title[MAX_PATH]={0}; 
	HWND hcon; 
	FILE *hf;
	static BYTE consolecreated=FALSE;
	static int hcrt=0;
	
	show_console=1;
	if(consolecreated==TRUE)
	{
		GetConsoleTitle(title,sizeof(title));
		if(title[0]!=0){
			hcon=FindWindow(NULL,title);
			ShowWindow(hcon,SW_SHOW);
		}
		hcon=(HWND)GetStdHandle(STD_INPUT_HANDLE);
		FlushConsoleInputBuffer(hcon);
		return;
	}
	AllocConsole(); 
	hcrt=_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),_O_TEXT);

	fflush(stdin);
	hf=_fdopen(hcrt,"w"); 
	*stdout=*hf; 
	setvbuf(stdout,NULL,_IONBF,0);
	GetConsoleTitle(title,sizeof(title));
	if(title[0]!=0){
		hcon=FindWindow(NULL,title);
		ShowWindow(hcon,SW_SHOW); 
		SetForegroundWindow(hcon);
	}
	consolecreated=TRUE;
}
void hide_console()
{
	char title[MAX_PATH]={0}; 
	HANDLE hcon; 
	
	show_console=0;
	GetConsoleTitle(title,sizeof(title));
	if(title[0]!=0){
		hcon=FindWindow(NULL,title);
		ShowWindow(hcon,SW_HIDE);
		SetForegroundWindow(hcon);
	}
}

HINSTANCE ghinstance=0;
HANDLE thread1obj=0;
HWND ghstatusbar=0;
HMENU ghmenu=0;

static int fps_counter=0;
static int thread_busy=0;
static int pause_thread=0;
extern unsigned char *dispbuffer;
#define BUF_WIDTH 704
#define BUF_HEIGHT 512
static char tmp_buf[BUF_WIDTH*BUF_HEIGHT*4];

int draw_fbuffer(HDC hdc,RECT *rect)
{
	BITMAPINFO bmi;
	if(hdc==0)
		return 0;
	memset(&bmi,0,sizeof(bmi));
	bmi.bmiHeader.biBitCount=32;
	bmi.bmiHeader.biWidth=BUF_WIDTH/2;
	bmi.bmiHeader.biHeight=BUF_HEIGHT;
	bmi.bmiHeader.biPlanes=1;
	//bmi.biSizeImage=704*512*4;
	//bmi.biXPelsPerMeter=12;
	//bmi.biYPelsPerMeter=12;
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	if(dispbuffer){
		int w,h;
		w=rect->right;
		h=rect->bottom;
		if(w<0)
			w=0;
		if(h<0)
			h=0;
		{
			int i;
			for(i=0;i<BUF_WIDTH*BUF_HEIGHT;i+=4){
				char r,g,b;
				int offset;
				offset=i;
				r=dispbuffer[offset];
				g=dispbuffer[offset+1];
				b=dispbuffer[offset+2];
				tmp_buf[offset]=b;
				tmp_buf[offset+1]=g;
				tmp_buf[offset+2]=r;
				tmp_buf[offset+3]=0;
			}
		}
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
			0,BUF_HEIGHT/2, //x,y source
			BUF_WIDTH/2,-BUF_HEIGHT/2, //w,h source
			tmp_buf,&bmi,DIB_RGB_COLORS,SRCCOPY);
	}
	if(thread_busy){
		SetBkColor(hdc,0);
		SetTextColor(hdc,0xFFFFFF);
		{
			RECT rect={0};
			char str[20];
			int len;
			_snprintf(str,sizeof(str),"%i",fps_counter);
			len=strlen(str);
			rect.right=100;
			rect.bottom=100;
			DrawText(hdc,str,len,&rect,DT_LEFT);
		}
	}
	return 0;
}
int render_thread()
{
	if(thread1obj==0)
		return 0;
	while(TRUE){
		DWORD result;
		pause_thread=0;
		result=WaitForSingleObject(thread1obj,INFINITE);
		if(WAIT_OBJECT_0==result){
			while(TRUE){
				thread_busy=1;
				tick_emu();
				fps_counter++;
				if(pause_thread)
					break;
			}
			thread_busy=0;
		}
	}
}
int wait_stop_emu_thread()
{
	DWORD delta,tick=GetTickCount();
	while(TRUE){
		pause_thread=1;
		Sleep(1);
		if(!thread_busy)
			break;
		delta=GetTickCount()-tick;
		if(delta>1000)
			break;
	}
	return 0;
}
int create_status_bar_parts(HWND hwnd,HWND hstatus)
{
	if(hwnd!=0 && hstatus!=0){
		int parts[2]={-1,-1};
		RECT rect={0};
		GetClientRect(hwnd,&rect);
		parts[0]=rect.right/2;
		return SendMessage(hstatus,SB_SETPARTS,2,(LPARAM)&parts);
	}
	return FALSE;
}
LRESULT CALLBACK debug_opt_dlg(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	extern int LOG_debug,CDLOG_debug,NETLINK_debug,SMPC_debug,SCSP_debug,VDP1_debug,VDP2_debug;
	struct CHKMAP{
		int idc;
		int *val;
	}static chkmap[]={
		{IDC_SHOW_CONSOLE,&show_console},
		{IDC_ENABLE_DEBUGLOG,&LOG_debug},
		{IDC_ENABLE_CDDEBUG_LOG,&CDLOG_debug},
		{IDC_ENABLE_NETLINK_LOG,&NETLINK_debug},
		{IDC_ENABLE_SCSP_LOG,&SCSP_debug},
		{IDC_ENABLE_VDP1_LOG,&VDP1_debug},
		{IDC_ENABLE_VDP2_LOG,&VDP2_debug},
		{IDC_ENABLE_SMPC_LOG,&SMPC_debug}
	};
	switch(msg){
	case WM_INITDIALOG:
		{
			int i;
			for(i=0;i<sizeof(chkmap)/sizeof(struct CHKMAP);i++){
				CheckDlgButton(hwnd,chkmap[i].idc,*chkmap[i].val?BST_CHECKED:BST_UNCHECKED);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wparam)){
		case IDC_SHOW_CONSOLE:
			if(IsDlgButtonChecked(hwnd,IDC_SHOW_CONSOLE))
				open_console();
			else
				hide_console();
			break;
		case IDC_ENABLE_DEBUGLOG:
		case IDC_ENABLE_CDDEBUG_LOG:
		case IDC_ENABLE_NETLINK_LOG:
		case IDC_ENABLE_SCSP_LOG:
		case IDC_ENABLE_VDP1_LOG:
		case IDC_ENABLE_VDP2_LOG:
		case IDC_ENABLE_SMPC_LOG:
			{
				int i,idc=LOWORD(wparam);
				for(i=0;i<sizeof(chkmap)/sizeof(struct CHKMAP);i++){
					if(idc==chkmap[i].idc){
						*chkmap[i].val=BST_CHECKED==IsDlgButtonChecked(hwnd,idc)?1:0;
						break;
					}
				}
			}
			break;
		case IDCANCEL:
		case IDOK:
			EndDialog(hwnd,0);
			break;
		}
		break;					
	}
	return 0;
}
LRESULT CALLBACK dialogproc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	static int timer=0;
	static int status_height=50;
	switch(msg){
	case WM_INITDIALOG:
		init_conf("C:\\EMU\\games\\RS\\023 Radiant Silvergun (J).cue");
		timer=SetTimer(hwnd,1337,33,NULL);
		LogStart();
		open_console();
		resize_console(120,500);
		{
			RECT rect={0};
			GetWindowRect(hwnd,&rect);
			move_console(0,rect.bottom,1200,600);
		}
		SetEvent(thread1obj);
		ghstatusbar=CreateStatusWindow(WS_CHILD|WS_VISIBLE,"",hwnd,IDC_STATUS);
		{
			RECT rect;
			if(GetWindowRect(ghstatusbar,&rect)){
				status_height=rect.bottom-rect.top;
			}
		}
		create_status_bar_parts(hwnd,ghstatusbar);
		ghmenu=LoadMenu(ghinstance,MAKEINTRESOURCE(IDR_MENU1));
		if(ghmenu)
			SetMenu(hwnd,ghmenu);
		break;
	case WM_COMMAND:
		switch(LOWORD(wparam)){
		case IDCANCEL:
			EndDialog(hwnd,0);
			break;
		case IDM_DEBUG_OPTIONS:
			DialogBoxParam(ghinstance,MAKEINTRESOURCE(IDD_DEBUG_OPTIONS),hwnd,debug_opt_dlg,0);
			break;
		case IDM_SAVE_STATE1:
		case IDM_SAVE_STATE2:
		case IDM_SAVE_STATE3:
		case IDM_SAVE_STATE4:
		case IDM_SAVE_STATE5:
			wait_stop_emu_thread();
			YabSaveStateSlot("c:\\temp\\",LOWORD(wparam)-IDM_SAVE_STATE1);
			SetEvent(thread1obj);
			break;
		case IDM_LOAD_STATE1:
		case IDM_LOAD_STATE2:
		case IDM_LOAD_STATE3:
		case IDM_LOAD_STATE4:
		case IDM_LOAD_STATE5:
			wait_stop_emu_thread();
			YabLoadStateSlot("c:\\temp\\",LOWORD(wparam)-IDM_LOAD_STATE1);
			SetEvent(thread1obj);
			break;
		}
		break;
	case WM_SIZE:
		resize_maindlg(hwnd);
		break;
	case WM_TIMER:
		{
			//InvalidateRect(GetDlgItem(hwnd,IDC_FBUFFER),NULL,0);
			InvalidateRect(hwnd,NULL,0);
			PERWin32HandleEvents();
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			RECT rect={0};
			GetClientRect(hwnd,&rect);
			rect.bottom-=status_height;
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
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow )
{
	ghinstance=hInstance;
	thread1obj=CreateEvent(NULL,FALSE,FALSE,"THREAD1");
	if(thread1obj){
		_beginthread(render_thread,0,NULL);
	}
	else
		MessageBox(NULL,"Cant create thread object","ERROR",MB_OK|MB_SYSTEMMODAL);

	DialogBoxParam(hInstance,(LPCTSTR)IDD_MAINDLG,NULL,dialogproc,0);
}