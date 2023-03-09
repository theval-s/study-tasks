global _start
section .text
_start:	
	rdrand eax
	mov [num], eax
	;получение случайного числа
;s
	;вывод "Starting number:" 
	mov rax, sys_write
	mov rdi, 1
	mov rsi, startText
	mov rdx, STLen
	syscall
	;pispiskak kak asd 
	;вывод в двоичном виде:
	mov rbx, 31
loopPrint1:
	;проходим по каждому биту умножением, выводим 1 если 1, 0 если 0
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
xor r8, r8
;Перевод строки
	mov rax, sys_write
	mov rdi, 1
	mov rsi, newLine
	mov rdx, 1
	syscall
	
	;отдельно проходим по каждому байту числа
mov rbx, 0
mloop:	
	mov r8b, byte[num+rbx]
	mov ecx, 0
	mov eax, 0
	loopCnt:	
	;считаем количество нулей в байте
		mov rdx, 1
		shl rdx, cl
		and rdx, r8
		jnz nzskip
		inc eax
		nzskip:
		inc ecx
		cmp ecx, 8
		jnz loopCnt
	mov ecx, eax
	;сдвиг вправо на кол-во нулей в байте
	shr r8, cl
	;замена в памяти байта на новый
	mov byte[num+rbx], r8b
	inc rbx
	cmp rbx, 4
	jnz mloop

	
	;Вывод "Result:"
	mov rax, sys_write
	mov rdi, 1
	mov rsi, endText
	mov rdx, ETLen
	syscall
	
	;вывод в двоичной, повторение:
	mov rbx, 31
loopPrint2:
	mov r8d, [num]
	mov rax, 1
	mov rcx, rbx
	shl rax, cl
	dec rbx
	and r8, rax
	jz printZero2
	jmp printOne2
	
printZero2:	mov rax, sys_write
		mov rdi, 1
		mov rsi, zeroC
		mov rdx, 1
		syscall
		cmp rbx, -1
		jnz loopPrint2
		jmp loopEnd2
printOne2:	mov rax, sys_write
		mov rdi, 1
		mov rsi, oneC
		mov rdx, 1
		syscall
		cmp rbx, -1
		jnz loopPrint2
		jmp loopEnd2	
loopEnd2:
	
	;Перевод строки
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
num dd 0xDEADBEEF
startText db 'Starting Num:'
STLen equ $ - startText
endText db 'Result:'
ETLen equ $ - endText
newLine db 10
sys_write equ 1
sys_exit equ 60 
