" Vim syntax file
" Language: dobj
" Maintainer Noah Santer
" Latest Revision: 11 May 2015

if exists("b:current_syntax")
    finish
endif

syn keyword functionKeyword function
syn keyword basicKeywords while do if else return yield block fork contained
syn keyword scopeKeyword scope contained

syn match numeric '[a-zA-Z_]\@<![0-9]\+' skipwhite
syn region syscall_region matchgroup=syscall start='\[[a-zA-Z_]\+' end='\]' contains=numeric contained

syn region body start="{" end="}" contains=basicKeywords,scopeKeyword,numeric,body,syscall,syscall_region,comment transparent
syn region string start="\"" end="\""

syn match comment '//.*$'

let b:current_syntax = "blc"

hi def link functionKeyword Type
hi def link basicKeywords Keyword
hi def link scopeKeyword Keyword
hi def link string String
hi def link numeric Constant
hi def link comment Comment
hi def link syscall Special
