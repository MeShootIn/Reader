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
  - `Arrow keys` -- to move through the text;
  - `Home` and `End` -- to move to the beginning and end of the line, respectively.

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

## License

[GNU General Public License v3.0](LICENSE)
