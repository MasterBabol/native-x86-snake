[BITS 32]	

GLOBAL	_setcursor

GLOBAL	_io_hlt, _io_cli, _io_sti, io_stihlt
GLOBAL	_io_in8,  _io_in16,  _io_in32
GLOBAL	_io_out8, _io_out16, _io_out32
GLOBAL	_io_load_eflags, _io_store_eflags
GLOBAL	_load_gdtr, _load_idtr
GLOBAL	_load_cr0, _store_cr0, _load_tr

extern _kernel_main, _timer_int, _syscall_sub, _key_int

GLOBAL	_memtest

global _int_handler_30
global _int_handler_31
global _int_handler_79

global _switchtask
global _getds, _setds

section .text

%macro SAVEREG 0
	pushad
	pushfd
%endmacro


%macro LOADREG 0
	popfd
	popad
%endmacro


entry:
call _kernel_main
jmp $

_setcursor :
	push  ebp
	mov   ebp, esp
	push  ebx
	push  edx
	push  eax
	pushfd

	mov      eax, 80
	mov      ebx, dword [ss:ebp + 12]
	mul      ebx

	mov      ebx, dword [ss:ebp + 8]
	add      eax, ebx
	mov      ebx, eax


	mov      dx, 0x03d4
	mov      ax, 0x0e
	out      dx, al
	mov      dx, 0x03d5
	mov      eax, ebx
	shr      eax, 8
	out      dx, al


	mov      dx, 0x03d4
	mov      ax, 0x0f
	out      dx, al
	mov      dx, 0x03d5
	mov      eax, ebx
	out      dx, al

	popfd
	pop      eax
	pop      edx
	pop      ebx
	pop      ebp 

	retn

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:	; void io_cli(void);
		CLI
		RET

_io_sti:	; void io_sti(void);
		STI
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX, 0
		IN		AL, DX
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX, 0
		IN		AX, DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX, DX
		RET

_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX, AL
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX, AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX, EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; PUSH EFLAGS의 의미
		POP		EAX
		RET

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; POP EFLAGS의 의미
		RET

_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV	AX,[ESP+4] ; limit
		MOV	[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV	AX,[ESP+4]  ; limit
		MOV	[ESP+6],AX
		LIDT	[ESP+6]
		RET

_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET


_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_load_tr:		; void load_tr(int tr);
		LTR		[ESP+4]			; tr
		RET


_memtest:; unsigned int memtest (unsigned int start, unsigned int end)
PUSH	EDI
PUSH	ESI
PUSH	EBX
MOV	ESI, 0xaa55aa55
MOV	EDI, 0x55aa55aa
MOV	EAX,[ESP+12+4]
mts_loop:
MOV	EBX,EAX
ADD	EBX, 0xffc
MOV	EDX,[EBX]
MOV	[EBX], ESI
XOR	DWORD [EBX], 0xffffffff
CMP	EDI,[EBX]	
JNE	mts_fin
XOR	DWORD [EBX], 0xffffffff
CMP	ESI,[EBX]	
JNE	mts_fin
MOV	[EBX], EDX
ADD	EAX, 0x1000
CMP	EAX,[ESP+12+8]
JBE	mts_loop
POP	EBX
POP	ESI
POP	EDI
RET
mts_fin:
MOV	[EBX], EDX
POP	EBX
POP	ESI
POP	EDI
RET

_switchtask:
jmp far [esp+4];
ret

_getds:
xor eax, eax
mov ax, ds
ret

_setds:
mov eax, [esp+4]
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
ret

_int_handler_30:
SAVEREG

call _timer_int

LOADREG
iretd

_int_handler_31:
SAVEREG

call _key_int

LOADREG
iretd

_int_handler_79:
push edx
push ecx
push ebx
push eax
call _syscall_sub
add esp, 16
iretd


