#T_REG - T_REG - T_REG
#

.name           "hello_asm"
.comment        "hello asm"

code:		ld %72,r15
            ld %10,r10
	    ldi  %4, %:code, r10
