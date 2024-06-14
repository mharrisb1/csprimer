/*
 *        @author    Michael Harris
 *        @date      2024-06-12
 *       
 *        HTTP headers to JSON string
 *
 *        void hhtojson(char *readbuf, char *writebuf, ssizt_t offset);
 *
 *        @param readbuf  - Buffer containing HTTP headers
 *        @param writebuf - Buffer to write JSON string to
 *        @param offset   - Location in writebuf to start writing to
 *
 *        Methodology
 *        -----------
 *        One-pass psuedo-lexer that reads a character of the header payload at
 *        a time and writes the character to a JSON serialized string stored in
 *        the write buffer starting at offset.
 *        
 *        Uses a simple state machine design where a token is pieced together a
 *        character at a time and written eagerly to a buffer. If the token ends
 *        up in an invalid state then the eagerly written token is removed from
 *        the buffer by resetting the write index appropriately. Token is either
 *        of type KEY (1) or VALUE (2). The transition of KEY -> VALUE occurs
 *        when a colon (:) character is reached. The transition of VALUE -> KEY
 *        occurs when a newline (\n) character is reached.
 *
 *        If a double quote (") character is reached while in VALUE state then
 *        an escape is added with the reverse solidus (\) character.
 *
 *        References
 *        -----------
 *        - https://www.json.org/json-en.html
 *        - https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
 *
 *        Notes
 *        -----
 *        - There's a bug where spaces in value string are ignore
 *
 */

.equ NULL,              0x00
.equ NEWLINE,           0x0A
.equ SPACE,             0x20
.equ DOUBLE_QUOTE,      0x22
.equ COMMA,             0x2C
.equ COLON,             0x3A
.equ BACKSLASH,         0x5C
.equ LEFT_BRACE,        0x7B
.equ RIGHT_BRACE,       0x7D
.equ DELETE,            0x7F

        .text
        .global _hhtojson

_hhtojson:
        sub sp, sp, 32        // allocate 32 bytes on the stack
        str x0, [sp, 0]       // add readbuf pointer to stack
        str x1, [sp, 8]       // add writebuf pointer to stack
        str x2, [sp, 16]      // add offset to stack
        str wzr, [sp, 24]     // add read index to stack
        strb wzr, [sp, 28]    // add token type enum to stack
        str wzr, [sp, 30]     // add token size to stack

.prep:
        ldr x0, [sp, 8]       // load writebuf pointer from stack
        ldr x1, [sp, 16]      // load offset from stack
        mov w2, LEFT_BRACE
        add x0, x0, x1        // increment pointer by offset
        strb w2, [x0]         // write to buffer
        add x0, x0, 1         // increment buffer 
        mov w2, DOUBLE_QUOTE
        strb w2, [x0]         // write to buffer
        add x1, x1, 2         // increment offset by 2
        str x1, [sp, 16]      // store incremented offset on stack

.tokenize:
        ldr x0, [sp, 0]       // load readbuf pointer from stack
        ldr w1, [sp, 24]      // load read index from stack
        add x0, x0, x1        // get read address
        ldrb w0, [x0]         // read char
        add w1, w1, 1         // increment read index
        str w1, [sp, 24]      // store incremented read index on stack
        mov w1, wzr           // clear register

        cmp w0, NULL
        beq .terminate

        cmp w0, DOUBLE_QUOTE
        beq .escape

        cmp w0, NEWLINE
        beq .transition2

        cmp w0, SPACE
        ble .tokenize

        cmp w0, COLON
        beq .transition1

        cmp w0, DELETE
        bge .tokenize

        b .write

.escape:
        mov w1, w0            // move char to secondary write register
        mov w0, BACKSLASH     // move reverse solidus to primary write register

.write:
        ldr x2, [sp, 8]       // load writebuf pointer from stack
        ldr x3, [sp, 16]      // load offset from stack
        ldrb w4, [sp, 30]     // load token size from stack
        add x2, x2, x3        // increment pointer by offset
        strb w0, [x2]         // write to buffer
        add x3, x3, 1         // increment offset
        add w4, w4, 1         // increment token size
        str x3, [sp, 16]      // store incremented offset on stack
        str w4, [sp, 30]      // store incremented token size on stack
        cbz w1, .tokenize     // move on if secondary write register is empty
        mov w0, w1            // move secondary to primary
        mov w1, wzr           // clear secondary
        b .write

.transition1:
        ldrb w2, [sp, 28]     // load token type enum from stack
        mov w1, wzr
        cbnz w2, .write       // if in VALUE state write char
        mov w0, w2
        eor w0, w0,  0x01     // toggle state
        str wzr, [sp, 30]     // reset token size
        strb w0, [sp, 28]     // store transitioned state on stack
        ldr x0, [sp, 8]       // load writebuf pointer from stack
        ldr x1, [sp, 16]      // load offset from stack
        add x0, x0, x1        // increment pointer by offset
        mov w2, DOUBLE_QUOTE
        strb w2, [x0]         // write to writebuf
        add x1, x1, 1         // increment offset
        str x1, [sp, 16]      // store incremented offset on stack
        mov w0, COLON
        mov w1, DOUBLE_QUOTE
        b .write

.transition2:
        ldrb w0, [sp, 28]     // load token type enum from stack
        cmp w0, 0x00          // if in KEY state clear token and start over
        beq .clear
        eor w0, w0, 0x01      // toggle state
        str wzr, [sp, 30]     // reset token size
        strb w0, [sp, 28]     // store transitioned state on stack
        ldr x0, [sp, 8]       // load writebuf pointer from stack
        ldr x1, [sp, 16]      // load offset from stack
        add x0, x0, x1        // increment pointer by offset
        mov w2, DOUBLE_QUOTE
        strb w2, [x0]         // write to writebuf
        add x1, x1, 1         // increment offset
        str x1, [sp, 16]      // store incremented offset on stack
        mov w0, COMMA
        mov w1, DOUBLE_QUOTE
        b .write

.clear:
        ldr x0, [sp, 16]      // load offset from stack
        ldrb w1, [sp, 30]     // load token size from stack
        sub x0, x0, x1        // subtract token size from offset to reset token
        str x0, [sp, 16]      // store new offset value on stack
        str wzr, [sp, 30]     // reset token size to 0
        b .tokenize

.terminate:
        ldr x0, [sp, 8]       // load writebuf from stack
        ldr x1, [sp, 16]      // load offset from stack
        sub x1, x1, 1         // removes the last continuation comma
        add x0, x0, x1        // increment pointer by offset
        mov w2, DOUBLE_QUOTE
        strb w2, [x0]         // write to buffer
        add x0, x0, 1         // increment pointer to next location
        mov w2, RIGHT_BRACE
        strb w2, [x0]         // write to buffer
        add x0, x0, 1         // increment pointer to next location
        mov w2, NULL
        strb w2, [x0]         // add null terminator to end of buffer

.return:
        add sp, sp, 32        // deallocate bytes from stack
        ret
