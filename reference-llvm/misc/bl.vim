" Vim syntax file
" Language: dobj
" Maintainer Noah Santer
" Latest Revision: 11 May 2015

if exists("b:current_syntax")
    finish
endif

syn match functionKeyword 'function' skipwhite nextgroup=attribute
syn keyword basicKeywords while do if else yield fork sync sync_r sync_w contained
syn keyword opKeywords and or xor not is !is
syn keyword scopeKeyword scope contained
syn keyword constKeyword true false

syn match numeric '[a-zA-Z_]\@<![0-9]\+' skipwhite
syn match char '\'\(\(.\)\|\(\\[rnte0]\)\)\'' contains=escapedChar
syn match attribute '@[a-zA-Z]\+' contained
syn region syscall_region matchgroup=syscall start='\[[a-zA-Z_]\+' end='\]' contains=numeric,char,string,opKeywords,constKeyword,syscall_region contained

syn region body start="{" end="}" contains=basicKeywords,opKeywords,scopeKeyword,numeric,char,constKeyword,body,syscall,syscall_region,comment transparent
syn region string start="\"" end="\"" contains=escapedChar

syn match escapedChar '\\\@<!\\[rnte0]' contained

syn match comment '//.*$'

let b:current_syntax = "blc"

hi def link functionKeyword Type
hi def link basicKeywords Keyword
hi def link scopeKeyword Keyword
hi Operator gui=italic
hi def link opKeywords Operator
hi def link constKeyword Constant
hi def link string String
hi escapedChar gui=bold guifg=magenta
hi def link numeric Constant
hi def link char Constant
hi def link comment Comment
hi def link syscall Special
hi def link attribute Identifier

syn sync fromstart
