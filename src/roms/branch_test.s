.segment "HEADER"
  .byte 'N', 'E', 'S', $1a
  .byte 2 ; number of 16k PRG ROM segments
  .byte 0 ; number of 8k CHR ROM segments

.segment "CODE"
  main:
    nop
    nop
    nop
    nop
    lda #$00
    beq pass
  fail:
    lda #01
    brk
  pass:
    lda #00
    brk

.segment "VECTORS"
  .word main ; $fffa = nmi
  .word main ; $fffc = reset

.segment "STARTUP"
.segment "CHARS"
.segment "BSS"
