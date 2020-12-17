.segment "HEADER"
  .byte 'N', 'E', 'S', $1a
  .byte 2 ; number of 16k PRG ROM segments
  .byte 0 ; number of 8k CHR ROM segments

.segment "CODE"

reset_nametable:
  ; fill the nametable with zeros.
  lda #$20
  sta $2006
  lda #$00
  sta $2006
  lda #$00
  ldy #$04
  :
    ldx #$00
    :
      sta $2007
      inx
      bne :-
    dey
    bne :--
  rts

draw_borders:
  ; fill top row with AA.
  lda #$20
  sta $2006
  lda #$00
  sta $2006
  ldy #$20
  lda #$AA
  :
    sta $2007
    dey
    bne :-

  ; set the ppu address direction to downwards.
  lda #$04
  sta $2000

  ; fill the left column with BB.
  lda #$20
  sta $2006
  lda #$20
  sta $2006
  ldy #$1D
  lda #$BB
  :
    sta $2007
    dey
    bne :-
  rts

verify_nametable:
  ; set the ppu address direction to right.
  lda #$00
  sta $2000

  lda #$20
  sta $2006
  lda #$00
  sta $2006

  ldy #$00
  loop_y:
    ldx #$00
    loop_x:
        ; check if we are done (03C0)
        cpy #$03
        bne verify_next
        cpx #$3C
        bne verify_next
        jmp pass
      verify_next:
        cpy #$00
        bne maybe_verify_bb
        cpx #$20
        bcc verify_aa ; < 0x20
      maybe_verify_bb:
        txa
        and #$1F
        beq verify_bb ; % 0x20 == 0
        ; fallthrough
      verify_00:
        lda $2007
        beq continue
        jmp fail
      verify_aa:
        lda $2007
        cmp #$AA
        beq continue
        jmp fail
      verify_bb:
        lda $2007
        cmp #$BB
        beq continue
        jmp fail
      continue:
        inx
        bne loop_x
    iny
    cpy #$04
    bne loop_y

  ; we never expect to get here
  jmp fail

main:
  jsr reset_nametable
  jsr draw_borders
  jsr verify_nametable

  ; we never expect to get here
  jmp fail

pass:
  lda #$00
  brk
fail:
  lda #$01
  brk

.segment "VECTORS"
  .word main ; $fffa = nmi
  .word main ; $fffc = reset

.segment "STARTUP"
.segment "CHARS"
.segment "BSS"
