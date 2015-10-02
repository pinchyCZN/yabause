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
int is_key_pressed(unsigned char *keys,int index)
{
	int result=0;
	int val=keys[index];
	if(val&0x80)
		result=1;
	return result;
}
int PERWin32HandleEvents(void)
{
	unsigned char keys[256];
	struct KEYMAP{
		void (*pressed)(PerPad_struct *p);
		void (*released)(PerPad_struct *p);
		char key;
	}keymap[]={
		{PerPadAPressed,PerPadAReleased,'A'},
		{PerPadBPressed,PerPadBReleased,'S'},
		{PerPadCPressed,PerPadCReleased,'D'},
		{PerPadXPressed,PerPadXReleased,'Z'},
		{PerPadYPressed,PerPadYReleased,'X'},
		{PerPadZPressed,PerPadZReleased,'C'},
		{PerPadStartPressed,PerPadStartReleased,VK_RETURN},
		{PerPadStartPressed,PerPadStartReleased,VK_SPACE},
		{PerPadUpReleased,PerPadUpReleased,VK_UP},
		{PerPadDownPressed,PerPadDownReleased,VK_DOWN},
		{PerPadLeftReleased,PerPadLeftReleased,VK_LEFT},
		{PerPadRightReleased,PerPadRightReleased,VK_RIGHT}
	};
	
	if(pad1==0)
		return 0;
	if(GetKeyboardState(keys)){
		int i;
		for(i=0;i<sizeof(keymap)/sizeof(struct KEYMAP);i++){
			if(is_key_pressed(keys,keymap[i].key))
				keymap[i].pressed(pad1);
			else
				keymap[i].released(pad1);
		}
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