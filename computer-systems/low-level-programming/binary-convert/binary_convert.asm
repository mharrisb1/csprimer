        section .text
        global binary_convert
binary_convert:
        xor eax, eax                   ; zero out accumulator

.loop:
        movzx ecx, byte [rdi]          ; take next byte from input string
        cmp ecx, 0                     ; check if null byte
        je .end
        and ecx, 1                     ; get ls bit from char
        shl eax, 1                     ; acc << 1
        add eax, ecx                   ; add char to acc
        add rdi, 1                     ; inc to next char pointer
        jmp .loop

.end:
        ret
