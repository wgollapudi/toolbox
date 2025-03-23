# Thanos

**Thanos** command‑line tool that randomly deletes a specified number or percentage of files from a directory.

---

## Overview

Thanos allows you to:

- **Delete Files by Number or Percentage:**
  Choose to delete a fixed number of files using `-n` or a percentage of matching files using `-p` (these options are mutually exclusive).

- **Optional Filters:**
  - `-d <directory>`: Specify the target directory (defaults to the current directory).
  - `-m <regex>`: Only consider files whose names (or relative paths when using recursive mode) match the provided regular expression.
  - `-r`: Process files recursively in all subdirectories.

- **Safety Features:**
  - `--dry-run`: Display which files would be deleted without actually deleting them.
  - `--confirm`: Require confirmation before deleting each file.

- **No Arguments:**
  If no arguments are passed, Thanos displays a cool piece of ASCII art as an homage.

---

## Usage

```bash
thanos [-n <number> | -p <percentage>] [optional: -d <directory>, -m <regex>, -r] [--dry-run] [--confirm] [-h]
```

### Options

- `-h`
  Show this help message.

- `-n <number>`
  Delete a fixed number of files (mutually exclusive with `-p`).

- `-p <percentage>`
  Delete a percentage of matching files (mutually exclusive with `-n`).

- `-d <directory>`
  Specify the directory to operate on (default: current directory).

- `-m <regex>`
  Filter files by a regex that applies to file names (or relative paths if `-r` is specified).

- `-r`
  Enable recursive mode to process files in subdirectories.

- `--dry-run`
  Show which files would be deleted without actually deleting them.

- `--confirm`
  Require interactive confirmation before each deletion.

---

## Installation

Use the provided Makefile:

```bash
make
sudo make install
```

To uninstall:

```bash
sudo make uninstall
```

---

## License

MIT License

---

## Acknowledgments

Inspired by the Marvel Cinematic Universe. Made at 2 in the morning because I'm really stupid.

