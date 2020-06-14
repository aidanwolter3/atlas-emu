.segment "CODE"
  main:
    nop
    nop
    nop
    nop

.segment "VECTORS"
  .word main ; $fffa = nmi
  .word main ; $fffc = reset

.segment "HEADER"
.segment "STARTUP"
.segment "CHARS"
.segment "BSS"
