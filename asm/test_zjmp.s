.name           "hello_asm"
.comment        "hello asm"

    ld %0,r1
    ld %0,r2
    sub r1,r2,r3
    ld %70,r5
    zjmp %:a
    ld %75,r5
a:      st r1,r2
    ld %10,r10

    aff r5
    aff r10
