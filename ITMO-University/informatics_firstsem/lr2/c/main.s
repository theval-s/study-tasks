	.file	"main.c"
	.intel_syntax noprefix
	.text
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	printBinary
	.type	printBinary, @function
printBinary:
.LFB6:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 176
	mov	DWORD PTR -164[rbp], edi
	mov	rax, QWORD PTR fs:40
	mov	QWORD PTR -8[rbp], rax
	xor	eax, eax
	lea	rdx, -144[rbp]
	mov	eax, 0
	mov	ecx, 16
	mov	rdi, rdx
	rep stosq
	mov	DWORD PTR -148[rbp], 0
	jmp	.L2
.L3:
	mov	eax, DWORD PTR -164[rbp]
	and	eax, 1
	mov	edx, eax
	mov	eax, DWORD PTR -148[rbp]
	cdqe
	mov	DWORD PTR -144[rbp+rax*4], edx
	mov	eax, DWORD PTR -164[rbp]
	shr	eax
	mov	DWORD PTR -164[rbp], eax
	add	DWORD PTR -148[rbp], 1
.L2:
	cmp	DWORD PTR -164[rbp], 0
	jne	.L3
	mov	DWORD PTR -148[rbp], 32
	jmp	.L4
.L5:
	mov	eax, DWORD PTR -148[rbp]
	cdqe
	mov	eax, DWORD PTR -144[rbp+rax*4]
	mov	esi, eax
	lea	rax, .LC0[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	sub	DWORD PTR -148[rbp], 1
.L4:
	cmp	DWORD PTR -148[rbp], 0
	jns	.L5
	mov	edi, 10
	call	putchar@PLT
	nop
	mov	rax, QWORD PTR -8[rbp]
	sub	rax, QWORD PTR fs:40
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	printBinary, .-printBinary
	.section	.rodata
	.align 8
.LC1:
	.string	"Original Number: %x\nBinary form: "
.LC2:
	.string	"Result: %x\nBinary form: "
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 32
	mov	DWORD PTR -24[rbp], -559038737
	mov	eax, DWORD PTR -24[rbp]
	mov	esi, eax
	lea	rax, .LC1[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, DWORD PTR -24[rbp]
	mov	edi, eax
	call	printBinary
	mov	DWORD PTR -20[rbp], 3
	jmp	.L8
.L11:
	mov	eax, DWORD PTR -24[rbp]
	mov	DWORD PTR -12[rbp], eax
	mov	eax, DWORD PTR -20[rbp]
	sal	eax, 3
	mov	edx, DWORD PTR -12[rbp]
	mov	ecx, eax
	sal	edx, cl
	mov	eax, edx
	shr	eax, 24
	mov	DWORD PTR -12[rbp], eax
	mov	WORD PTR -26[rbp], 0
	mov	DWORD PTR -16[rbp], 0
	jmp	.L9
.L10:
	mov	eax, DWORD PTR -16[rbp]
	mov	edx, 1
	mov	ecx, eax
	sal	edx, cl
	mov	eax, edx
	mov	DWORD PTR -4[rbp], eax
	mov	eax, DWORD PTR -4[rbp]
	and	eax, DWORD PTR -12[rbp]
	test	eax, eax
	sete	al
	movzx	edx, al
	movzx	eax, WORD PTR -26[rbp]
	add	eax, edx
	mov	WORD PTR -26[rbp], ax
	add	DWORD PTR -16[rbp], 1
.L9:
	cmp	DWORD PTR -16[rbp], 7
	jle	.L10
	movsx	eax, WORD PTR -26[rbp]
	mov	ecx, eax
	shr	DWORD PTR -12[rbp], cl
	mov	eax, 3
	sub	eax, DWORD PTR -20[rbp]
	sal	eax, 3
	mov	ecx, eax
	sal	DWORD PTR -12[rbp], cl
	mov	eax, 3
	sub	eax, DWORD PTR -20[rbp]
	sal	eax, 3
	mov	edx, 255
	mov	ecx, eax
	sal	edx, cl
	mov	eax, edx
	mov	DWORD PTR -8[rbp], eax
	mov	eax, DWORD PTR -8[rbp]
	not	eax
	and	DWORD PTR -24[rbp], eax
	mov	eax, DWORD PTR -24[rbp]
	or	eax, DWORD PTR -12[rbp]
	mov	DWORD PTR -24[rbp], eax
	sub	DWORD PTR -20[rbp], 1
.L8:
	cmp	DWORD PTR -20[rbp], 0
	jns	.L11
	mov	eax, DWORD PTR -24[rbp]
	mov	esi, eax
	lea	rax, .LC2[rip]
	mov	rdi, rax
	mov	eax, 0
	call	printf@PLT
	mov	eax, DWORD PTR -24[rbp]
	mov	edi, eax
	call	printBinary
	mov	eax, 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-7ubuntu2) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
