        %define MASK 07fffffeh

        section .text
        global pangram
pangram:
; rdi: source string
        xor ecx, ecx                   ; bs = 0

.loop:
        movzx edx, byte[rdi]           ; c = *s
        cmp edx, 0
        je .end
        add rdi, 1                     ; s++
        cmp edx, '@'                   ; if c is first 64 chars of ascii table
        jl .loop                       ; continue
        bts ecx, edx
        jmp .loop

.end:
        xor eax, eax
        and ecx, MASK
        cmp ecx, MASK
        sete al
        ret
