.name           "hello_asm"
.comment        "hello asm"

ld %50,r1
ld %1,r2
ld %10,r10
add r1,r2,r3

#aff r1
#aff r2
aff r3
aff r10
