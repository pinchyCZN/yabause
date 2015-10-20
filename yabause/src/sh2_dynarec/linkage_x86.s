;;/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
;*   Yabause - linkage_x86.s                                               *
;*   Copyright (C) 2009-2011 Ari64                                         *
;*                                                                         *
;*   This program is free software; you can redistribute it and/or modify  *
;*   it under the terms of the GNU General Public License as published by  *
;*   the Free Software Foundation; either version 2 of the License, or     *
;*   (at your option) any later version.                                   *
;*                                                                         *
;*   This program is distributed in the hope that it will be useful,       *
;*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
;*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
;*   GNU General Public License for more details.                          *
;*                                                                         *
;*   You should have received a copy of the GNU General Public License     *
;*   along with this program; if not, write to the                         *
;*   Free Software Foundation, Inc.,                                       *
;*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
;* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
; 	.file	"linkage_x86.s"

extern _master_ip,_master_cc,_master_pc,_slave_ip,_slave_cc,_slave_pc
extern _decilinestop_p,_decilineusec_p,_linecount_p,_maxlinecount_p,_vblanklinecount_p
extern _saved_centicycles,_rccount,_invalidate_count,_add_link,_hash_table,_jump_dirty
extern _sh2_recompile_block
extern _restore_candidate,_clean_blocks,_jump_in
extern _get_addr,_cached_code,_invalidate_addr
extern _yabsys_timing_bits,_yabsys_timing_mask
extern _SH2CycleFrac_p,_UsecFrac_p
extern _MSH2,_SSH2
extern _MappedMemoryWriteLong,_MappedMemoryWriteWord,_MappedMemoryWriteByte
extern _MappedMemoryReadLong,_MappedMemoryReadWord
extern _NumberOfInterruptsOffset,_DynarecMasterHandleInterrupts,_DynarecSlaveHandleInterrupts
extern _CurrentSH2,_SmpcINTBACKEnd
extern _FRTExec,_WDTExec,_ScuExec,_ScspExec,_SmpcExec,_Cs2Exec,_M68KExec
extern _M68KSync
extern _Vdp2HBlankIN,_Vdp2HBlankOUT,_Vdp2VBlankIN,_Vdp2VBlankOUT
extern _CheatDoPatches
extern _BiosHandleFunc

section	.text

global _YabauseDynarecOneFrameExec
_YabauseDynarecOneFrameExec:
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
	and	edi,ebx,
	mov	[esi],edi ;/* SH2CycleFrac */
	shr	ebx,cl,
	mov	[ebp-28],ebx ;/* scucycles */
	add	ebx, ebx ;/* sh2cycles */
	mov	eax,_MSH2
	mov	ecx,_NumberOfInterruptsOffset
	sub	ebx,edx  ;/* sh2cycles(full line) - decilinecycles*9 */
	mov	[_CurrentSH2],eax
	mov	[ebp-24],ebx ;/* sh2cycles */
	cmp	dword[eax+ecx],0
	jne	_master_handle_interrupts
	mov	esi,_master_cc
	sub	esi,ebx
	ret	;/* jmp _master_ip */
	;.size	YabauseDynarecOneFrameExec, .-YabauseDynarecOneFrameExec

global _master_handle_interrupts
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

global _slave_entry
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
	cmp	dword[eax+ecx],0
	jne	_slave_handle_interrupts
	mov	esi,_slave_cc
	sub	esi,ebx
	jmp	[edx] ;/* jmp *_slave_ip */
	;.size	slave_entry, .-slave_entry

global _slave_handle_interrupts
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
global _cc_interrupt_master
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

global _jump_vaddr_eax_master
_jump_vaddr_eax_master:
	mov	edi,eax
	jmp	_jump_vaddr_edi_master
	;.size	jump_vaddr_eax_master, .-jump_vaddr_eax_master
