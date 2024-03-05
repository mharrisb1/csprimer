; formula used is n(n + 1) / 2
; source https://math.stackexchange.com/a/60581

        global sum_to_n
        section .text
sum_to_n:
        mov rax, rdi                   ; copy n to rax (also avoids needing to clear rax)
        mov rdx, rax                   ; copy n to rdx
        mul rax                        ; n = n^2
        add rax, rdi                   ; n = n + 1
        shr rax, 1                     ; n = n / 2
        ret
