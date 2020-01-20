.name           "marcher"
.comment        "marcher"

start:
    ld %0, r2 ; let i = 0;
    ld %4, r15 ; const r15 = 4
    ld %64, r14 ; const r14 = 64
    ld %4294967295, r13 ; const r13 = 0xFFFFFFFF
    sti r1, %:live, %1
loop:
    ldi %:start, r2, r3
    sti r3, %:end, r2
    add r2, r15, r2 ; r2 += 4
    sub r2, r14, r16 ; if r 16 = r2 - r14
    xor r16, r13, r16 ; r16 = !r16
    zjump %:loop
live:
    live %0
    xor %0, %0, r16
    zjmp %:start
end:
    arr %51