global _jump_vaddr_ecx_master
_jump_vaddr_ecx_master:
	mov	edi,ecx
	jmp	_jump_vaddr_edi_master
	;.size	jump_vaddr_ecx_master, .-jump_vaddr_ecx_master
global _jump_vaddr_edx_master
_jump_vaddr_edx_master:
	mov	edi,edx
	jmp	_jump_vaddr_edi_master
	;.size	jump_vaddr_edx_master, .-jump_vaddr_edx_master
global _jump_vaddr_ebx_master
_jump_vaddr_ebx_master:
	mov	edi,ebx
	jmp	_jump_vaddr_edi_master
	;.size	jump_vaddr_ebx_master, .-jump_vaddr_ebx_master
global _jump_vaddr_ebp_master
_jump_vaddr_ebp_master:
	mov	edi,ebp
	jmp	_jump_vaddr_edi_master
	;.size	jump_vaddr_ebp_master, .-jump_vaddr_ebp_master
global _jump_vaddr_eax_slave
_jump_vaddr_eax_slave:
	mov	edi,eax
	jmp	_jump_vaddr_edi_slave
	;.size	jump_vaddr_eax_slave, .-jump_vaddr_eax_slave
global _jump_vaddr_ecx_slave
_jump_vaddr_ecx_slave:
	mov	edi,ecx
	jmp	_jump_vaddr_edi_slave
	;.size	jump_vaddr_ecx_slave, .-jump_vaddr_ecx_slave
global _jump_vaddr_edx_slave
_jump_vaddr_edx_slave:
	mov	edi,edx
	jmp	_jump_vaddr_edi_slave
	;.size	jump_vaddr_edx_slave, .-jump_vaddr_edx_slave
global _jump_vaddr_ebx_slave
_jump_vaddr_ebx_slave:
	mov	edi,ebx
	jmp	_jump_vaddr_edi_slave
	;.size	jump_vaddr_ebx_slave, .-jump_vaddr_ebx_slave
global _jump_vaddr_ebp_slave
_jump_vaddr_ebp_slave:
	mov	edi,ebp
	;.size	jump_vaddr_ebp_slave, .-jump_vaddr_ebp_slave
global _jump_vaddr_edi_slave
_jump_vaddr_edi_slave:
	or	edi,1
	;.size	jump_vaddr_edi_slave, .-jump_vaddr_edi_slave
global _jump_vaddr_edi_master
_jump_vaddr_edi_master:
	mov	eax,edi
	;.size	_jump_vaddr_edi_master, .-_jump_vaddr_edi_master

global _jump_vaddr
_jump_vaddr:
  ;/* Check hash table */
	shr	eax,0x16
	xor	eax,edi
	movzx	eax,ax
	shl	eax,4
	cmp	[_hash_table+eax],edi
	jne	.C2
.C1:
	mov	edi,[_hash_table+4+eax]
	jmp	[edi]
.C2:
	cmp	[_hash_table+8+eax],edi
	lea	eax,[8+eax]
	je	.C1
  ;/* No hit on hash table, call compiler */
	push	edi
	call	_get_addr
	add	esp,4
	jmp	[eax]
	;.size	jump_vaddr, .-jump_vaddr

global _verify_code
_verify_code:
	;/* eax = source */
	;/* ebx = target */
	;/* ecx = length */
	mov	edi,[-4+eax+ecx]
	xor	edi,[-4+ebx+ecx]
	jne	.D5
	mov	edx,ecx
	add	ecx,-4
	je	.D3
	test	edx,4
	cmove	ecx,edx
	push	esi
.D2:
	mov	edx,[-4+eax+ecx]
	mov	ebp,[-4+ebx+ecx]
	mov	esi,[-8+eax+ecx]
	xor	ebp,edx
	mov	edi,[-8+ebx+ecx]
	jne	.D4
	xor	edi,esi
	jne	.D4
	add	ecx,-8
	jne	.D2
	pop	esi
.D3:
	ret
