        global sum_to_n
        section .text
sum_to_n:
        xor rax, rax                   ; reset accumulator register

reduce:
        add rax, rdi                   ; add n to accumulator
        sub rdi, 1                     ; decrement n
        jnc reduce                     ; loop if no carry (proxy for n != 0 with underflow safety)
        ret
