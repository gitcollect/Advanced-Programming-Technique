set nu
colorscheme monokai
syntax on
"set cul
"set cuc
"set ruler
set showcmd
set nocompatible
set autoindent
set tabstop=4
"set softtapstop=4
"set cmdhight=2
set magic
set noeb
set ignorecase
set mouse=a
set selectmode=mouse,key
set showmatch
set matchtime=1
let Tlist_Auto_Open=0
let g:miniBufExplMapWindowNavVim = 1 
let g:miniBufExplMapWindowNavArrows = 1 
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplModSelTarget = 2
"autocmd vimenter * NERDTree
if has("autocmd")
      autocmd BufReadPost *
	  	\ if line("'\"") > 0 && line("'\"") <= line("$") |
        \   exe "normal g`\"" |
		\ endif
endif
"当打开vim且没有文件时自动打开NERDTree"
autocmd vimenter * if !argc() | NERDTree  | endif
"" 只剩 NERDTree时自动关闭
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTreeType") && b:NERDTreeType == "primary") | q | endif
