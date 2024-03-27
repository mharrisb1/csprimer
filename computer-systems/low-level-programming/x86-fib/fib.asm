        section .text
        global fib
; derived from "Recursion" example from https://cs.lmu.edu/~ray/notes/nasmtutorial/
fib:
        xor rax, rax                   ; zero out return value (r)
        jmp .l1

.l1:
        cmp rdi, 1                     ; check n <= 1
        jnbe .l2                       ; jump to next layer if not
        add rax, rdi                   ; r += n
        ret

.l2:
        push rdi                       ; save n to stack
        sub rdi, 1                     ; n--
        call .l1                       ; call previous layer
        sub rdi, 1                     ; n--
        call .l1                       ; call previous layer
        pop rdi                        ; restore n
        ret
