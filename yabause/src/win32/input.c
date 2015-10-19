#include <windows.h>
#include "..\peripheral.h"

#define PERCORE_WIN32 32
static PerPad_struct *pad1=0;

int PERWin32Init(void)
{
	PerPortReset();
	pad1=PerPadAdd(&PORTDATA1);
	return 0;
}
void PERWin32DeInit(void)
{
}
int PERWin32HandleEvents(void)
{
	unsigned char keys[256];
	static unsigned char last_keys[256];
	struct KEYMAP{
		void (*pressed)(PerPad_struct *p);
		void (*released)(PerPad_struct *p);
		char key;
	}static keymap[]={
		{PerPadAPressed,PerPadAReleased,'A'},
		{PerPadBPressed,PerPadBReleased,'S'},
		{PerPadCPressed,PerPadCReleased,'D'},
		{PerPadXPressed,PerPadXReleased,'Z'},
		{PerPadYPressed,PerPadYReleased,'X'},
		{PerPadZPressed,PerPadZReleased,'C'},
		{PerPadStartPressed,PerPadStartReleased,VK_RETURN},
		{PerPadStartPressed,PerPadStartReleased,VK_SPACE},
		{PerPadUpPressed,PerPadUpReleased,VK_UP},
		{PerPadDownPressed,PerPadDownReleased,VK_DOWN},
		{PerPadLeftPressed,PerPadLeftReleased,VK_LEFT},
		{PerPadRightPressed,PerPadRightReleased,VK_RIGHT}
	};
	
	if(pad1==0)
		return 0;
	if(GetKeyboardState(keys)){
		int i;
		for(i=0;i<sizeof(keymap)/sizeof(struct KEYMAP);i++){
			char a,b;
			a=keys[keymap[i].key]&0x80;
			b=last_keys[keymap[i].key]&0x80;
			if(a!=b){
				if(a)
					keymap[i].pressed(pad1);
				else
					keymap[i].released(pad1);
			}
		}
		memcpy(last_keys,keys,sizeof(last_keys));
	}
	return 0;
}
u32 PERWin32Scan(u32 flags)
{
	return 0;
}
void PERWin32Flush(void)
{
}
void PERWin32KeyName(UNUSED u32 key,char *name,UNUSED int size)
{
	*name=0;
}

PerInterface_struct PERWin32 = {
PERCORE_WIN32,
"Win32 Input Interface",
PERWin32Init,
PERWin32DeInit,
PERWin32HandleEvents,
PERWin32Scan,
0,
PERWin32Flush,
PERWin32KeyName
};

int get_percore_win32id()
{
	return PERCORE_WIN32;
}