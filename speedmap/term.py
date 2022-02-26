from log import log

log.info("Hello World!")

import sys
import termios
import curses

fd = sys.stdout

class Cell(object):
    pass


_term_mode = termios.tcgetattr(fd.fileno())
