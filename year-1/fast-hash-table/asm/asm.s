global   _infinite_loop
global _bebe
section  .text
_infinite_loop:
loop:
    mov      rax, 0x02000004
    mov      rdi, 1
    mov      rsi, msg
    mov      rdx, 13
    syscall
    
    jmp loop
    mov      rax, 0x02000001
    xor      rdi, rdi
    syscall

_bebe:
push rbp
   mov rbp, rsp
   sub rsp, 24
   mov rax, rdi
   mov rcx, 2
   cmp rax, rcx
   jge L0
   mov rax, 1
   add rsp, 24
   pop rbp
   ret
L0:
   mov rcx, rdi
   mov rdx, 1
   sub rcx, rdx
   mov rcx, rdi
   mov rdi, rcx
   mov [rbp - 8], rcx
   call _bebe
   mov [rbp - 16], rbx
   mov rbx, rax
   mov [rbp - 24], r12
   mov r12, [rbp - 8]
   mov [rbp - 8], r13
   mov r13, 2
   sub r12, r13
   mov rdi, r12
   call _bebe
   mov r11, rax
   add rbx, r11
   mov rax, rbx
   mov rbx, [rbp - 16]
   mov r12, [rbp - 24]
   mov r13, [rbp - 8]
   add rsp, 24
   pop rbp
   ret

section .data
    msg:    db       "Hello world!", 10
