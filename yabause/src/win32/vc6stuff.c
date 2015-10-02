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

#include "..\m68kcore.h"
#include "..\cdbase.h"
#include "..\sh2core.h"
#include "..\sh2int.h"
#include "..\peripheral.h"
#include "..\scsp.h"
#include "..\vdp1.h"
#include "..\vidsoft.h"
#include "..\cs0.h"
#include "..\debug.h"


extern CDInterface ISOCD;

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
}
int YuiErrorMsg(char *str)
{
	return 0;
}


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
	static HMODULE hsock=0;
	if(hsock==0)
		hsock=LoadLibraryA("ws2_32");
	if(hsock){
		getaddrinfo_ptr = (getaddrinfo_ptr_t)GetProcAddress(hsock, "getaddrinfo");
		freeaddrinfo_ptr = (freeaddrinfo_ptr_t)GetProcAddress(hsock, "freeaddrinfo");
	}
	return 0;
}

yabauseinit_struct mYabauseConf;

int init_conf(char *cdpath)
{
	memset(&mYabauseConf,0,sizeof(mYabauseConf));
	mYabauseConf.m68kcoretype = M68KCORE_C68K;
	mYabauseConf.percoretype = get_percore_win32id();
	mYabauseConf.sh2coretype = SH2CORE_DEFAULT;
	mYabauseConf.vidcoretype = VIDCORE_SOFT;
	mYabauseConf.sndcoretype = SNDCORE_DUMMY;
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
int SCSP_debug=1;
int VDP1_debug=1;
int VDP2_debug=1;
int NETLINK_debug=1;

void _DebugPrintf(const char * format,va_list l)
{
	if(MainLog==NULL)
		return;
	if(MainLog->output_type==DEBUG_STDOUT){
		if(MainLog->output.stream)
			vfprintf(MainLog->output.stream,format,l);
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