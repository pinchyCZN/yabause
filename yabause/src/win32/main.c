#include "..\m68kcore.h"
#include "..\cdbase.h"
#include "..\sh2core.h"
#include "..\sh2int.h"
#include "..\peripheral.h"
#include "..\scsp.h"
#include "..\vdp1.h"
#include "..\vidsoft.h"


extern CDInterface SPTICD;

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

PerInterface_struct *PERCoreList[] = {
&PERDummy,
#ifdef HAVE_LIBSDL
&PERSDLJoy,
#endif
#ifdef __APPLE__
&PERMacJoy,
#endif
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