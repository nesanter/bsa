" Vim syntax file
" Language: dobj
" Maintainer Noah Santer
" Latest Revision: 11 May 2015

if exists("b:current_syntax")
    finish
endif

syn keyword basicKeywords while if else return yield block fork scope contained

syn match qualifier '[a-zA-Z][a-zA-Z0-9_]*::' nextgroup=ident contained
syn match ident '[a-zA-Z][a-zA-Z0-9_]*\(\.[a-zA-Z][a-zA-Z0-9_]*)*\(\.\*)\?' contained

syn region defineBlock start="define" end=";" contains=basicKeywords,ident,qualifier,expr,code

syn region expr start="(" end=")" contained
syn region code start="{" end="}" contained

let b:current_syntax = "dobj"

hi def link basicKeywords Keyword
hi def link qualifier Type
hi def link ident Identifier
hi def link expr Constant
hi def link code Constant

