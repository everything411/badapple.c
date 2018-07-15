.386
.model flat, stdcall
option casemap :none
include windows.inc
include kernel32.inc
include masm32.inc
includelib kernel32.lib
includelib masm32.lib
.data
FileName db 'bad_apple.txt',0
RunMpg123 db 'mpg123 -q bad_apple.mp3',0
.data?
hINSTANCE HINSTANCE ?
NotEOF dd ?
buffer db 10000 dup(?)
.CODE
START:
	invoke CreateFile,addr FileName,GENERIC_READ,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL
	mov hINSTANCE,eax
	.if hINSTANCE==INVALID_HANDLE_VALUE
		invoke ExitProcess,1
	.endif
	invoke WinExec,addr RunMpg123,SW_NORMAL
	.while 1
		invoke ReadFile,hINSTANCE,addr buffer,9662,addr NotEOF,NULL
		.if !NotEOF
			invoke CloseHandle,hINSTANCE
			invoke ExitProcess,0
		.endif
		invoke locate,0,0
		invoke StdOut,addr buffer
		invoke Sleep,30
	.endw
end START