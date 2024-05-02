/*

        @author:  Michael Harris
        @date:    2024-04-30
        
        About
        ----------
        
        A64 solution for protobuf varint problem in bits and bytes section of comp sys module

        Function signatures:

                void varint_encode(uint8_t *buf, uint64_t i);
                uint64_t varint_decode(uint8_t *buf, uint8_t size);

        Notes
        ----------

        - Why 32 byte allocation on the stack?
                - 8 bytes for buf address, 8 bytes for i, 1 byte for temp counter (== 17 bytes)
                - For perf, it is recommended to align using a multiple of 16 (round up to 32)
                - Decode is similar with 8 bytes for buf address, 1 byte for size, and 8 bytes for return value

        - Unsafe since we check buffer size on decode but not on encode but there should not be an instance where we go out of bounds

        References
        ----------

        - https://developer.arm.com/documentation/ddi0602/2024-03/Base-Instructions
        - https://developer.arm.com/documentation/102374/0101/Procedure-Call-Standard
        - https://developer.arm.com/documentation/102374/0101/Registers-in-AArch64---general-purpose-registers
        - https://courses.cs.washington.edu/courses/cse469/19wi/arm64.pdf

        Additional Resources
        --------------------

        - Godbolt for reverse engineering C to GNU ARM
        - GPT-4-Turbo for annotating lines of assembly code created by godbolt if I needed an extra explanation

*/
        .text
        
        .global varint_encode
        .type varint_encode, @function

        .global varint_decode
        .type varint_decode, @function

varint_encode:
        sub sp, sp, 32         // allocate 32 bytes on the stack
        str x0, [sp, 8]        // store ref to `buf` at sp + 8
        str x1, [sp]           // store ref to `i` at sp + 0
        strb wzr, [sp, 31]     // store one byte of zero register at sp + 31 for counter
        b .loop1

/* Responsible for taking the 7 lowest bits with optional continuation bit  */
.takelow7b:
        ldr x0, [sp]           // load `i` from stack
        and w0, w0, 0x7F       // mask to get lowest 7 bits
        strb w0, [sp, 30]      // temp storage of lowest 7 bits
        ldr x0, [sp]           // reload `i`
        lsr x0, x0, 7          // local shift right 7 bits
        str x0, [sp]           // store shifted `i` back to memory
        ldr x0, [sp]           // reload `i`
        cmp x0, 0              // check if we've exhausted bits in `i`
        beq .enqueue           // skip flipping continuation bit if exhausted
        ldrb w0, [sp, 30]      // load lowest 7 bit part we took earlier
        eor w0, w0, 0x80       // flip on continuation bit
        strb w0, [sp, 30]      // store part back in mem

/* Pushes part to buffer */
.enqueue:
        ldrb w0, [sp, 31]      // load counter from stack
        ldr x1, [sp, 8]        // load buffer from stack
        add x0, x1, x0         // calculate buffer offset
        ldrb w1, [sp, 30]      // load part from stack
        strb w1, [x0]          // store part in buffer at offset
        ldrb w0, [sp, 31]      // reload counter from stack
        add w0, w0, 1          // counter++
        strb w0, [sp, 31]      // store incremented counter
        
/* Main while loop  */
.loop1:
        ldr x0, [sp]           // load `i` from stack
        cmp x0, 0              // if (i == 0)
        bne .takelow7b         // go to `.enqueue` if not eq to 0
        add sp, sp, 32         // deallocate bytes from stack
        ret
        
varint_decode:
        sub sp, sp, 32         // allocate 32 bytes on the stack
        str xzr, [sp]          // store return value, `i` with initial value as 0 at sp + 0
        str x0, [sp, 8]        // store `buf` at sp + 8
        strb w1, [sp, 31]      // store `size` at sp + 31
        b .loop2

.dequeue:
        ldrb w1, [sp, 31]      // load `size` from stack
        sub x1, x1, 1          // decrement `size`
        strb w1, [sp, 31]      // store decremented size
        ldr x0, [sp, 8]        // load `buf` address from stack
        add x0, x0, x1         // get byte offset for buffer value
        ldrb w0, [x0]          // load part from buffer at offset
        strb w0, [sp, 30]      // store part on stack at sp + 30
        cmp w0, 0              // if (part == 0)
        beq .loop2
        and w0, w0, 0x7F       // ignore continuation bit in part
        ldr x1, [sp]           // load `i` from stack
        lsl x1, x1, 7          // logical left shift `i` 7 bits
        orr x1, x1, x0         // `i` | `part`
        str x1, [sp]           // store `i` back on stack at sp + 0
        ldrb w0, [sp, 30]      // reload unmanipulated part from stack
        and w0, w0, 0x80       // only keep continuation bit from part
        cmp w0, 0x80           // check if continuation bit is on
        bne .loop2
        strb wzr, [sp, 31]     // if continuation bit is not on set `size` to zero

.loop2:
        ldrb w0, [sp, 31]      // load `size` from stack
        cmp w0, 0              // if (i == 0)
        bne .dequeue
        ldr x0, [sp]           // load return value from stack
        add sp, sp, 32         // deallocate bytes from stack
        ret
