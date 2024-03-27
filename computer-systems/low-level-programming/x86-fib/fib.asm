        section .text
        global fib
fib:
        mov rax, rdi                   ; if n <= 1 return n
        cmp rdi, 1
        jle .end

        sub rdi, 1
        push rdi                       ; push n - 1 to top of stack
        call fib                       ; f(n - 1)
        pop rdi

        push rax                       ; push f(n - 1) to the stack
        sub rdi, 1
        call fib                       ; f(n - 2)

        pop rcx                        ; pop f(n - 1) from top of stack
        add rax, rcx                   ; f(n - 1) + f(n - 2)
.end:
        ret
