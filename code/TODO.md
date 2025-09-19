# TODO
This is a list of bugs or features that need addressing.

## Bugs
- TODO/NOTE highlighting doesn't work dispite the config.4coder value being enabled
- Scroll Bar doesn't seem to show up when enabled
- `treat_as_code` doesn't allow .odin files to have code rules applied
    - For example line wrapping

## Features
- Light vim keybindings
	[X] jkhl movement
    - Move below line to this line J
	- yank Y yy yiw yaw yt yf
	- Mark setting like visual mode
		- Start Mark at cursor
		- Move cursor to expand selection
		- Commands use "selection" as a target
	- delete D dd diw daw dt df
        - Only did D
	[X] Undo/Redo (u, ctrl-r)
	- Indenting >> <<
	- Replace r C cc ciw caw ct cf s S
        - Only Did C and S
	- Basic commands
		- Write :w
		- Edit :e
		- Substitute :s :%s
		- Go to line # :100 (Go to line 100)
	- Go to top/bottom of file gg G
        - Only implemented G or goto_end_of_file
	[X] Move to Next Empty Line ctrl-{ ctrl-}
	- Leader key <space>
	- Open File browser <leader>of
	- Fuzzy find files <leader><leader>
	- Fuzzy find in files <leader>f
	- Fuzzy find buffers <leader>sb
    - Fuzzy find previously loaded projects <leader>sp
    - Fuzzy find in current buffer <leader>/
    - Fuzzy find keymaps
    - AutoComplete ctrl-space
        - Up/Down in completion menu ctrl-n ctrl-p
	- Change to upper/lower case `
	- Search current Buffer /
		- Next/Previous Search Instance n N
	- Lines between mark and cursor highlighted in Visual Mode
	- Move View Left/Right/Up/Down ctrl-j ctrl-k ctrl-h ctrl-l
        - Almost Done. I can move left and right
	- Split View Vertical/Horizontal <leader>wv <leader>wh
	- Close Current View <leader>wc
    [X] Go to previous file ctrl-6 (I thought of it as ctrl-^ for a long time)
- Toggle Terminal with current project as cwd
	- Create new terminal if one doesn't exist
	- Open existing terminal if one does exist
	- Allow splits (Most likely vertical)
		- If a split is created, it should be a new terminal rather than a copy of the existing terminal
		- Also should toggle on/off with the main terminal
- Open LazyGit
	- Possibly cover entire screen
	- Respect LazyGit shortcuts
- More languages supported in code highlighting
    - Currently only c/c++
