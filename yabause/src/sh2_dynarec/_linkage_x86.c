
#if 0
extern int master_ip;
int __declspec(naked)  __YabauseDynarecOneFrameExec()
{
	_asm{

	push	ebp
	mov		ebp,esp
	mov		eax,_master_ip
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

newline:
;/* const u32 decilinecycles = yabsys.DecilineStop >> _yabsys_timing_bits; */
;/* const u32 cyclesinc = yabsys.DecilineStop * 10; */
	mov	eax,_decilinestop_p
	mov	ecx,_yabsys_timing_bits
	mov	eax,[eax]
	lea	ebx,[eax+eax*4] ;/* decilinestop*5 */
	shr	eax,cl	;/* decilinecycles */
	shl	ebx,1	;/* cyclesinc=decilinestop*10 */
	lea	edx,[eax+eax*8]  ;/* decilinecycles*9 */
        ;/* yabsys.SH2CycleFrac += cyclesinc;*/
        ;/* sh2cycles = (yabsys.SH2CycleFrac >> (_yabsys_timing_bits + 1)) << 1;*/
        ;/* yabsys.SH2CycleFrac &= ((_yabsys_timing_mask << 1) | 1);*/
	mov	esi,_SH2CycleFrac_p
	mov	edi,_yabsys_timing_mask
	inc	ecx ;/* _yabsys_timing_bits+1 */
	add	ebx,[esi] ;/* SH2CycleFrac */
	stc
	adc	edi, edi ;/* ((_yabsys_timing_mask << 1) | 1) */
	mov	[ebp-20],eax ;/* decilinecycles */
	and	edi,ebx
	mov	[esi],edi ;/* SH2CycleFrac */
	shr	ebx,cl
	mov	[ebp-28],ebx ;/* scucycles */
	add	ebx, ebx ;/* sh2cycles */
	mov	eax,_MSH2
	mov	ecx,_NumberOfInterruptsOffset
	sub	ebx,edx  ;/* sh2cycles(full line) - decilinecycles*9 */
	mov	[_CurrentSH2],eax
	mov	[ebp-24],ebx ;/* sh2cycles */
	cmp	[eax+ecx],0
	jne	_master_handle_interrupts
	mov	esi,_master_cc
	sub	esi,ebx
	ret	;/* jmp _master_ip */
	;.size	YabauseDynarecOneFrameExec, .-YabauseDynarecOneFrameExec

;global _master_handle_interrupts
_master_handle_interrupts:
	mov	eax,[ebp-40] ;/* get return address */
	mov	[_master_ip],eax
	call	_DynarecMasterHandleInterrupts
	mov	eax,_master_ip
	mov	esi,_master_cc
	mov	[ebp-40],eax ;/* overwrite return address */
	sub	esi,ebx
	ret	;/* jmp _master_ip */
	;.size	_master_handle_interrupts, .-_master_handle_interrupts

;global _slave_entry
_slave_entry:
	mov	ebx,[16+esp] ;/* sh2cycles */
	mov	[_master_cc],esi
	sub	esp,0x12
	push	ebx
	call	_FRTExec
	mov	[esp],ebx
	call	_WDTExec
	mov	edx,_slave_ip
	add	esp,0x16
	test	edx, edx
	je	_cc_interrupt_master ;/* slave not running */
	mov	eax,_SSH2
	mov	ecx,_NumberOfInterruptsOffset
	mov	[_CurrentSH2],eax
	cmp	[eax+ecx],0
	jne	_slave_handle_interrupts
	mov	esi,_slave_cc
	sub	esi,ebx
	jmp	[edx] ;/* jmp *_slave_ip */

;global _slave_handle_interrupts
_slave_handle_interrupts:
	call	_DynarecSlaveHandleInterrupts
	mov	edx,_slave_ip
	mov	esi,_slave_cc
	sub	esi,ebx
	jmp	[edx] ;/* jmp *_slave_ip */
	;.size	_slave_handle_interrupts, .-_slave_handle_interrupts

global _cc_interrupt
_cc_interrupt: ;/* slave */
	mov	ebx,[16+esp] ;/* sh2cycles */
	mov	[_slave_ip],ebp
	mov	[_slave_cc],esi
	add	esp,-0x12
	push	ebx
	call	_FRTExec
	mov	[esp],ebx
	call	_WDTExec
	add	esp,0x16
	;.size	cc_interrupt, .-cc_interrupt
;global _cc_interrupt_master
_cc_interrupt_master:
	lea	ebp,[40+esp]
	mov	eax,[-16+ebp] ;/* decilinecount */
	mov	ebx,[-20+ebp] ;/* decilinecycles */
	inc	eax
	cmp	dword eax,0x9
	ja	.A3
	mov	[-16+ebp],eax ;/* decilinecount++ */
	je	.A2
	mov	[-24+ebp],ebx ;/* sh2cycles */
.A1:
	mov	esi,_master_cc
	mov	eax,_MSH2
	mov	ecx,_NumberOfInterruptsOffset
	mov	[_CurrentSH2],eax
	cmp	dword[eax+ecx],0
	jne	_master_handle_interrupts
	sub	esi,ebx
	ret	;/* jmp _master_ip */	
.A2:
	call	_Vdp2HBlankIN
	jmp	.A1
.A3:
	mov	ebx,[-28+ebp] ;/* scucycles */
	add	esp,-0x12
	push	ebx
	call	_ScuExec
	call	_M68KSync
	call	_Vdp2HBlankOUT
	call	_ScspExec
	mov	ebx,[_linecount_p]
	mov	eax,[_maxlinecount_p]
	mov	ecx,[_vblanklinecount_p]
	mov	edx,[ebx]
	mov	eax,[eax]
	mov	ecx,[ecx]
	inc	edx
	and dword[ebp-16],0 ;/* decilinecount=0 */
	cmp	eax, edx ;/* max ? */
	je	nextframe
	mov	[ebx],edx ;/* linecount++ */
	cmp	ecx, edx ;/* vblank ? */
	je	vblankin
nextline:
	add	esp,0x16
	call	finishline
	jmp	newline
finishline: ;/* CHECK - Stack align? */
      ;/*const u32 usecinc = yabsys.DecilineUsec * 10;*/
	mov	eax,[_decilineusec_p]
	mov	ebx,[_UsecFrac_p]
	mov	ecx,[_yabsys_timing_bits]
	mov	eax,[eax]
	mov	edx,[ebx]
	lea	esi,[eax+eax*4]
	mov	edi,_yabsys_timing_mask
	add	esi, esi
      ;/*yabsys.UsecFrac += usecinc;*/
	add	esi,edx
	add	esp,-8 ;/* Align stack */
      ;/*_SmpcExec(yabsys.UsecFrac >> _yabsys_timing_bits);
      ;/*_Cs2Exec(yabsys.UsecFrac >> _yabsys_timing_bits);
      ;/*yabsys.UsecFrac &= _yabsys_timing_mask;*/
	mov	[ebx],esi ;/* UsecFrac */
	shr	esi,cl
	push	esi
	call	_SmpcExec
	;/* _SmpcExec may modify UsecFrac; must reload it */
	mov	esi,[ebx] ;/* UsecFrac */
	mov	ecx,_yabsys_timing_bits
	and	edi,esi
	shr	esi,cl
	mov	[esp],esi
	call	_Cs2Exec
	mov	[ebx],edi ;/* UsecFrac */
	mov	ecx,[_saved_centicycles]
	mov	ebx,[12+ebp] ;/* m68kcenticycles */
	mov	eax,[8+ebp] ;/* m68kcycles */
	add	ecx,ebx
	mov	ebx,ecx
	add	ecx,-0x100
	cmovnc	ecx,ebx
	adc	eax,0
	mov	[_saved_centicycles],ecx
	mov	[esp],eax ;/* cycles */
	call	_M68KExec
	add	esp,0x12
	ret
vblankin:
	call	_SmpcINTBACKEnd
	call	_Vdp2VBlankIN
	call	_CheatDoPatches
	jmp	nextline
nextframe:
	call	_Vdp2VBlankOUT
	and		dword[ebx],0 ;/* linecount = 0 */
	call	finishline
	call	_M68KSync
	mov	esi,[_rccount]
	inc	esi
	and	dword[_invalidate_count],0
	and	esi,0x3f
	cmp	dword[_restore_candidate+esi*4],0
	mov	[_rccount],esi
	jne	.A5
.A4:
	mov	eax,[16+esp]
	add	esp,0x44
	mov	[_master_ip],eax
	pop	ebx
	pop	esi
	pop	edi
	pop	ebp
	ret
.A5:
	;/* Move 'dirty' blocks to the 'clean' list */
	mov	ebx,[_restore_candidate+esi*4] 
	mov	ebp,esi
	and dword[_restore_candidate+esi*4],0
	shl	ebp,5
.A6:
	shr	ebx,1
	jnc	.A7
	mov	[esp],ebp
	call	_clean_blocks
.A7:
	inc	ebp
	test ebp,0x31
	jne	.A6
	jmp	.A4
	;.size	_cc_interrupt_master, .-_cc_interrupt_master

global _dyna_linker
_dyna_linker:
	;/* eax = virtual target address */
	;/* ebx = instruction to patch */
	mov	ecx,eax
	mov	edx,0x1023
	shr	ecx,0x12
	and	edx,ecx
	and	ecx,0xDFFFF
	or	edx,0x1024
	cmp	edx, ecx
	cmova	ecx,edx
	;/* _jump_in lookup */
	mov	edx,[_jump_in+ecx*4]
.B1:
	test	edx, edx
	je	.B3
	mov	edi,[edx]
	xor	edi,eax
	je	.B2
	mov	edx,[12+edx]
	jmp	.B1
.B2:
	mov	edi,[ebx]
	mov	ebp,esi
	lea	esi,[4+ebx+edi]
	mov	edi,eax
	pusha
	call	_add_link
	popa
	mov	edi,[8+edx]
	mov	esi,ebp
	lea	edx,[-4+edi]
	sub	edx,ebx
	mov	[ebx],edx
	jmp	[edi]
.B3:
	;/* _hash_table lookup */
	mov	edi,eax
	shr	edi,0x16
	xor	edi,eax
	movzx	edi,di
	shl	edi,4
	cmp	[_hash_table+edi], eax
	jne	.B5
.B4:
	mov	[_hash_table+4+edi], edx
	jmp	[edx]
.B5:
	cmp	[_hash_table+8+edi],eax
	lea	edi,[8+edi]
	je	.B4
	;/* _jump_dirty lookup */
	mov	edx,[_jump_dirty+ecx*4]
.B6:
	test	edx, edx
	je	.B8
	mov	ecx,[edx]
	xor	ecx,eax
	je	.B7
	mov	edx,[12+edx]
	jmp	.B6
.B7:
	mov	edx,[8+edx]
	;/* _hash_table insert */
	mov	ebx,[_hash_table-8+edi]
	mov	ecx,[_hash_table-4+edi]
	mov	[_hash_table-8+edi],eax
	mov	[_hash_table-4+edi],edx
	mov	[_hash_table+edi],ebx
	mov	[_hash_table+4+edi],ecx
	jmp	[edx]
.B8:
	mov	edi,eax
	pusha
	call	_sh2_recompile_block
	test	eax, eax
	popa
	je	_dyna_linker
	;/* shouldn't happen */
	int3
	;.size	_dyna_linker, .-_dyna_linker


	}

}

#endif