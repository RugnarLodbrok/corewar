.name           "hello_asm"
.comment        "hello asm"

ld %63,r1
ld %52,r2
ld %10,r10
and r1,r2,r3

#aff r1
#aff r2
aff r3
aff r10
