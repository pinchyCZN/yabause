void Vdp2VBlankOUT(void);
void CheatDoPatches(void);
void Vdp2VBlankIN(void);
void SmpcINTBACKEnd(void);
void M68KExec(s32 cycles);
extern int saved_centicycles;
void Cs2Exec(u32);
void SmpcExec(s32 t);
void ScspExec(void);
void Vdp2HBlankOUT(void);
void M68KSync(void);
void ScuExec(u32 timing);
void Vdp2HBlankIN(void);
void WDTExec(u32 cycles);
void FRTExec(u32 cycles);

int FASTCALL BiosHandleFunc(SH2_struct * sh);
u16 FASTCALL MappedMemoryReadWord(u32 addr);

void master_handle_interrupts();
void slave_handle_interrupts();
void cc_interrupt_master();
void newline();
void jump_vaddr();
/*
NOTE: __fastcall for MSVC ecx=arg1 edx=arg2
*/
void __declspec(naked)  YabauseDynarecOneFrameExec(int a,int b)
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
	jmp newline
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
   ; ret address/master_ip (44/40) (alternate esp at call)
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
void __declspec(naked) newline()
{
	_asm{
;/* const u32 decilinecycles = yabsys.DecilineStop >> yabsys_timing_bits; */
;/* const u32 cyclesinc = yabsys.DecilineStop * 10; */
;	cmp dword ptr[master_pc],0x06050b16
;	jne	skip
;	nop
;skip:
	mov	eax,[decilinestop_p]
	mov	ecx,[yabsys_timing_bits]
	mov	eax,[eax]
	lea	ebx,[eax+eax*4] ;/* decilinestop*5 */
	shr	eax,cl	;/* decilinecycles */
	shl	ebx,1	;/* cyclesinc=decilinestop*10 */
	lea	edx,[eax+eax*8]  ;/* decilinecycles*9 */
        ;/* yabsys.SH2CycleFrac += cyclesinc;*/
        ;/* sh2cycles = (yabsys.SH2CycleFrac >> (yabsys_timing_bits + 1)) << 1;*/
        ;/* yabsys.SH2CycleFrac &= ((yabsys_timing_mask << 1) | 1);*/
	mov	esi,[SH2CycleFrac_p]
	mov	edi,[yabsys_timing_mask]
	inc	ecx ;/* yabsys_timing_bits+1 */
	add	ebx,[esi] ;/* SH2CycleFrac */
	stc
	adc	edi, edi ;/* ((yabsys_timing_mask << 1) | 1) */
	mov	[ebp-20],eax ;/* decilinecycles */
	and	edi,ebx
	mov	[esi],edi ;/* SH2CycleFrac */
	shr	ebx,cl
	mov	[ebp-28],ebx ;/* scucycles */
	add	ebx, ebx ;/* sh2cycles */
	mov	eax,[MSH2]
	mov	ecx,[NumberOfInterruptsOffset]
	sub	ebx,edx  ;/* sh2cycles(full line) - decilinecycles*9 */
	mov	[CurrentSH2],eax
	mov	[ebp-24],ebx ;/* sh2cycles */
	cmp	dword ptr[eax+ecx],0
	jne	master_handle_interrupts
	mov	esi,master_cc
	sub	esi,ebx
	ret	;/* jmp master_ip */
	}
}
void __declspec(naked) master_handle_interrupts()
{
	_asm{
	mov	eax,[ebp-40] ;/* get return address */
	mov	[master_ip],eax
	call	DynarecMasterHandleInterrupts
	mov	eax,master_ip
	mov	esi,master_cc
	mov	[ebp-40],eax ;/* overwrite return address */
	sub	esi,ebx
	ret	;/* jmp master_ip */
	}
}
void __declspec(naked) slave_entry()
{
	_asm{
	mov	ebx,[16+esp] ;/* sh2cycles */
	mov	[master_cc],esi
	sub	esp,12
	push	ebx
	call	FRTExec
	mov	[esp],ebx
	call	WDTExec
	mov	edx,slave_ip
	add	esp,16
	test	edx, edx
	je	cc_interrupt_master ;/* slave not running */
	mov	eax,SSH2
	mov	ecx,NumberOfInterruptsOffset
	mov	[CurrentSH2],eax
	cmp	dword ptr[eax+ecx],0
	jne	slave_handle_interrupts
	mov	esi,slave_cc
	sub	esi,ebx
	jmp	edx ;/* jmp *slave_ip */
	}
}
void __declspec(naked) slave_handle_interrupts()
{
	_asm{
	call	DynarecSlaveHandleInterrupts
	mov	edx,slave_ip
	mov	esi,slave_cc
	sub	esi,ebx
	jmp	edx ;/* jmp *slave_ip */
	}
}
void __declspec(naked) cc_interrupt()
{
	_asm{
	;/* slave */
	mov	ebx,[16+esp] ;/* sh2cycles */
	mov	[slave_ip],ebp
	mov	[slave_cc],esi
	add	esp,-12
	push	ebx
	call	FRTExec
	mov	[esp],ebx
	call	WDTExec
	add	esp,16
	jmp cc_interrupt_master
	}
}
void __declspec(naked) cc_interrupt_master()
{
	_asm{
	lea	ebp,[40+esp]
	mov	eax,[-16+ebp] ;/* decilinecount */
	mov	ebx,[-20+ebp] ;/* decilinecycles */
	inc	eax
	cmp	eax,9
	ja	L_A3
	mov	[-16+ebp],eax ;/* decilinecount++ */
	je	L_A2
	mov	[-24+ebp],ebx ;/* sh2cycles */
L_A1:
	mov	esi,master_cc
	mov	eax,MSH2
	mov	ecx,NumberOfInterruptsOffset
	mov	[CurrentSH2],eax
	cmp	dword ptr[eax+ecx],0
	jne	master_handle_interrupts
	sub	esi,ebx
	ret	;/* jmp master_ip */	
L_A2:
	call	Vdp2HBlankIN
	jmp	L_A1
L_A3:
	mov	ebx,[-28+ebp] ;/* scucycles */
	add	esp,-12
	push	ebx
	call	ScuExec
	call	M68KSync
	call	Vdp2HBlankOUT
	call	ScspExec
	mov	ebx,[linecount_p]
	mov	eax,[maxlinecount_p]
	mov	ecx,[vblanklinecount_p]
	mov	edx,[ebx]
	mov	eax,[eax]
	mov	ecx,[ecx]
	inc	edx
	and dword ptr[ebp-16],0 ;/* decilinecount=0 */
	cmp	edx,eax  ;/* max ? */
	je	nextframe
	mov	[ebx],edx ;/* linecount++ */
	cmp	edx,ecx   ;/* vblank ? */
	je	vblankin
nextline:
	add	esp,16
	call	finishline
	jmp	newline
finishline: ;/* CHECK - Stack align? */
      ;/*const u32 usecinc = yabsys.DecilineUsec * 10;*/
	mov	eax,[decilineusec_p]
	mov	ebx,[UsecFrac_p]
	mov	ecx,[yabsys_timing_bits]
	mov	eax,[eax]
	mov	edx,[ebx]
	lea	esi,[eax+eax*4]
	mov	edi,yabsys_timing_mask
	add	esi, esi
      ;/*yabsys.UsecFrac += usecinc;*/
	add	esi,edx
	add	esp,-8 ;/* Align stack */
      ;/*SmpcExec(yabsys.UsecFrac >> yabsys_timing_bits);
      ;/*Cs2Exec(yabsys.UsecFrac >> yabsys_timing_bits);
      ;/*yabsys.UsecFrac &= yabsys_timing_mask;*/
	mov	[ebx],esi ;/* UsecFrac */
	shr	esi,cl
	push	esi
	call	SmpcExec
	;/* SmpcExec may modify UsecFrac; must reload it */
	mov	esi,[ebx] ;/* UsecFrac */
	mov	ecx,yabsys_timing_bits
	and	edi,esi
	shr	esi,cl
	mov	[esp],esi
	call	Cs2Exec
	mov	[ebx],edi ;/* UsecFrac */
	mov	ecx,[saved_centicycles]
	mov	ebx,[12+ebp] ;/* m68kcenticycles */
	mov	eax,[8+ebp] ;/* m68kcycles */
	add	ecx,ebx
	mov	ebx,ecx
	add	ecx,-100
	cmovnc	ecx,ebx
	adc	eax,0
	mov	[saved_centicycles],ecx
	mov	[esp],eax ;/* cycles */
	call	M68KExec
	add	esp,12
	ret
vblankin:
	call	SmpcINTBACKEnd
	call	Vdp2VBlankIN
	call	CheatDoPatches
	jmp	nextline
nextframe:
	call	Vdp2VBlankOUT
	and		dword ptr[ebx],0 ;/* linecount = 0 */
	call	finishline
	call	M68KSync
	mov	esi,[rccount]
	inc	esi
	and	dword ptr[invalidate_count],0
	and	esi,0x3f
	cmp	dword ptr[restore_candidate+esi*4],0
	mov	[rccount],esi
	jne	L_A5
L_A4:
	mov	eax,[16+esp]
	add	esp,44
	mov	[master_ip],eax
	pop	ebx
	pop	esi
	pop	edi
	pop	ebp
	ret
L_A5:
	;/* Move 'dirty' blocks to the 'clean' list */
	mov	ebx,dword ptr[restore_candidate+esi*4] 
	mov	ebp,esi
	and dword ptr[restore_candidate+esi*4],0
	shl	ebp,5
L_A6:
	shr	ebx,1
	jnc	L_A7
	mov	[esp],ebp
	call	clean_blocks
L_A7:
	inc	ebp
	test ebp,31
	jne	L_A6
	jmp	L_A4
	}
}

