;MBR BOOTSTRAP/LOADER INTEGRRATION PROGRAM

[org 0]
jmp 0x07c0:start
start :
        mov     ax, cs
        mov     ds, ax
        mov     es, ax

        mov     ax, 0xffff
        mov     ss, ax
        mov     sp, ax
        mov     bp, ax

        push    7
        mov     ax, msgIntro
        push    ax
        push    0
        push    0
        call    print

        call    ReadSector

         cli
        lgdt [gdtr]

         mov eax, cr0
         or eax, 1
         mov cr0, eax

         jmp pipeinit
         nop
         nop
         pipeinit:

         jmp dword SysCodeSelector:0x7c00 + Layer_32

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

print:
    push    bp
    push    bx
    push    es
    
    mov     bp, sp 
    mov     ax, 0xb800
    mov     es, ax

    mov     ax, [ss:bp+8]
    mov     bx, 2
    mul     bx  
    mov     bx, ax
 
    mov     ax, 160
    mul     word [ss:bp+10]
    add     bx, ax

    mov     si, [ss:bp+12]

    mov     ax, [ss:bp+14]

    mov     di, bx
    
    LOOP_LABLE:
        mov     ah, byte [ds:si]
        mov     byte [es:bx], ah
        inc     bx
        mov     byte [es:bx], al
        inc     bx
        inc     si
        cmp     ah, 0
        jne     LOOP_LABLE

    pop     es
    pop     bx
    pop     bp
    retn

ReadSector:
    push    bp
    push    es
    pushf
   
        mov     ax, 0x1000
        mov     es, ax
        mov     bx, 0

    read:
        mov     ah, 2
        mov     al, 1
        mov     ch, byte [TRACKCOUNT]
        mov     cl, byte [SECTORCOUNT]
        mov     dh, byte [HEADCOUNT]
        mov     dl, 0
        int     0x13
        jc      error

        inc     byte [SECTORCOUNT]
        cmp     byte [SECTORCOUNT], 19
        jb      endRead
        mov     byte [SECTORCOUNT], 1
        inc     byte [HEADCOUNT]
        cmp     byte [HEADCOUNT], 2
        jb      endRead
        mov     byte [HEADCOUNT], 0 
        inc     byte [TRACKCOUNT]
        cmp     byte [TRACKCOUNT], 80
        jb      endRead

    
    endRead:

        mov     bx, es
        add     bx, 0x20
        mov     es, bx

        mov     bx, 0
        inc     word [LOOPCOUNT]    
        mov     cx, word [LOOPCOUNT]
        cmp     cx, 30
        jb      read

    popf
    pop     es
    pop     bp
    retn

error:
        push    4
        mov     ax, msgError
        push    ax
        push    1
        push    0

        call    print
        jmp $
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

msgIntro db 'Starting up...', 0
msgError db 'Error on reading data.', 0

SECTORCOUNT : db 2
HEADCOUNT   : db 0
TRACKCOUNT  : db 0
LOOPCOUNT   : dw 0

[bits 32]
    Layer_32:
        mov ax, SysDataSelector
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov gs, ax
        mov fs, ax

        call enable_A20

        mov esp, 0x1fffff
        mov ebp, esp

        jmp dword SysCodeSelector:0x10000


enable_A20:
    call    a20wait
    mov     al,0xAD
    out     0x64,al

    call    a20wait
    mov     al,0xD0
    out     0x64,al

    call    a20wait2
    in      al,0x60
    push    eax

    call    a20wait
    mov     al,0xD1
    out     0x64,al

    call    a20wait
    pop     eax
    or      al,2
    out     0x60,al

    call    a20wait
    mov     al,0xAE
    out     0x64,al

    call    a20wait
    ret

a20wait:
.l0:    mov     ecx,65536
.l1:    in      al,0x64
    test    al,2
    jz      .l2
    loop    .l1
    jmp     .l0
.l2:    ret

a20wait2:
.l0:    mov     ecx,65536
.l1:    in      al,0x64
    test    al,1
    jnz     .l2
        loop    .l1
    jmp     .l0
.l2:    ret

gdtr:
        dw gdt_end - gdt - 1
        dd gdt+0x7c00
gdt:
    ;null descriptor
        dq 0
   SysCodeSelector equ 0x08
        dw 0xffff
        dw 0x0000
        db 0x00
        db 0x9a
        db 0xcf
        db 0
   SysDataSelector equ 0x10
        dw 0xffff
        dw 0x0000
        db 0x00
        db 0x92
        db 0xcf
        db 0
gdt_end:

times 510-($-$$)    db 0
dw  0xaa55
