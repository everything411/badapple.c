;#Mode=CON
.386
.model flat, stdcall
option casemap :none
include windows.inc
include masm32.inc
include kernel32.inc
includelib kernel32.lib
includelib masm32.lib
.const
TextName db 'bad_apple.txt',0
.data?
hINSTANCE HINSTANCE ?
File_Not_End dd ?
buffer db 10000 dup(?)
.CODE
START:
	invoke CreateFile,addr TextName,GENERIC_READ,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
	mov hINSTANCE,eax
	.if hINSTANCE==INVALID_HANDLE_VALUE
		invoke ExitProcess,1
	.endif
	.while 1
		invoke ReadFile,hINSTANCE,addr buffer,9662,addr File_Not_End,NULL
		.if !File_Not_End
			invoke CloseHandle,hINSTANCE
			invoke ExitProcess,0
		.endif
		invoke locate,0,0
		invoke StdOut,addr buffer
		invoke Sleep,30
	.endw
	
end START