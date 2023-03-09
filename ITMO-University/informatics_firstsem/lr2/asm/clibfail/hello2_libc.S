; Вывод сообщения с помощью функций стандартной
; библиотеки C
;
; nasm -felf64 hello2_libc.S && gcc -no-pie -o hello2_libc hello2_libc.o
; (-no-pie можно заменить на -static)

            global main
            extern printf, exit

            section .text
main:
            mov rdi, fmt
            mov rsi, msg
            mov rax, 0                          ; для printf в rax помещается 
                                                ; количество вещественных аргументов
            call printf

            ;mov rdi, 42
            ;call exit

            ret
            
            section .data
fmt         db      `%s\n`, 0            
msg         db      'Hello, world!', 0
