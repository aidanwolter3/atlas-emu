; This simple program highlights the pretty-printing feature of AtlasEmu by
; executing each addressing mode of LDA.

.segment "HEADER"
  .byte 'N', 'E', 'S', $1a
  .byte 2 ; number of 16k PRG ROM segments
  .byte 0 ; number of 8k CHR ROM segments

.segment "CODE"
  main:
    lda #$ab
    lda $ab
    lda $ab,x
    lda $abcd
    lda $abcd,x
    lda ($ab,x)
    lda ($ab),y
  pass:
    lda #$00
    brk

.segment "VECTORS"
  .word main ; $fffa = nmi
  .word main ; $fffc = reset

.segment "STARTUP"
.segment "CHARS"
.segment "BSS"
