myterm
======
myterm is a simple utility used to start a user's preferred terminal emulator.

Installation
------------
The following software is required to build and install myterm:

  * C compiler (such as GCC)
  * make

Install myterm by running:

    $ make
    $ sudo make install

Usage
-----
The name of the terminal binary is read from the `TERMINAL` environment
variable. If `TERMINAL` is not found in the process environment, myterm will
try to read the preferred terminal from a file at `~/.config/terminal`.

All arguments to myterm are passed directly to the terminal emulator. If needed,
`TERMINAL_STRIP` can be used to remove a single argument before executing
the terminal. For example, `TERMINAL_STRIP="-e"` will cause myterm to
remove `-e` if it is first in the argument list.

Additional arguments can be passed to the terminal from `TERMINAL_ARGS`. For
example, `TERMINAL=xterm TERMINAL_ARGS="foo bar" myterm baz` will call `xterm`
with the following argument list: `foo bar baz`.

`TERMINAL` may also be configured to point to myterm itself. In this case,
myterm will look for the terminal name in `TERMINAL_CMD` followed by the
configuration file.

See `myterm(1)` for full usage details.

License and Copyright
---------------------
Copyright (c) 2022 Corey Hinshaw

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
