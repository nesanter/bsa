" Vim syntax file
" Language: mu
" Maintainer Noah Santer
" Latest Revision: 1 Sep 2015

if exists("b:current_syntax")
    finish
endif

syn match constantKeyword '\<constant\>' skipwhite nextgroup=constantIdent
syn match constantIdent "[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*=" skipwhite nextgroup=@numeric contained

syn match functionKeyword 'function' skipwhite nextgroup=attribute
syn keyword basicKeywords while do if else yield fork sync sync_r sync_w contained
syn keyword opKeywords and or xor not is !is
syn keyword scopeKeyword scope contained
syn keyword constKeyword true false

syn match numeric10 '\<\(0d\)\?[0-9]\+\>' skipwhite contained
syn match numeric2 '\<0b[01]\+\>' skipwhite contained
syn match numeric8 '\<0o[0-7]\+\>' skipwhite contained
syn match numeric16 '\<0x[0-9a-fA-F]\+\>' skipwhite contained
syn cluster numeric contains=numeric10,numeric2,numeric8,numeric16
syn match char '\'\(\(.\)\|\(\\[rnte0]\)\)\'' contains=escapedChar
syn match attribute '@[a-zA-Z]\+' contained
syn region syscall_region matchgroup=syscall start='\[[a-zA-Z_]\+' end='\]' contains=@numeric,char,string,opKeywords,constKeyword,syscall_region contained

syn region body start="{" end="}" contains=basicKeywords,opKeywords,scopeKeyword,@numeric,char,constKeyword,body,syscall,syscall_region,comment transparent
syn region string start="\"" end="\"" contains=escapedChar

" syn match escapedChar '\(\\\@<!\\[rnte0]\)\|\(\\\@<!\\x[0-9a-fA-F][0-9a-fA-F]\)' contained
syn match escapedChar '\(\\\\\)\|\(\\[rnte0]\)\|\(\\x[0-9a-fA-F]\{2\}\)' contained

syn match comment '//.*$'

let b:current_syntax = "mu"

hi def link constantKeyword Type
hi def link functionKeyword Type
hi def link basicKeywords Keyword
hi def link scopeKeyword Keyword
hi Operator gui=italic cterm=italic
hi def link opKeywords Operator
hi def link constKeyword Constant
hi def link string String
hi escapedChar gui=bold guifg=magenta cterm=bold ctermfg=magenta
hi def link numeric10 Constant
hi def link numeric2 Constant
hi def link numeric8 Constant
hi def link numeric16 Constant
hi def link char Constant
hi def link comment Comment
hi def link syscall Special
hi def link attribute Identifier

syn sync fromstart