.D4:
	pop	esi
.D5:
	add	esp,4 ;/* pop return address, we're not returning */
	call	_get_addr
	add	esp,4 ;/* pop virtual address */
	jmp	[eax]
	;.size	verify_code, .-verify_code

global _WriteInvalidateLong
_WriteInvalidateLong:
	mov	ecx,eax
	shr	ecx,0x12
	bt	[_cached_code],ecx
	jnc	_MappedMemoryWriteLong
	push	eax
	push	edx
	push	eax
	call	_invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	jmp	_MappedMemoryWriteLong
	;.size	WriteInvalidateLong, .-WriteInvalidateLong
global _WriteInvalidateWord
_WriteInvalidateWord:
	mov	ecx,eax
	shr	ecx,0x12
	bt	[_cached_code],ecx
	jnc	_MappedMemoryWriteWord
	push	eax
	push	edx
	push	eax
	call	_invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	jmp	_MappedMemoryWriteWord
	;.size	WriteInvalidateWord, .-WriteInvalidateWord
global _WriteInvalidateByteSwapped
_WriteInvalidateByteSwapped:
	xor	eax,1
	;.size	WriteInvalidateByteSwapped, .-WriteInvalidateByteSwapped
global _WriteInvalidateByte
_WriteInvalidateByte:
	mov	ecx,eax
	shr	ecx,0x12
	bt	[_cached_code],ecx
	jnc	_MappedMemoryWriteByte
	push	eax
	push	edx
	push	eax
	call	_invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	jmp	_MappedMemoryWriteByte
	;.size	WriteInvalidateByte, .-WriteInvalidateByte

global _div1
_div1:
	;/* eax = dividend */
	;/* ecx = divisor */
	;/* edx = sr */
	bt	edx,9   ;/* M bit */
	jc	div1_negative_divisor
	bts	edx,0   ;/* Get T bit and set */
	adc eax,eax ;/* rn=(rn<<1)+T */
	adc	ebx,ebx ;/* New Q in ebx */
	mov	ebp,ecx
	btr	edx,8   ;/* Get Q bit and clear it */
	cmc
	sbb	edi, edi ;/* 0xFFFFFFFF if old_Q clear, 0 otherwise */
	sbb	ebp,0
	xor	ebp,edi
	add	eax,ebp ;/* rn+rm if old_Q, rn-rm if !old_Q */
		           ;/* carry set if rn < old_rn */
	adc	ebx,edi ;/* low bit = (rn<old_rn)^new_Q^!old_Q */
	                   ;/* inverted for old_Q==0, ie (rn>=old_rn)^new_Q */
	not	edi	   ;/* if old_Q clear, edi=0 */
	or	edi,ebp ;/* zero if old_Q==0 && rn==old_rn */
	neg	edi       ;/* clear carry if edi==0 */
	adc	ebx,-1  ;/* invert result for old_Q==0 && rn==old_rn */
	and	ebx,1
	xor	edx,ebx ;/* New T = (Q==M) */
	shl	ebx,8
	or	edx,ebx ;/* save new Q */
;/*
;	push	edx
;	push	eax
;	push	ecx
;	call	debug_division
;	pop	ecx
;	pop	eax
;	pop	edx
;*/
	ret
div1_negative_divisor:
	btr	edx,0   ;/* Get T bit and clear */
	adc eax, eax ;/* rn=(rn<<1)+T */
	adc	ebx, ebx ;/* New Q in ebx */
	mov	ebp,ecx
	btr	edx,8   ;/* Get Q bit and clear it */
	sbb	edi, edi ;/* 0xFFFFFFFF if old_Q set, 0 otherwise */
	sbb	ebp,0
	xor	ebp,edi
	not	edi	   ;/* if old_Q clear, edi=-1 */
	add	eax,ebp ;/* rn+rm if !old_Q, rn-rm if old_Q */
		           ;/* carry set if rn < old_rn */
	adc	ebx,edi ;/* low bit = (rn<old_rn)^new_Q^!old_Q */
	                   ;/* inverted for old_Q==0, ie (rn>=old_rn)^new_Q */
	or	edi,ebp ;/* zero if old_Q==1 && rn==old_rn */
	neg	edi       ;/* clear carry if edi==0 */
	adc	ebx,-1  ;/* invert result for old_Q==1 && rn==old_rn */
	and	ebx,1
	xor	edx,ebx ;/* New T = (Q==M) */
	shl	ebx,8
	or	edx,ebx ;/* save new Q */
	ret
	;.size	div1, .-div1