void __declspec(naked) dyna_linker()
{
	_asm{
	;/* eax = virtual target address */
	;/* ebx = instruction to patch */
	mov	ecx,eax
	mov	edx,1023
	shr	ecx,12
	and	edx,ecx
	and	ecx,0xDFFFF
	or	edx,1024
	cmp	ecx,edx
	cmova	ecx,edx
	;/* jump_in lookup */
	mov	edx,[jump_in+ecx*4]
L_B1:
	test	edx, edx
	je	L_B3
	mov	edi,[edx]
	xor	edi,eax
	je	L_B2
	mov	edx,[12+edx]
	jmp	L_B1
L_B2:
	mov	edi,[ebx]
	mov	ebp,esi
	lea	esi,[4+ebx+edi]
	mov	edi,eax
	pushad
	call	add_link
	popad
	mov	edi,[8+edx]
	mov	esi,ebp
	lea	edx,[-4+edi]
	sub	edx,ebx
	mov	[ebx],edx
	jmp	edi
L_B3:
	;/* hash_table lookup */
	mov	edi,eax
	shr	edi,16
	xor	edi,eax
	movzx	edi,di
	shl	edi,4
	cmp	[hash_table+edi], eax
	jne	L_B5
L_B4:
	mov	[hash_table+4+edi], edx
	jmp	edx
L_B5:
	cmp	[hash_table+8+edi],eax
	lea	edi,[8+edi]
	je	L_B4
	;/* jump_dirty lookup */
	mov	edx,[jump_dirty+ecx*4]
L_B6:
	test	edx, edx
	je	L_B8
	mov	ecx,[edx]
	xor	ecx,eax
	je	L_B7
	mov	edx,[12+edx]
	jmp	L_B6
L_B7:
	mov	edx,[8+edx]
	;/* hash_table insert */
	mov	ebx,[hash_table-8+edi]
	mov	ecx,[hash_table-4+edi]
	mov	[hash_table-8+edi],eax
	mov	[hash_table-4+edi],edx
	mov	[hash_table+edi],ebx
	mov	[hash_table+4+edi],ecx
	jmp	edx
L_B8:
	mov	edi,eax
	pushad
	call	sh2_recompile_block
	test	eax, eax
	popad
	je	dyna_linker
	;/* shouldn't happen */
	int 3
	}
}

