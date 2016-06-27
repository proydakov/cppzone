.code

LogicAND proc
	mov eax, ecx
	and eax, edx
	ret
LogicAND endp

LogicOR proc
	mov eax, ecx
	or  eax, edx
	ret
LogicOR endp

LogicXOR proc
	mov eax, ecx
	xor eax, edx
	ret
LogicXOR endp

end
