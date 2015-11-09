#if (_MSC_VER<=1200)
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
#endif
#include "..\m68kcore.h"
#include "..\cdbase.h"
#include "..\sh2core.h"
#include "..\sh2int.h"
#include "..\sh2_dynarec\sh2_dynarec.h"
#include "..\peripheral.h"
#include "..\scsp.h"
#include "..\vdp1.h"
#include "..\vidsoft.h"
#include "..\vidogl.h"
#include "..\cs0.h"
#include "..\debug.h"
#include "SNDVisual.h"

extern CDInterface ISOCD;
extern SoundInterface_struct SNDVisual;

M68K_struct * M68KCoreList[] = {
&M68KDummy,
#ifdef HAVE_C68K
&M68KC68K,
#endif
NULL
};

SH2Interface_struct *SH2CoreList[] = {
&SH2Interpreter,
&SH2DebugInterpreter,
#ifdef SH2_DYNAREC
&SH2Dynarec,
#endif
NULL
};

extern PerInterface_struct PERWin32;
PerInterface_struct *PERCoreList[] = {
&PERDummy,
&PERWin32,
#ifdef HAVE_DIRECTINPUT
&PERDIRECTX,
#endif
NULL
};

CDInterface *CDCoreList[] = {
&DummyCD,
&ISOCD,
#ifndef UNKNOWN_ARCH
&ArchCD,
#endif
NULL
};

SoundInterface_struct *SNDCoreList[] = {
&SNDDummy,
&SNDVisual,
#ifdef HAVE_LIBSDL
&SNDSDL,
#endif
#ifdef HAVE_LIBAL
&SNDAL,
#endif
#ifdef HAVE_DIRECTSOUND
&SNDDIRECTX,
#endif
#ifdef ARCH_IS_MACOSX
&SNDMac,
#endif
NULL
};

VideoInterface_struct *VIDCoreList[] = {
&VIDDummy,
#ifdef HAVE_LIBGL
&VIDOGL,
#endif
&VIDSoft,
NULL
};

void YuiSwapBuffers()
{
	ogl_swap_buffers();
}
int YuiErrorMsg(char *str)
{
	printf("ERROR MSG:%s\n",str);
	return 0;
}

#include <winsock2.h>
#include <ws2tcpip.h>
#include <shlwapi.h>
#include <Shlobj.h>

#ifndef	_GNU_H_WINDOWS32_SOCKETS
#if (_MSC_VER<=1200)
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
#endif

typedef int  (WSAAPI * getaddrinfo_ptr_t)  (const char *, const char* , const struct addrinfo *, struct addrinfo **);
typedef void (WSAAPI * freeaddrinfo_ptr_t) (struct addrinfo*);
getaddrinfo_ptr_t getaddrinfo_ptr;
freeaddrinfo_ptr_t freeaddrinfo_ptr;
#if (_MSC_VER<=1200)
void freeaddrinfo(struct addrinfo *a)
{
	freeaddrinfo_ptr(a);
}
int getaddrinfo(const char *a, const char*b , const struct addrinfo *c, struct addrinfo **d)
{
	return getaddrinfo_ptr(a,b,c,d);
}
#endif
int vc6_load_freeaddrinfo()
{
	static HMODULE hsock=0;
	if(hsock==0)
		hsock=LoadLibraryA("ws2_32");
	if(hsock){
		getaddrinfo_ptr = (getaddrinfo_ptr_t)GetProcAddress(hsock, "getaddrinfo");
		freeaddrinfo_ptr = (freeaddrinfo_ptr_t)GetProcAddress(hsock, "freeaddrinfo");
	}
	return 0;
}

int get_appdata_folder(char *path,int size)
{
#define APP_NAME "yabause"
#ifndef CSIDL_LOCAL_APPDATA
#define CSIDL_LOCAL_APPDATA 28
#endif
	int found=FALSE;
	ITEMIDLIST *pidl;
	IMalloc	*palloc;
	HWND hwindow=0;
	if(path==0 || size<MAX_PATH)
		return found;
	if(SHGetSpecialFolderLocation(hwindow,CSIDL_LOCAL_APPDATA,&pidl)==NOERROR){
		if(SHGetPathFromIDList(pidl,path)){
			_snprintf(path,size,"%s\\%s",path,APP_NAME);
			found=TRUE;
		}
		if(SHGetMalloc(&palloc)==NOERROR){
			palloc->lpVtbl->Free(palloc,pidl);
			palloc->lpVtbl->Release(palloc);
		}
	}
	return found;
}

