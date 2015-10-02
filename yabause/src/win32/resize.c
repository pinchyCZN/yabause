#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "resource.h"


enum{
	CONTROL_ID,
	XPOS,YPOS,
	WIDTH,HEIGHT,
	HUG_L,
	HUG_R,
	HUG_T,
	HUG_B,
	HUG_CTRL_X,
	HUG_CTRL_Y,
	HUG_HEIGHT,
	HUG_WIDTH,
	HUG_CTRL_TXT_X,
	HUG_CTRL_TXT_Y,
	HUG_CTRL_TXT_X_,
	HUG_CTRL_TXT_Y_,
	SIZE_HEIGHT_OFF,
	SIZE_WIDTH_OFF,
	SIZE_HEIGHT_PER,
	SIZE_WIDTH_PER,
	SIZE_TEXT_CTRL,
	CONTROL_FINISH,
	RESIZE_FINISH
};

int process_anchor_list(HWND hwnd,short *list)
{
	int limit=9999;
	int i=0,j,x,y,width,height;
	HWND dlg_item;
	HDC	 hdc;
	RECT crect;
	SIZE text_size;
	char str[255];
	double f;
	int done=FALSE;
	int last_text=0;

	memset(&crect,0,sizeof(crect));
	hdc=GetDC(hwnd);
	GetClientRect(hwnd, &crect);
	do{
		switch(list[i]){
		case CONTROL_ID:
			x=y=width=height=0;
			dlg_item=GetDlgItem(hwnd,list[i+1]);
			if(dlg_item==NULL)
				done=TRUE;
			break;
		case XPOS:
			x+=list[i+1];
			break;
		case YPOS:
			y+=list[i+1];
			break;
		case WIDTH:
			width+=list[i+1];
			break;
		case HEIGHT:
			height+=list[i+1];
			break;
		case HUG_L:
			x+=crect.left+list[i+1];
			break;
		case HUG_R:
			x+=crect.right+list[i+1];
			break;
		case HUG_T:
			y+=crect.top+list[i+1];
			break;
		case HUG_B:
			y+=crect.bottom+list[i+1];
			break;
		case HUG_CTRL_X:
			break;
		case HUG_CTRL_Y:
			break;
		case HUG_HEIGHT:
			j=crect.bottom-crect.top;
			f=(double)list[i+1]/1000.0;
			y+=j*f;
			break;
		case HUG_WIDTH:
			j=crect.right-crect.left;
			f=(double)list[i+1]/1000.0;
			x+=j*f;
			break;
		case HUG_CTRL_TXT_X:
			if(last_text!=list[i+1]){
				GetDlgItemText(hwnd,list[i+1],str,sizeof(str)-1);
				GetTextExtentPoint32(hdc,str,strlen(str),&text_size);
				last_text=list[i+1];
			}
			x+=text_size.cx;
			break;
		case HUG_CTRL_TXT_X_:
			if(last_text!=list[i+1]){
				GetDlgItemText(hwnd,list[i+1],str,sizeof(str)-1);
				GetTextExtentPoint32(hdc,str,strlen(str),&text_size);
				last_text=list[i+1];
			}
			x-=text_size.cx;
			break;
		case HUG_CTRL_TXT_Y:
			if(last_text!=list[i+1]){
				GetDlgItemText(hwnd,list[i+1],str,sizeof(str)-1);
				GetTextExtentPoint32(hdc,str,strlen(str),&text_size);
				last_text=list[i+1];
			}
			y+=text_size.cy;
			break;
		case HUG_CTRL_TXT_Y_:
			if(last_text!=list[i+1]){
				GetDlgItemText(hwnd,list[i+1],str,sizeof(str)-1);
				GetTextExtentPoint32(hdc,str,strlen(str),&text_size);
				last_text=list[i+1];
			}
			y-=text_size.cy;
			break;
		case SIZE_HEIGHT_OFF:
			height+=crect.bottom-crect.top+list[i+1];
			break;
		case SIZE_WIDTH_OFF:
			width+=crect.right-crect.left+list[i+1];
			break;
		case SIZE_HEIGHT_PER:
			j=crect.bottom-crect.top;
			f=(double)list[i+1]/1000.0;
			height+=f*j;
			break;
		case SIZE_WIDTH_PER:
			j=crect.right-crect.left;
			f=(double)list[i+1]/1000.0;
			width+=f*j;
			break;
		case SIZE_TEXT_CTRL:
			if(last_text!=list[i+1]){
				GetDlgItemText(hwnd,list[i+1],str,sizeof(str)-1);
				GetTextExtentPoint32(hdc,str,strlen(str),&text_size);
				last_text=list[i+1];
			}
			width+=text_size.cx;
			height+=text_size.cy;
			break;
		case CONTROL_FINISH:
			SetWindowPos(dlg_item,NULL,x,y,width,height,SWP_NOZORDER);
			break;
		case RESIZE_FINISH:
			done=TRUE;
			break;
		default:
			printf("bad command %i\n",list[i]);
			break;
		}
		i+=2;
		if(i>limit)
			done=TRUE;
	}while(!done);
	ReleaseDC(hwnd,hdc);
	return TRUE;
}
short win_maindlg[]={
	CONTROL_ID,IDC_STATUS,
		HUG_L,0,
		HUG_B,-20,
		SIZE_WIDTH_OFF,0,
		HEIGHT,20,
		CONTROL_FINISH,-1,
	RESIZE_FINISH
};
int get_idc_name(int idc,char *name,int len)
{
	int found=FALSE;
	FILE *f;
	char str[1024];
	f=fopen("resource.h","rt");
	if(f!=0){
		memset(str,0,sizeof(str));
		while(fgets(str,sizeof(str)-1,f)){
			int id=0;
			char t[256]={0};
			sscanf(str,"#define %s %i",t,&id);
			if(idc==id){
				strncpy(name,t,len);
				found=TRUE;
				break;
			}
		}
		fclose(f);
	}
	return found;
}
int dump_sizes(HWND hwnd,short *IDC)
{
	int i;
	RECT client,win;
	GetClientRect(hwnd,&client);
	for(i=0;i<1000;i++){
		POINT p={0};
		int width,height;
		int cw,ch;
		char name[256];
		if(IDC[i]==RESIZE_FINISH)
			break;
		if(IDC[i]==CONTROL_ID)
			i++;
		else{
			i++;
			continue;
		}
		sprintf(name,"%i",IDC[i]);
		get_idc_name(IDC[i],name,sizeof(name));
		printf("CONTROL_ID,%s,\n",name);
		GetWindowRect(GetDlgItem(hwnd,IDC[i]),&win);
		p.x=win.left;
		p.y=win.top;
		ScreenToClient(hwnd,&p);
		width=win.right-win.left;
		height=win.bottom-win.top;
		cw=client.right-client.left;
		ch=client.bottom-client.top;
		printf("\tXPOS,%i,YPOS,%i, /*x.off=%i y.off=%i*/\n",
			p.x,p.y,
			-client.right+p.x,-client.bottom+p.y);
		printf("\tWIDTH,%i,HEIGHT,%i, /*w.off=%i h.off=%i*/\n\tCONTROL_FINISH,-1,\n",
			width,height,
			-cw+width,-ch+height);

	}
	printf("\n");return 0;
}


