        section .text
        global mat_index
mat_index:
; rdi: matrix
; esi: rows
; edx: cols
; ecx: rindex
; r8d: cindex
        imul rdx, 4                    ; calc row width
        imul rcx, rdx                  ; calc row offset
        imul r8, 4                     ; calc col offset
        add rcx, r8                    ; calc total offset
        mov rax, [rdi + rcx]           ; dereference element at offset
        ret
