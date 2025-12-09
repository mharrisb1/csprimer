.global f
.type  f, @function

f:
    pushq   %rbp          
    movq    %rsp, %rbp    
    subq    $16, %rsp     

    movl    %edi, -4(%rbp)

    cmpl    $1, %edi      
    jg      .Lcheck_two   
    movl    $1, %eax      
    jmp     .Lend         

.Lcheck_two:

    cmpl    $2, %edi      
    jne     .Lrecurse     
    movl    $2, %eax      
    jmp     .Lend         

.Lrecurse:

    movl    -4(%rbp), %edi
    leal    -1(%edi), %edi
    call    f             
    movl    %eax, -8(%rbp)

    movl    -4(%rbp), %edi
    leal    -2(%edi), %edi
    call    f             
    addl    %eax, -8(%rbp)

    movl    -4(%rbp), %edi
    leal    -3(%edi), %edi
    call    f             

    addl    -8(%rbp), %eax
        
.Lend:
    leave                 
    ret                   

    .section .note.GNU-stack,"",@progbits
