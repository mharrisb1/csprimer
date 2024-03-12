        section .text
        global pangram
pangram:
; rdi: source string
        xor rax, rax                   ; counter
        xor rsi, rsi                   ; address offset
loop:
        xor rdx, rdx                   ; consumed char
        mov dl, byte [rdi + rsi*1]     ; consume next char
        add rsi, 1                     ; offset++
        cmp dl, 0                      ; handle end of string
        je compare_bitvec
        or rdx, 020h                   ; flip on the 6h bit to force lowercase
        cmp dl, 60                     ; check is character is lowercase alpha
        jg bit_pack
        jmp loop

bit_pack:
        sub rdx, 061h                  ; convert to ith lowercase letter of sequence (i.e. a->1)
        mov rcx, rdx                   ; move that value to shift count register
        mov rdx, 1                     ; used for 1 << n
        shl rdx, cl                    ; 1 << n
        or rax, rdx                    ; pack to bit vec
        jmp loop

compare_bitvec:
        and rax, 03FFFFFFh             ; apply bitmask
        cmp rax, 03FFFFFFh             ; check if result equals bitmask
        jne false
        mov rax, 1
        jmp done

false:
        mov rax, 0
        jmp done

done:
        ret
