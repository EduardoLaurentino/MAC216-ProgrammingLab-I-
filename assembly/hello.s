.LC0:
	.ascii	"Hello, World!\0" #.string "Hello, World!"
	.text
	.globl	main
	#.type	main, @function
main:
#.LFB0:
	pushq   %ebp
        movl    %esp, %ebp
        subl    $8, %esp
        andl    $-16, %esp
        movl    $0, %eax
        movl    %eax, -4(%ebp)
        movl    -4(%ebp), %eax
        call    __alloca
        call    ___main
        movl    $LC0, (%esp)
        call    _printf
        movl    $0, %eax
        leave
        ret
#.LFE0:
#	.size	main, .-main
#	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
#	.section	.note.GNU-stack,"",@progbits