void __declspec(naked) jump_vaddr_eax_master()
{
	_asm{
	mov	edi,eax
	jmp	jump_vaddr_edi_master
	}
}
void __declspec(naked) jump_vaddr_ecx_master()
{
	_asm{
	mov	edi,ecx
	jmp	jump_vaddr_edi_master
	}
}
void __declspec(naked) jump_vaddr_edx_master()
{
	_asm{
	mov	edi,edx
	jmp	jump_vaddr_edi_master
	}
}
void __declspec(naked) jump_vaddr_ebx_master()
{
	_asm{
	mov	edi,ebx
	jmp	jump_vaddr_edi_master
	}
}
void __declspec(naked) jump_vaddr_ebp_master()
{
	_asm{
	mov	edi,ebp
	jmp	jump_vaddr_edi_master
	}
}
void __declspec(naked) jump_vaddr_eax_slave()
{
	_asm{
	mov	edi,eax
	jmp	jump_vaddr_edi_slave
	}
}
void __declspec(naked) jump_vaddr_ecx_slave()
{
	_asm{
	mov	edi,ecx
	jmp	jump_vaddr_edi_slave
	}
}
void __declspec(naked) jump_vaddr_edx_slave()
{
	_asm{
	mov	edi,edx
	jmp	jump_vaddr_edi_slave
	}
}
void __declspec(naked) jump_vaddr_ebx_slave()
{
	_asm{
	mov	edi,ebx
	jmp	jump_vaddr_edi_slave
	}
}
void __declspec(naked) jump_vaddr_ebp_slave()
{
	_asm{
	mov	edi,ebp
	jmp jump_vaddr_edi_slave
	}
}
void __declspec(naked)  jump_vaddr_edi_slave()
{
		_asm{
	or	edi,1
	jmp jump_vaddr_edi_master
		}
}
void __declspec(naked)  jump_vaddr_edi_master()
{
	_asm{
	mov	eax,edi
	jmp jump_vaddr
	}
}
void __declspec(naked)  jump_vaddr()
{
	_asm{
  ;/* Check hash table */
	shr	eax,16
	xor	eax,edi
	movzx	eax,ax
	shl	eax,4
	cmp	dword ptr[hash_table+eax],edi
	jne	L_C2
L_C1:
	mov	edi,[hash_table+4+eax]
	jmp	edi
L_C2:
	cmp	[hash_table+8+eax],edi
	lea	eax,[8+eax]
	je	L_C1
  ;/* No hit on hash table, call compiler */
	push	edi
	call	get_addr
	add	esp,4
	jmp	eax
	}
}
void __declspec(naked) verify_code()
{
	_asm{
	;/* eax = source */
	;/* ebx = target */
	;/* ecx = length */
	mov	edi,[-4+eax+ecx]
	xor	edi,[-4+ebx+ecx]
	jne	L_D5
	mov	edx,ecx
	add	ecx,-4
	je	L_D3
	test	edx,4
	cmove	ecx,edx
	push	esi
L_D2:
	mov	edx,[-4+eax+ecx]
	mov	ebp,[-4+ebx+ecx]
	mov	esi,[-8+eax+ecx]
	xor	ebp,edx
	mov	edi,[-8+ebx+ecx]
	jne	L_D4
	xor	edi,esi
	jne	L_D4
	add	ecx,-8
	jne	L_D2
	pop	esi
L_D3:
	ret
L_D4:
	pop	esi
L_D5:
	add	esp,4 ;/* pop return address, we're not returning */
	call	get_addr
	add	esp,4 ;/* pop virtual address */
	jmp	eax
	}
}
void __declspec(naked) FASTCALL WriteInvalidateLong(u32 addr, u32 val)
{
	_asm{
	mov	ecx,eax
	shr	ecx,12
	bt	[cached_code],ecx
	;*FASTCLL
	;jnc	MappedMemoryWriteLong
	jnc	write
	push	eax
	push	edx
	push	eax
	call	invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	;*FASTCLL
	;jmp	MappedMemoryWriteLong
write:
	push	ecx
	mov		ecx,eax
	call	MappedMemoryWriteLong
	pop		ecx
	ret
	}
}
void __declspec(naked) FASTCALL WriteInvalidateWord(u32 addr, u32 val)
{
	_asm{
	mov	ecx,eax
	shr	ecx,12
	bt	[cached_code],ecx
	;*FASTCLL
	;jnc	MappedMemoryWriteWord
	jnc	write
	push	eax
	push	edx
	push	eax
	call	invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	;*FASTCLL
	;jmp	MappedMemoryWriteWord
write:
	push	ecx
	mov		ecx,eax
	call	MappedMemoryWriteWord
	pop		ecx
	ret
	}
}
void __declspec(naked) FASTCALL WriteInvalidateByteSwapped(u32 addr, u32 val)
{
	_asm{
	xor	eax,1
	jmp WriteInvalidateByte
	}
}
void __declspec(naked) FASTCALL WriteInvalidateByte(u32 addr, u32 val)
{
	_asm{
	mov	ecx,eax
	shr	ecx,12
	bt	[cached_code],ecx
	;*FASTCLL
	;jnc	MappedMemoryWriteByte
	jnc	write
	push	eax
	push	edx
	push	eax
	call	invalidate_addr
	pop	eax
	pop	edx
	pop	eax
	;FASTCLL
	;jmp	MappedMemoryWriteByte
write:
	push	ecx
	mov		ecx,eax
	call	MappedMemoryWriteByte
	pop		ecx
	ret
	}
}
void __declspec(naked) div1()
{
	_asm{
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
	}
}
void __declspec(naked) macl()
{
	_asm{
	;/* ebx = sr */
	;/* ebp = multiplicand address */
	;/* edi = multiplicand address */
	;/* eax = return MACL */
	;/* edx = return MACH */
	push	edx ;/* MACH */
	push	eax ;/* MACL */
	mov	eax,edi
	; *FASTCLL*
	push	ecx
	mov		ecx,eax
	call	MappedMemoryReadLong
	pop		ecx
	mov	esi,eax
	mov	eax,ebp
	push	ecx
	mov		ecx,eax
	call	MappedMemoryReadLong
	pop		ecx
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
	cmp	edx,esi
	cmovg	edx,esi
	cmovg	eax,ecx
	ret
	}
}
void __declspec(naked) macw()
{
	_asm{
	;/* ebx = sr */
	;/* ebp = multiplicand address */
	;/* edi = multiplicand address */
	;/* eax = return MACL */
	;/* edx = return MACH */
	push	edx ;/* MACH */
	push	eax ;/* MACL */
	mov	eax,edi
	;*FASTCLL
	push	ecx
	mov		ecx,eax
	call	MappedMemoryReadWord
	pop		ecx
	movsx	esi,ax
	mov		eax,ebp
	;*FASTCLL
	push	ecx
	mov		ecx,eax
	call	MappedMemoryReadWord
	pop		ecx
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
	sar	esi,31
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
	}
}

void __declspec(naked) master_handle_bios()
{
	_asm{
	mov	edx,[esp] ;/* get return address */
	mov	[master_pc],eax
	mov	[master_cc],esi
	mov	[master_ip],edx
	mov	eax,MSH2
	;*FASTCLL
	push	ecx
	mov		ecx,eax
	call	BiosHandleFunc
	pop		ecx
	mov	edx,master_ip
	mov	esi,master_cc
	mov	[esp],edx
	ret	;/* jmp *master_ip */
	}
}
void __declspec(naked) slave_handle_bios()
{
	_asm{
	pop	edx ;/* get return address */
	mov	[slave_pc],eax
	mov	[slave_cc],esi
	mov	[slave_ip],edx
	mov	eax,[SSH2]
	;*FASTCLL
	push	ecx
	mov		ecx,eax
	call	BiosHandleFunc
	pop		ecx
	mov	edx,slave_ip
	mov	esi,slave_cc
	jmp	edx ;/* jmp *slave_ip */
	}
}
void __declspec(naked) breakpoint()
{
	_asm{
	ret
	;/* Set breakpoint here for debugging */
	}
}



