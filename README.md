# Trident Editor
A free terminal text editor for Linux

<img src="pic/img.png"/>

## Compiling and running
Compile Trident Editor by running the compile file as bash: 
```sh
bash compile
```
then run tr-ed:
```
tr-ed
```

## Customizing
Trident Editor settings are stored under `~/.config/tr-ed/settings.scbl` (if the tr-ed folder or settings.scbl file dont exist, Trident Editor will create them when you run it).
The language used for the settings is SCBL with a very simple syntax: `<function> <parameters>;`.

To change a setting, type in `set` and the option after that. List of options:
- tabsize (NOT DONE!): The amount of spaces the tab character is converted to
- color: The color of the specified component

> Example: `set tabsize 4`

### color
The color option wants a few more parameters, first one being one of the listed below:
- background: The editor backround color
- foreground: The editor text color
- frame: The editor frame foreground color
- keyword (NOT DONE!): Language keywords highlight color
- string (NOT DONE!): Strings highlight color
- number (NOT DONE!): Numbers highlight color
- boolean (NOT DONE!): Booleans highlight color
- datatype (NOT DONE!): Language data types highlight color
- std (NOT DONE!): Standard language functions/namespaces highlight color
- comment (NOT DONE!): Comment highlight color
- prerpoc (NOT DONE!): Pre-processor directives

The next parameter will be the color. List of colors:
- black
- red
- green
- yellow
- blue
- magenta
- cyan
- white
- grey
- brightred
- brightgreen
- brightyellow
- brightblue
- brightmagenta
- brightcyan
- brightyellow

> Example: `set color background black`

## To do
- Selection
- Copy/Cut/Paste
- Syntax highlighting
- Line counting
- Scroll bar