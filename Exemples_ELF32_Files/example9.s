.global main
.text
main:
    mov r1, #0x80
    mov r2, #0x10
    mov r3, #0
test:
	add r3, #1
	cmp r1, r2
    bne different
    beq fin

different:
	sub r1, r1, r2
	b test

fin:
	mov r4, r3
