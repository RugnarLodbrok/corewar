.name           "loop"
.comment        "loop"

loop:
    sti r1, %:live, %1
    xor %0, %0, r16
live:
    live %0
    zjmp %:loop