global _macl
_macl:
	;/* ebx = sr */
	;/* ebp = multiplicand address */
	;/* edi = multiplicand address */
	;/* eax = return MACL */
	;/* edx = return MACH */
	push	edx ;/* MACH */
	push	eax ;/* MACL */
	mov	eax,edi
	call	_MappedMemoryReadLong
	mov	esi,eax
	mov	eax,ebp
	call	_MappedMemoryReadLong
	add	ebp,4
	add	edi,4
	imul	esi
	add	eax,[esp] ;/* MACL */
	adc	edx,[4+esp] ;/* MACH */
	add	esp,8
	test bl,0x2
	jne	macl_saturation
	ret
macl_saturation:

	mov	esi,0xFFFF8000
	xor	ecx,ecx
	cmp	edx,esi
	cmovl	edx,esi
	cmovl	eax,ecx
	not	esi
	not	ecx
	cmp	esi, edx
	cmovg	edx,esi
	cmovg	eax,ecx
	ret
	;.size	macl, .-macl

global _macw
_macw:
	;/* ebx = sr */
	;/* ebp = multiplicand address */
	;/* edi = multiplicand address */
	;/* eax = return MACL */
	;/* edx = return MACH */
	push	edx ;/* MACH */
	push	eax ;/* MACL */
	mov	eax,edi
	call	_MappedMemoryReadWord
	movsx	esi,ax
	mov		eax,ebp
	call	_MappedMemoryReadWord
	movsx	eax,ax
	add		ebp,2
	add		edi,2
	imul	esi
	test	bl,0x2
	jne	macw_saturation
	add	eax,[esp] ;/* MACL */
	adc	edx,[4+esp] ;/* MACH */
	add	esp,8
	ret
macw_saturation:
	mov	esi,[esp]
	sar	esi,0x31
	add	eax,[esp] ;/* MACL */
	adc	edx,esi
	mov	esi,0x80000000
	mov	ecx,0x7FFFFFFF
	add	esi,eax
	adc	edx,0
	cmovne	eax,ecx
	not	ecx
	cmovl	eax,ecx
	pop	edx
	pop	edx
	ret
	;.size	macw, .-macw

global _master_handle_bios
_master_handle_bios:
	mov	edx,[esp] ;/* get return address */
	mov	[_master_pc],eax
	mov	[_master_cc],esi
	mov	[_master_ip],edx
	mov	eax,_MSH2
	call	_BiosHandleFunc
	mov	edx,_master_ip
	mov	esi,_master_cc
	mov	[esp],edx
	ret	;/* jmp *_master_ip */
	;.size	master_handle_bios, .-master_handle_bios

global _slave_handle_bios
_slave_handle_bios:
	pop	edx ;/* get return address */
	mov	[_slave_pc],eax
	mov	[_slave_cc],esi
	mov	[_slave_ip],edx
	mov	eax,[_SSH2]
	call	_BiosHandleFunc
	mov	edx,_slave_ip
	mov	esi,_slave_cc
	jmp	[edx] ;/* jmp *_slave_ip */
	;.size	slave_handle_bios, .-slave_handle_bios

global _breakpoint
_breakpoint:
	ret
	;/* Set breakpoint here for debugging */
	;.size	breakpoint, .-breakpoint
