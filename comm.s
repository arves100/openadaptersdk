	.area _CODE

_SendPrinterByte::
        di
	lda	hl,2(sp)
	ld	a,(hl)
        ldh     (0x01),a
        ld      a,#0x81
        ldh     (0x02),a
sent:   ldh     a,(0x02)
	and	#0x80
	jp	NZ,sent
        ldh     a,(0x01)
	ld	e,a
        ld      d,#0x00
        ei
	ret

_GetHigh::
        lda     hl,2(sp)
        inc     hl
        ld      a,(hl)
        ld      e,a
        ld      d,#0x00
        ret
_GetLow::
        lda     hl,2(sp)
        ld      a,(hl)
        ld      e,a
        ld      d,#0x00
        ret
