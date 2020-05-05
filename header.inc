;we use a variable in memeory named noob(dq) and newb(db) for all functions operation to be easy

	section .data
noob: dq 0
newb: db 0


exit:	mov rax, 60
		syscall
		ret


string_length:
		mov r8, 0

	.loop:
		mov r9b, [rdi+r8]

		inc r8

		cmp r9b, 0
		jnz .loop

		sub r8, 1
		mov rax, r8
		ret

print_string:
		mov r8, 0
	.loop:

		mov r9b, [rdi+r8]
		
		cmp r9b, 0
		je .end
		
		mov r10, rdi
		add r10, r8

		inc r8
		push rdi

		mov rax, 1
		mov rdi, 1
		mov rdx, 1
		mov rsi, r10
		syscall

		pop rdi
		jmp .loop
	.end:
		call print_newline
		ret

print_uint:
		mov rax, rdi	;dividend must be in rax by default
		mov r11, 10		;moving 10 to register
		mov r10, 0		;r9 is used as counter
	
	.loop1:
		mov rdx, 0
		div r11			;remainder to be pushed to stack
		push rdx

		inc r10
		cmp rax, 0		;
	jne .loop1

	.loop2:
		pop r8		;popping to get original number
		add r8 , '0'	;add for making ascii value of digit
		mov [noob], r8

						;print the digit to stdout
		mov rax, 1
		mov rdi, 1
		mov rdx, 1
		mov rsi, noob
		syscall
		
		dec r10
		test r10, r10		;loop continues until r9 == 0
		jnz .loop2

		call print_newline
		ret 

print_newline:
		mov qword[noob], 10
		mov rax, 1
		mov rdi, 1
		mov rdx, 1
		mov rsi, noob
		syscall
		ret

print_char:
		mov byte[newb], dil

		mov rsi, newb
		mov rax, 1
		mov rdi, 1
		mov rdx, 1
		syscall
		call print_newline
		ret

print_int:
		cmp rdi, 0
		jge .else
		xor rdi, 0xffffffffffffffff
		add rdi, 1	
		push rdi

		mov byte[newb], '-'
		mov rsi, newb
		mov rax, 1
		mov rdi, 1
		mov rdx, 1
		syscall

		pop rdi
		.else:
		
		call print_uint
		ret

parse_uint:				;returns uint in rax and digit count in rdx
		mov r10, 0
		mov r8, 0
		mov rax, 0
	.loop:
		mov r8b, [rdi+r10]
	
		cmp r8, 0
		je .end

		mov r9, 10
		mul r9

		sub r8b, '0'
		add rax, r8

		inc r10
		
		jmp .loop
	.end:

		mov rdx, r10
		ret

parse_int:				;returns int in rax, digits in rdx
		mov r8, 0
		mov r8b, [rdi]

		cmp r8b, '-'
		je .neg
		
		call parse_uint
		ret

	.neg:

		lea rdi, [rdi+1]
			
		call parse_uint 	

		inc rdx
		xor rax, 0xffffffffffffffff
		inc rax
		ret

string_equals:				;returns 0 or 1 in rax
		mov r8, 0
		mov r9, 0
		mov r10, 0
	.loop: 
		mov r8b, [rdi+r10]
		mov r9b, [rsi+r10]
		inc r10
		cmp r8b, r9b
		je  .elif

		mov rax, 0
		ret

	.elif:
		cmp r8b, 0
		jne .loop

		mov rax, 1
		ret

read_char:					;returns charecter read in char
		push 0

		mov rax, 0
		mov rdi, 0
		mov rsi, rsp
		mov rdx, 1
		syscall

		pop rax
		ret

string_copy:				;returns buffer address or 0 in rax
		call string_length
		cmp rax, rdx
		jge .big

		mov r8, 0
		mov r9, 0
	.loop:
		mov r8b, [rdi+r9]
		mov [rsi+r9], r8b

		inc r9

		cmp r8b, 0
		je .end

		jmp .loop

	.end:
		mov rax, rsi
		ret

	.big:
		mov rax, 0
		ret

read_word:					;returns buffer address or 0 in rax
		mov r8, rdi
		mov r9, rsi
		mov r10, 0
	
	.star:

		call read_char

		cmp rax, ' '
		je .star

		mov [r8+r10], rax
		inc r10

		cmp r10, r9
		jle .cont
		mov rax, 0
		ret

	.cont:
		cmp rax, 0
		je .end
		cmp rax, 10
		je .end

		jmp .star

	.end:
		dec r10
		mov byte[r8+r10], 0
		mov rax, r8
		ret