int reposition_controls(HWND hwnd, int *list)
{
	process_anchor_list(hwnd,list);
	return TRUE;
}
#define GRIPPIE_SQUARE_SIZE 15
int create_grippy(HWND hwnd,HWND *hgrippy)
{
	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	
	*hgrippy=CreateWindow("Scrollbar",NULL,WS_CHILD|WS_VISIBLE|SBS_SIZEGRIP,
		client_rect.right-GRIPPIE_SQUARE_SIZE,
		client_rect.bottom-GRIPPIE_SQUARE_SIZE,
		GRIPPIE_SQUARE_SIZE,GRIPPIE_SQUARE_SIZE,
		hwnd,NULL,NULL,NULL);
	return *hgrippy?TRUE:FALSE;
}

int move_grippy(HWND hwnd,HWND grippy)
{
	RECT client_rect;
	GetClientRect(hwnd,&client_rect);
	if(grippy!=0)
	{
		SetWindowPos(grippy,NULL,
			client_rect.right-GRIPPIE_SQUARE_SIZE,
			client_rect.bottom-GRIPPIE_SQUARE_SIZE,
			GRIPPIE_SQUARE_SIZE,GRIPPIE_SQUARE_SIZE,
			SWP_NOZORDER|SWP_SHOWWINDOW);
	}
	return 0;
}

int convert_coords_list(HWND hwnd,short *list,int count)
{
	int i;
	for(i=0;i<count;i+=2){
		int token=list[i];
		if(token==RESIZE_FINISH)
			break;
		switch(token){
		case CONTROL_ID:
		case CONTROL_FINISH:
			break;
		case XPOS:
		case WIDTH:
		case HUG_L:
		case HUG_R:
		case HUG_CTRL_X:
		case HUG_CTRL_TXT_X:
		case HUG_CTRL_TXT_X_:
		case SIZE_WIDTH_OFF:
			{
				RECT rect={0};
				rect.right=list[i+1];
				if(MapDialogRect(hwnd,&rect))
					list[i+1]=rect.right;
			}
			break;
		case YPOS:
		case HEIGHT:
		case HUG_T:
		case HUG_B:
		case HUG_CTRL_Y:
		case HUG_CTRL_TXT_Y:
		case HUG_CTRL_TXT_Y_:
		case SIZE_HEIGHT_OFF:
			{
				RECT rect={0};
				rect.bottom=list[i+1];
				if(MapDialogRect(hwnd,&rect))
					list[i+1]=rect.bottom;
			}
		break;
		}

	}
}
int convert_maindlg_coords(HWND hwnd)
{
	static int done=FALSE;
	if(!done){
		convert_coords_list(hwnd,win_maindlg,sizeof(win_maindlg)/sizeof(short));
		done=TRUE;
	}
	return 0;
}
int resize_maindlg(HWND hwnd)
{
	convert_maindlg_coords(hwnd);
	return reposition_controls(hwnd,win_maindlg);
}
