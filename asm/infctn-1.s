.name           "infctn-1"
.comment        "hello asm"

init: ld %72,r1
    ld %69,r2
    ld %76,r3
    ld %80,r4
    ld %10,r10
    st r6,0

start:  st r1,r5
    st r2,r5
    st r3,r5
    st r4,r5

print: aff r1
    aff r2
    aff r3
    aff r4
    aff r10
    zjmp %:start
