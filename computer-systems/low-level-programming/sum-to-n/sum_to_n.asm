        global sum_to_n
        section .text
sum_to_n:
        mov rax, rdi
        mov rdx, rax
        mul rax
        add rax, rdi
        shr rax, 1
        ret
