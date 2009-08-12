;
; EasyFlash
;
; (c) 2009 Thomas 'skoe' Giesel
;
; This software is provided 'as-is', without any express or implied
; warranty.  In no event will the authors be held liable for any damages
; arising from the use of this software.
;
; Permission is granted to anyone to use this software for any purpose,
; including commercial applications, and to alter it and redistribute it
; freely, subject to the following restrictions:
;
; 1. The origin of this software must not be misrepresented; you must not
;    claim that you wrote the original software. If you use this software
;    in a product, an acknowledgment in the product documentation would be
;    appreciated but is not required.
; 2. Altered source versions must be plainly marked as such, and must not be
;    misrepresented as being the original software.
; 3. This notice may not be removed or altered from any source distribution.

    .importzp       sp, sreg, regsave
    .importzp       ptr1, ptr2, ptr3, ptr4
    .importzp       tmp1, tmp2, tmp3, tmp4
    .importzp       regbank

    .import         popax

; address of buffer
zpbuff   = ptr1

; address of EasyFlash address
zpaddr   = ptr2

; address of EasyFlash offset
zpoffs   = ptr3

; I/O address used to select the bank
EASYFLASH_IO_BANK    = $de00

; =============================================================================
;
; Fill the given buffer (256 bytes) with the test pattern for the flash address
; pointed to. Refer to the comment in tortureTest.c.
;
; void __fastcall__ tortureTestFillBuffer(const uint8_t* pBuffer,
;                                         const EasyFlashAddr* pAddr);
;
; parameters:
;       pAddr in AX
;       address pBuffer on cc65-stack
;
; return:
;       -
;
; =============================================================================
.export _tortureTestFillBuffer
.proc   _tortureTestFillBuffer
_tortureTestFillBuffer:
        ; remember address of EasyFlash address
        sta zpaddr
        stx zpaddr + 1

        ; get and save address of buffer
        jsr popax
        sta zpbuff
        stx zpbuff + 1

        ; get high-byte of offset
        ldy #3
        lda (zpaddr), y

        ; byte 0..2k-1 => bank number
        cmp #8
        bcs notLT2k

        ; load bank number
        ldy #0
        lda (zpaddr), y
        bcc fillConst

notLT2k:
        ; byte 2k..4k-1 => 0xaa
        cmp #16
        bcs notLT4k

        lda #$aa
        bcc fillConst

notLT4k:
        ; byte 4k..6k-1 => 0x55
        cmp #24
        bcs notLT6k

        lda #$55

fillConst:
        ; fill the buffer with the value in A
        ldy #0
fillConst1:
        sta (zpbuff), y
        iny
        bne fillConst1
        rts

notLT6k:
        ; byte 6k..7k-1 => 0..255
        cmp #28
        bcs notLT7k

        ldy #0
fillInc:
        tya
        sta (zpbuff), y
        iny
        bne fillInc
        rts

notLT7k:
        ; byte 7k..8k-1 => 0..255
        ldy #0
        ldx #255
fillDec:
        txa
        sta (zpbuff), y
        dex
        iny
        bne fillDec
        rts
.endproc

; =============================================================================
;
; Test the banking register: First 0..63 then 63..0
;
; uint16_t __fastcall__ tortureTestBanking(void);
;
; parameters:Refer to the comment in tortureTest.c.
;       -
;
; return:
;       AX  0 for no error, otherwise
;           high byte (X) bank which didn't work, low byte (A) actual bank set
;
; =============================================================================
.export _tortureTestBanking
.proc   _tortureTestBanking
_tortureTestBanking:
        ldx #0
tb:
        stx EASYFLASH_IO_BANK
        cpx $8000
        bne bankError
        inx
        cpx #64
        bne tb

        dex     ; 63
tb2:
        stx EASYFLASH_IO_BANK
        cpx $8000
        bne bankError
        dex
        bpl tb2

        lda #0
        tax
        rts

bankError:
        lda $8000
        rts

.endproc

; =============================================================================
;
; Compare the given buffer (256 bytes) with the test pattern for the flash address
; pointed to.
;
; uint16_t __fastcall__ tortureTestCompare(const uint8_t* pBuffer,
;                                          const EasyFlashAddr* pAddr);
;
; parameters:
;       pAddr in AX
;       address pBuffer on cc65-stack
;
; return:
;       256 for success, errornous offset (0..255) for failure
;
; =============================================================================
.export _tortureTestCompare
.proc   _tortureTestCompare
_tortureTestCompare:
        ; remember address of EasyFlash address
        sta zpaddr
        stx zpaddr + 1

        ; get and save address of buffer
        jsr popax
        sta zpbuff
        stx zpbuff + 1

        ; set the bank
        ldy #0
        lda (zpaddr), y
        sta EASYFLASH_IO_BANK

        ; get the chip number (LOROM/HIROM) and remember it
        iny
        lda (zpaddr), y
        tax
        ; get the offset and put it to another pointer
        iny
        lda (zpaddr), y
        sta zpoffs
        iny
        lda (zpaddr), y ; now we have hi byte of offset in A

        ; calc address for LOROM or HIROM
        cpx #0
        clc
        bne addHIROM
        ; add $8000 to the offset ($0000..$1FFF => $8000..$9FFF)
        adc #$80        ; hi byte of offset still in A
        bne compare     ; always branch
addHIROM:
        ; add $A000 to the offset ($0000..$1FFF => $A000..$BFFF)
        adc #$a0        ; hi byte of offset still in A
compare:
        sta zpoffs + 1
        ldy #0
cmp1:
        lda (zpoffs), y
        cmp (zpbuff), y
        bne different
        iny
        bne cmp1        ; Y = 0

        ; success: return 256
        ldx #1
        tya             ; Y still 0
        rts

different:
        ; return errornous offset 0..255
        tya
        ldx #0
        rts
.endproc