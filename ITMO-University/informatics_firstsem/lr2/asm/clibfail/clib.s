global _start


section .text
_start:	
;mov r9, 0xDEADBEEF
mov eax, 0x12345678
mov [num], eax
mov rbx, 0
mov r8b, byte[num+rbx]
shr r8, 4
mov byte[num+rbx], r8b
mov rbx, 31
loopPrint1:
	mov r8d, [num]
	mov rax, 1
	mov rcx, rbx
	shl rax, cl
	dec rbx
	and r8, rax
	jz printZero1
	jmp printOne1
	
printZero1:	mov rax, sys_write
		mov rdi, 1
		mov rsi, zeroC
		mov rdx, 1
		syscall
		cmp rbx, -1
		jnz loopPrint1
		jmp loopEnd1
printOne1:	mov rax, sys_write
		mov rdi, 1
		mov rsi, oneC
		mov rdx, 1
		syscall
		cmp rbx, -1
		jnz loopPrint1
		jmp loopEnd1
loopEnd1:	
	mov rax, sys_write
	mov rdi, 1
	mov rsi, newLine
	mov rdx, 1
	syscall
	mov rax, sys_exit 
	xor rdi, rdi
	syscall 

section .data
zeroC db 48
oneC db 49
startText db 'Starting Num:'
STLen equ $ - startText
endText db 'Result:'
ETLen equ $ - endText
newLine db 10
sys_write equ 1
sys_exit equ 60 
num dd 0xDEADBEEF