yabauseinit_struct mYabauseConf;

int init_conf(char *cdpath)
{
	memset(&mYabauseConf,0,sizeof(mYabauseConf));
	mYabauseConf.m68kcoretype = M68KCORE_C68K;
	mYabauseConf.percoretype = get_percore_win32id();
	mYabauseConf.sh2coretype = SH2CORE_DEFAULT;
	//mYabauseConf.sh2coretype = SH2CORE_DYNAREC;
	mYabauseConf.vidcoretype = VIDCORE_SOFT;
	mYabauseConf.vidcoretype = VIDCORE_OGL;
	mYabauseConf.sndcoretype = SNDCORE_VISUAL; //SNDCORE_DUMMY;
	mYabauseConf.cdcoretype = CDCORE_ISO;
	mYabauseConf.carttype = CART_NONE;
	mYabauseConf.regionid = 0;
	mYabauseConf.biospath = 0;
	mYabauseConf.cdpath = 0;
	mYabauseConf.buppath = 0;
	mYabauseConf.mpegpath = 0;
	mYabauseConf.cartpath = 0;
	mYabauseConf.videoformattype = VIDEOFORMATTYPE_NTSC;

	mYabauseConf.cdpath=cdpath;
	YabauseInit(&mYabauseConf);
	//ScspSetFrameAccurate(1);
	return 0;
}

int tick_emu()
{
	//YabauseEmulate();
	YabauseExec();
	return 0;
}
int LOG_debug=1;
int CDLOG_debug=0;
int SMPC_debug=1;
int SCSP_debug=0;
int VDP1_debug=0;
int VDP2_debug=1;
int NETLINK_debug=1;

void _DebugPrintf(const char * format,va_list l)
{
	if(MainLog==NULL)
		return;
	switch(MainLog->output_type){
	case DEBUG_STDOUT:
	case DEBUG_STDERR:
	case DEBUG_STREAM:
		if(MainLog->output.stream)
			vfprintf(MainLog->output.stream,format,l);
		break;
	}
}
void DebugPrintfLOG(const char * format,...)
{
	va_list l;
	if(LOG_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfCDLOG(const char * format,...)
{
	va_list l;
	if(CDLOG_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfSMPC(const char * format,...)
{
	va_list l;
	if(SMPC_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfSCSP(const char * format,...)
{
	va_list l;
	if(SCSP_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfVDP1(const char * format,...)
{
	va_list l;
	if(VDP1_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfVDP2(const char * format,...)
{
	va_list l;
	if(VDP1_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}
void DebugPrintfNETLINK(const char * format,...)
{
	va_list l;
	if(NETLINK_debug){
		va_start(l,format);
		_DebugPrintf(format,l);
	}
}

/*
int jump_vaddr_edi_slave(){return 0; }
int jump_vaddr_ebp_slave(){return 0; }
int jump_vaddr_ebx_slave(){return 0; }
int jump_vaddr_edx_slave(){return 0; }
int jump_vaddr_ecx_slave(){return 0; }
int jump_vaddr_eax_slave(){return 0; }
int jump_vaddr_edi_master(){return 0; }
int jump_vaddr_ebp_master(){return 0; }
int jump_vaddr_ebx_master(){return 0; }
int jump_vaddr_edx_master(){return 0; }
int jump_vaddr_ecx_master(){return 0; }
int jump_vaddr_eax_master(){return 0; }
int dyna_linker(){ }
int WriteInvalidateLong(){ }
int WriteInvalidateWord(){ }
int WriteInvalidateByteSwapped(){ }
int WriteInvalidateByte(){ }
int verify_code(){ }
int macw(){ }
int macl(){ }
int div1(){ }
int slave_entry(){ }
int cc_interrupt(){ }
int master_handle_bios(){ }
int slave_handle_bios(){ }
int mmap(void *addr,unsigned int length,int prot,int flags,int fd,unsigned int offset)
{
	HANDLE hmap;
	int result=-1;
	hmap=CreateFileMapping(INVALID_HANDLE_VALUE,
			NULL,
			PAGE_EXECUTE_READWRITE,
			0,
			length,
			"SH2_MAP");
	if(hmap){
		addr=MapViewOfFile(hmap,FILE_MAP_ALL_ACCESS,0,offset,length);
		if(addr!=0)
			result=addr;
	}
	return result;
}
int munmap(void *addr,unsigned int length)
{
}
void YabauseDynarecOneFrameExec(int a,int b){ }
*/