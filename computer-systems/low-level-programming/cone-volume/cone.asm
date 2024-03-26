        default rel

        section .text
        global volume
; xmm0 = radius (and return)
; xmm1 = height
volume:
        mulss xmm0, xmm0               ; v = r^2
        mulss xmm0, xmm1               ; v *= h
        mulss xmm0, [pi_on_3]          ; v *= pi/3
        ret

        section .rodata
pi_on_3:
        dd 1.0472
