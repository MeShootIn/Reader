# Reader

This is a simple text viewer written in WinAPI. Unlike many others, this one can view and iterate through the text with the number of lines greater than `SHRT_MAX` (32767) without lags and crashes.

Initially, it was my course work on the subject "Development of User Interfaces", but then I decided to add the caret functionality.

## Installation

Just clone the repository somewhere:
```cmd
git clone https://github.com/MeShootIn/Reader.git
```

## Usage

Run `bin/Release/Reader.exe` file.

### Movements

Use the following keys:
  - <kbd>Up</kbd>, <kbd>Down</kbd>, <kbd>Left</kbd>, <kbd>Right</kbd> -- to move through the text;
  - <kbd>Home</kbd> and <kbd>End</kbd> -- to move to the beginning and end of the line, respectively;
  - <kbd>Page Up</kbd> and <kbd>Page Down</kbd> -- to scroll the page up and down respectively.

### Menu bar

The following menu items are present:
  - `File`:
    - `Open...` -- opens a file;
    - `Close` -- closes the currently viewed file;
    - `Quit` -- `Close` + exits the program.
  - `View`:
    - [ ] `Layout mode` -- enables line wrapping to eliminate the need of scrolling horizontally in order to see overly long lines;
    - [x] `Non-layout mode` -- disables line wrapping (checked by default).
  - `Help`:
    - `About` -- shows information about the program.

### Test

To test this program, you can try to view the files from the directory "test".

## License

[GNU General Public License v3.0](LICENSE)
