%include 'io.inc'
global _start
	
	section .text
_start:
		mov r12, 0
	.loop:
		xor rdi, rdi
		mov dil, byte[val+r12]
		call print_uint

		inc r12

		cmp r12, 8
		jl .loop

		mov rdi, 0
		call exit

	section .data
val:	dq 0x1112131415161718
