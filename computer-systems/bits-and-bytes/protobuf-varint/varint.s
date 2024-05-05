/*

        @author:  Michael Harris
        @date:    2024-04-29
        
        About
        ----------
        
        A64 solution for protobuf varint problem in bits and bytes section of comp sys module

        Function signatures:

                void varint_encode(uint8_t *buf, uint64_t i);
                uint64_t varint_decode(uint8_t *buf);

        Approach
        ----------

        - Try to hand code the asm as much as possible and refer to compiled asm for hints
        - Whether my own work or from copying the compiler's output, annotate every line

        Notes
        ----------

        - Why 32 byte allocation on the stack?
                - 8 bytes for buf address, 8 bytes for i, 1 byte for temp counter (== 17 bytes)
                - For perf, it is recommended to align using a multiple of 16 (round up to 32)
                - Decode is similar with 8 bytes for buf address, 1 byte for size, and 8 bytes for return value
        - Use `[x/w]zr` register to access a zero value without having to manually set a register to zero first
        - The compiler (GCC using -O0) only uses 2 registers. Is there a good reason for this? Can you make it faster otherwise?
        - The compiler adds some `nop` instructions. Is this because of a compiler trick or is the way the C is written confusing?
        - Are there any good examples of well-structured assembly codebases that minimize repeating code?

        References
        ----------

        - https://developer.arm.com/documentation/ddi0602/2024-03/Base-Instructions
        - https://developer.arm.com/documentation/102374/0101/Procedure-Call-Standard
        - https://developer.arm.com/documentation/102374/0101/Registers-in-AArch64---general-purpose-registers

        Additional Resources
        --------------------

        - Godbolt for reverse engineering C to GNU ARM
        - GPT-4-Turbo for talking through why stack pointer aligment is important (still kind of fuzzy though)
        - GPT-4-Turbo for annotating lines of assembly if I needed an extra explanation of why the compiler did something
        - GPT-4-Turbo for finding out a way to iterate through the buffer in ascending order instead of descending in decode

*/
        .text
        
        .global varint_encode
        .type varint_encode, @function

        .global varint_decode
        .type varint_decode, @function

varint_encode:
        sub sp, sp, 32        // allocate 32 bytes on the stack
        str x0, [sp, 8]       // store buffer addr on stack
        str x1, [sp]          // store i on stack
        strb wzr, [sp, 31]    // store counter on stack with initial value of 0

.slice:
        ldr x0, [sp]          // load i from stack
        and w0, w0, 0x7F      // take lowest 7 bits from i
        strb w0, [sp, 30]     // store part on stack
        ldr x0, [sp]          // reload unsliced i from stack
        lsr x0, x0, 7         // right shift i by 7 bits
        str x0, [sp]          // store shifted i on stack
        cmp x0, 0             // i == 0
        beq .enqueue
        ldrb w0, [sp, 30]     // load part from stack
        orr w0, w0, 0x80      // flip on continuation bit
        strb w0, [sp, 30]     // store part back on stack        

.enqueue:
        ldr x0, [sp, 8]       // load buffer addr from stack
        ldrb w1, [sp, 31]     // load counter from stack
        add x0, x0, x1        // get offset address for buffer
        add w1, w1, 1         // increment counter
        strb w1, [sp, 31]     // store incremented counter on stack
        ldrb w1, [sp, 30]     // load part from stack
        strb w1, [x0]         // store part at buffer offset addr
        ldr x0, [sp]          // load i from stack
        cmp x0, 0             // i == 0
        bne .slice
        add sp, sp, 32        // deallocate bytes from stack
        ret

varint_decode:
        sub sp, sp, 32        // allocate 32 bytes on the stack
        str x0, [sp, 8]       // store buffer addr on stack. Use same offset as encode for readability
        str xzr, [sp]         // store i on stack with initial value of 0
        strb wzr, [sp, 31]    // store counter on stack with initial value of 0
        strb wzr, [sp, 29]    // store shift counter on stack. Skip sp[30:31] to reuse for part variable

.dequeue:
        ldr x0, [sp, 8]       // load buffer addr from stack
        ldrb w1, [sp, 31]     // load counter from stack
        add x0, x0, x1        // get offset address from buffer
        add w1, w1, 1         // increment counter
        strb w1, [sp, 31]     // store incremented counter on stack
        ldrb w0, [x0]         // load part from buffer offset addr
        strb w0, [sp, 30]     // store part on stack
        and w0, w0, 0x7F      // ignore continuation bit
        ldrb w1, [sp, 29]     // load shift counter from stack
        lsl x0, x0, x1        // left shift part by shift counter bits
        ldr x1, [sp]          // load i from stack
        orr x1, x1, x0        // flip on positive bits in i that are flipped on in part
        str x1, [sp]          // store i on stack
        ldrb w0, [sp, 30]     // load unshifted part from stack
        and w0, w0, 0x80      // only keep continuation bit
        cmp w0, 0x80          // check if continuation bit is flipped on
        bne .return
        ldrb w1, [sp, 29]     // load shift counter from stack
        add w1, w1, 7         // increment shift counter by 7
        strb w1, [sp, 29]     // store incremented shift counter on stack
        b .dequeue

.return:
        ldr x0, [sp]          // load i from stack in return register
        add sp, sp, 32        // deallocate bytes from stack
        ret
