        default rel

        section .text
        global volume
; xmm0 = radius (and return)
; xmm1 = height
volume:
        mulss xmm0, xmm0               ; r^2
        mulss xmm0, [pi]               ; r^2 * pi
        mulss xmm0, xmm1               ; r^2 * pi * h
        divss xmm0, [three_f]          ; 1/3 * (r^2 * pi * h)
        ret

        section .data
pi:
        dd 3.1416                      ; approx pi

three_f:
        dd 3.0                         ; 3 floating point
