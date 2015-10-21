
extern int master_ip;
int _cdecl __YabauseDynarecOneFrameExec()
{
	_asm{
	push	ebp
	mov		ebp,esp
	mov		eax,master_ip
	xor		ecx, ecx
	push	edi
	push	esi
	push	ebx
	push	ecx ;/* zero */
	push	ecx
	push	ecx
	push	ecx
	push	ecx ;/* put m68k here (?) */
	push	ecx
	call	$+5 ;/* 40+4=44 */
	mov		[ebp-40],eax ;-40(ebp) ;/* overwrite return address */
;/* Stack frame:
   ; arg2 - m68kcenticycles (+8/+12)
   ; arg1 - m68kcycles (+4/+8)
   ; return address (0)
   ; ebp (4/0)
   ; save edi (8/4)
   ; save esi (12/8)
   ; save ebx (16/12)
   ; decilinecount (20/16)
   ; decilinecycles (24/20)
   ; sh2cycles (28/24)
   ; scucycles (32/28)
   ; ... (36/32)
   ; ... (40/36)
   ; ret address/_master_ip (44/40) (alternate esp at call)
   ; save eax (48/44)
   ; save ecx (52/48)
   ; save edx (56/52)
   ; ... (esp at call)
   ; next return address (64/60)
   ; total = 64 */
;/*   usecinc?
;   cyclesinc?*/
	}

}