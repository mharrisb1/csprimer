        section .text
        global mat_index
mat_index:
; rdi: matrix
; esi: rows
; edx: cols
; ecx: rindex
; r8d: cindex
; pointer offset from matrix:
; (rindex * cols * 4) + (cindex * 4) = 4 * (rindex * cols + cindex)
        imul rcx, rdx
        add rcx, r8
        mov rax, [rdi + rcx*4]
        ret
