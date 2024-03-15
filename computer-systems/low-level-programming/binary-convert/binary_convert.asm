        %define MASK 01h

        section .text
        global binary_convert
binary_convert:
        xor eax, eax                   ; accumulator, init to 0
        xor ecx, ecx                   ; power, init to 0
        xor rsi, rsi                   ; string offset, init to 0

.strlen:
        movzx edx, byte[rdi + rsi]     ; get char at offset
        cmp edx, 0
        je .loop
        add rsi, 1                     ; increment offset
        jmp .strlen

.loop:
        cmp esi, 0                     ; guards against seg faults
        je .done
        sub esi, 1
        movzx edx, byte[rdi + rsi]     ; get char at offset
        and edx, MASK                  ; turn '0' -> 0 or '1' -> 1
        shl edx, cl                    ; edx := edx**power
        add cl, 1                      ; increment power
        add eax, edx                   ; add to accumulator
        jmp .loop

.done:
        ret
