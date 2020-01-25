.name           "marcher"
.comment        "marcher"

; r2 = iterator
; r3 = memory chunk
; r14 = size of all code to copy
; r16 = /dev/null
init:
    ld %:end, r16
    ld %:start, r15
    sub r16, r15, r14 ; r14 = size of all code to copy
    ld %4, r15 ; const r15 = 4
    ld %64, r14 ; const r14 = 64
    sti r1, %:live, %1
start:
    ld %0, r2 ; r2 = iterator
loop:
    ldi %:start, r2, r3 ;r3 = memory chunk
    sti r3, %:end, r2
    live:
        live %0
    add r2, r15, r2 ; r2 += 4
    sub r2, r14, r16 ; if r 16 = r2 - r14
    zjmp %:end  ;break
    xor r13, r13, r16 ;break
    zjmp %:loop
end:
    aff r16
