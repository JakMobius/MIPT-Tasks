
global __ZNK10hash_table8get_pairEPKc
extern   _string_hash
extern  _memcmp
section  .text

;_string_hash:
;          xor rax, rax
;          crc32 rax, qword [rdi]
;          crc32 rax, qword [rdi + 8]
;          crc32 rax, qword [rdi + 16]
;          crc32 rax, qword [rdi + 24]
;          crc32 rax, qword [rdi + 32]
;          crc32 rax, qword [rdi + 40]
;          crc32 rax, qword [rdi + 48]
;          crc32 rax, qword [rdi + 56]
;          ret

__ZNK10hash_table8get_pairEPKc:
		; Saving registers
		    push    rbp
        push    r15
        push    r14
        push    r13
        push    r12
        push	  rbx
        push    rax ; stack aligmnent, push is faster than sub rsp, 8
        ; Preparing string_hash call
		; Optimised excessive memory accesses here
		; Local variables are now stored in registers
        mov     r12, rdx
        mov     r15, rsi
        mov     qword [rsp], rdi
        mov     rdi, rdx
        ; Calculating string hash
        call    _string_hash
        ; Dividing string hash by capacity
        movsxd  rcx, dword [r15] 	  ; rcx = this->capacity
        xor     edx, edx
        div     rcx 					  ; edx = chain_index
  	    mov     rcx, qword [r15 + 16] ; rcx = this->lists

  	    ; chain_index *= sizeof(s_list_entry)
        lea     rdx, [rdx + 2*rdx]		  ; chain_index *= 3
        shl     rdx, 4					  ; chain_index *= 16
        mov     ebp, dword [rcx + rdx + 28] ; ebp = lists[chain_index]->head
        mov     eax, dword [rcx + rdx + 24] ; eax = lists[chain_index]->tail
        ; if(eax == ebp), don't jump in the cycle
        cmp     eax, ebp
        je      .return_zero

        mov     r13, qword [rcx + rdx]
        mov     r15, qword [rcx + rdx + 16]
.cycle:
		; comparing keys
		; rbx = walker
        movsxd  rbx, eax
        ; offset = walker * 8 * 3 = r14 * 8
        lea     r14, [rbx + 2*rbx]
        ; rdi = lists->entries[walker]
        mov     rdi, qword [r13 + 8*r14]
        mov     edx, 64
        mov     rsi, r12
        call    _memcmp
        test    eax, eax
        je      .return_found_entry
        mov     eax, dword [r15 + 4*rbx]
        cmp     eax, ebp
        jne     .cycle
.return_zero:
		; xmm0 = 0
        xorps   xmm0, xmm0
        ; rax = returned structure address
        mov     rax, qword [rsp]
        ; writing zeroes in result
        movups  [rax], xmm0
        mov     qword [rax + 16], 0
        jmp     .return
.return_found_entry:
		; rdx = offset of pair
        lea     rdx, [8*r14]
        add     rdx, r13
        ; reading .hash field
        mov     rcx, qword [rdx + 16]
        ; rax = returned stucture address
        mov     rax, qword [rsp]
        ; writing .hash field
        mov     qword [rax + 16], rcx
        movups  xmm0, [rdx]
        ; writing .key and .value field
        movups  [rax], xmm0
.return:
		; restoring stack
		    add     rsp, 8
		    pop		  rbx
        pop     r12
        pop     r13
        pop     r14
        pop     r15
        pop     rbp
        ret