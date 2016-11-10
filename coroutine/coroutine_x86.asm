.MODEL FLAT, C
OPTION casemap:none

.CODE

;-----------------------------
; return eip value
;-----------------------------
experimental_coroutine_get_instruction_pointer PROC
	mov eax, [esp]
	ret
experimental_coroutine_get_instruction_pointer ENDP

;-----------------------------
; return esp value
;-----------------------------
experimental_coroutine_get_stack_pointer PROC
	mov eax, esp
	ret
experimental_coroutine_get_stack_pointer ENDP

;-----------------------------
; save stack to memory
;-----------------------------
STACKHDR STRUCT
 lsize	DWORD	?
 laddr	DWORD	?
 lrebp	DWORD   ?
 r_eax	DWORD	?
 r_ebx	DWORD	?
 r_ecx	DWORD	?
 r_edx	DWORD	?
 r_esi	DWORD	?
 r_edi	DWORD	?
STACKHDR ENDS

experimental_coroutine_save_stack PROC
	; save ebp
	push ebp
	mov eax, ebp

	; ebp will address output stack data
	mov ebp, [esp+16]

	; save registers
	mov ( STACKHDR PTR [ebp] ).r_eax, eax
	mov ( STACKHDR PTR [ebp] ).r_ebx, ebx
	mov ( STACKHDR PTR [ebp] ).r_ecx, ecx
	mov ( STACKHDR PTR [ebp] ).r_edx, edx
	mov ( STACKHDR PTR [ebp] ).r_esi, esi
	mov ( STACKHDR PTR [ebp] ).r_edi, edi

	; save relative ebp
	sub eax, [esp+8]
	mov ( STACKHDR PTR [ebp] ).lrebp, eax

	; calculate stack size
	push ecx
	mov ecx, [esp+12]
	sub ecx, esp
	sub ecx, 4		; ignore pushed ebp and ecx, but account for esp itself
	mov ( STACKHDR PTR [ebp] ).lsize, ecx
	
	; check stack size
	push ecx
	add ecx, SIZEOF STACKHDR
	sub ecx, [esp+20]
	jns @@2
	pop ecx

	; save stack data
	push esi
	mov esi, [esp+16]
	add ebp, SIZEOF STACKHDR
	shr ecx, 2
@@1:
	mov eax, [esi]
	mov [ebp], eax
	sub esi, 4
	add ebp, 4
	loop @@1
	pop esi
	pop ecx

	; finally, save instruction address
	mov ebp, [esp+16]
	call experimental_coroutine_get_instruction_pointer
	mov ( STACKHDR PTR [ebp] ).laddr, eax
	pop ebp
	ret
@@2:
	mov ( STACKHDR PTR [ebp] ).lsize, 0
	pop ecx
	pop ecx
	pop ebp
	ret
experimental_coroutine_save_stack ENDP

;-----------------------------
; restore stack from memory
;-----------------------------
experimental_coroutine_restore_stack PROC
	; ebp will address input stack data
	mov ebp, [esp+8]

	; edi will adress stack base
	mov esi, [esp+4]

	; restore stack data
	; esp is now modified!
	mov ecx, ( STACKHDR PTR [ebp] ).lsize
	mov esp, esi
	sub esp, ecx
	add esp, 4
	mov edi, esi
	mov edx, ebp
	add edx, SIZEOF STACKHDR
	shr ecx, 2
@@1:
	mov eax, [edx]
	mov [edi], eax
	sub edi, 4
	add edx, 4
	loop @@1

	; restore stack frame (ebp) and push to the new stack
	mov edi, ( STACKHDR PTR [ebp] ).lrebp
	add edi, esi
	push edi

	; restore registers
	mov eax, ( STACKHDR PTR [ebp] ).r_eax
	mov ebx, ( STACKHDR PTR [ebp] ).r_ebx
	mov ecx, ( STACKHDR PTR [ebp] ).r_ecx
	mov edx, ( STACKHDR PTR [ebp] ).r_edx
	mov esi, ( STACKHDR PTR [ebp] ).r_esi
	mov edi, ( STACKHDR PTR [ebp] ).r_edi

	; ebp is ok, and real ebp value is on the stack
	; we are ready to jump
	push ( STACKHDR PTR [ebp] ).laddr
	ret
experimental_coroutine_restore_stack ENDP

END
