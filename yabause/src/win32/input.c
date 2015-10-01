#include "..\peripheral.h"

#define PERCORE_WIN32 32

int PERWin32Init(void)
{
	return 0;
}
void PERWin32DeInit(void)
{
}
int PERWin32HandleEvents(void)
{
	return 0;
}
u32 PERWin32Scan(u32 flags)
{
int i;
i++;
return i;
}
void PERWin32Flush(void)
{
	int i;
	i++;
	return i;
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
