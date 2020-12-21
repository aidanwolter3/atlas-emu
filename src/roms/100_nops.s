.segment "HEADER"
  .byte 'N', 'E', 'S', $1a
  .byte 2 ; number of 16k PRG ROM segments
  .byte 0 ; number of 8k CHR ROM segments

.segment "CODE"
  main:
    .repeat 100
    nop
    .endrep
  pass:
    lda #$00
    brk

.segment "VECTORS"
  .word main ; $fffa = nmi
  .word main ; $fffc = reset

.segment "STARTUP"
.segment "CHARS"
.segment "BSS"
