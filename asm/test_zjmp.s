.name           "hello_asm"
.comment        "hello asm"

    ld %72,r1
    ld %0,r3
    ld %0,r4
    sub r3,r4,r5
    zjmp %:a
    ld %0,r1
a:      st r1,r2
    ld %10,r10

    aff r1
    aff r2
    aff r10
