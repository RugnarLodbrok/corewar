.name           "Jumper !"
.comment        "hello asm"

start: ld %72,r1
    st r1,0
    ld %0,r2

live:	live	%1
    zjmp %:start
