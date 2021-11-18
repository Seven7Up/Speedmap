# This script is based on pwntools (pwnlib.log)
# BIG CREDS to the guys, they did a great work
# Source: https://github.com/Gallopsled/pwntools/

import time
import random
import threading
import logging

from stdlib import ThreadWithReturnValue
from colors import Fore


# Variables
_log_level = logging.INFO
_spinner_style = Fore.BLUE
__reset = Fore.RESET
_msgtype_prefixes = {
    "info": [Fore.BLUE, "*"],
    "success": [Fore.GREEN, "+"],
    "error": [Fore.RED, "-"],
    "warn": [Fore.YELLOW, "!"],
    "debug": [Fore.MAGENTA, "DEBUG"]
}
# SPIIINEEERS *w*
# Source: https://github.com/Gallopsled/pwntools/blob/dev/pwnlib/term/spinners.py
spinners = [
    ['/.......', './......', '../.....', '.../....', '..../...', '...../..', '....../.',
     '.......\\', '......\\.', '.....\\..', '....\\...', '...\\....', '..\\.....', '.\\......'],
    ['|', '/', '-', '\\'],
    ['q', 'p', 'b', 'd'],
    ['.', 'o', 'O', '0', '*', ' ', ' ', ' '],
    ['▁', '▃', '▄', '▅', '▆', '▇', '█', '▇', '▆', '▅', '▄', '▃'],
    ['┤', '┘', '┴', '└', '├', '┌', '┬', '┐'],
    ['←', '↖', '↑', '↗', '→', '↘', '↓', '↙'],
    ['◢', '◢', '◣', '◣', '◤', '◤', '◥', '◥'],
    ['◐', '◓', '◑', '◒'],
    ['▖', '▘', '▝', '▗'],
    ['.', 'o', 'O', '°', ' ', ' ', '°', 'O', 'o', '.', ' ', ' '],
    ['<', '<', '∧', '∧', '>', '>', 'v', 'v'],
]

# class Thread(ThreadWithReturnValue):
#     def __init__(self, *args, **kwargs):
#         super(Thread, self).__init__(*args, **kwargs)
#         self.old = context.copy()

#     def __bootstrap(self):
#         """
#         Implementation Details:
#             This only works because the class is named ``Thread``.
#             If its name is changed, we have to implement this hook
#             differently.
#         """
#         context.update(**self.old)
#         sup = super(ThreadWithReturnValue, self)
#         bootstrap = getattr(sup, '_bootstrap', None)
#         if bootstrap is None:
#             sup.__bootstrap()
#         else:
#             bootstrap()
#     _bootstrap = __bootstrap


class Formatter(logging.Formatter):
    indent = '    '
    nlindent = '\n' + indent

    def format(self, record):
        msg = super(Formatter, self).format(record)
        msgtype = getattr(record, 'msgtype', None)
        if msgtype is None:
            return msg

        if msgtype in _msgtype_prefixes:
            style, symb = _msgtype_prefixes[msgtype]
            prefix = '[%s%s%s] ' % (style, symb, __reset)
        else:
            prefix = '[?] '

        msg = prefix + msg
        msg = self.nlindent.join(msg.splitlines())
        return msg


class Handler(logging.StreamHandler):
    def emit(self, record):
        level = logging.getLogger(record.name).getEffectiveLevel()
        if level == 1:
            level = _log_level
        if level > record.levelno:
            return

        progress = getattr(record, 'progress', None)

        if progress is None:# or not term.term_mode:
            super(Handler, self).emit(record)
            return

        # yay, spinners!

        msgtype = record.msgtype
        record.msgtype = 'animated'
        msg = "%s\n" % self.format(record)

        if not hasattr(progress, '_spinner_handle'):
            spinner_handle = None#term.output('')
            msg_handle = None#term.output(msg)
            stop = threading.Event()

            def spin():
                '''Wheeeee!'''
                state = 0
                states = random.choice(spinners.spinners)
                while True:
                    prefix = '[%s%s%s] ' % (
                        _spinner_style, states[state], __reset)
                    spinner_handle.update(prefix)
                    state = (state + 1) % len(states)
                    if stop.wait(0.1):
                        break
            t = ThreadWithReturnValue(target=spin)
            t.daemon = True
            t.start()
            progress._spinner_handle = spinner_handle
            progress._msg_handle = msg_handle
            progress._stop_event = stop
            progress._spinner_thread = t
        else:
            progress._msg_handle.update(msg)

        if msgtype != 'status':
            progress._stop_event.set()
            progress._spinner_thread.join()
            style, symb = _msgtype_prefixes[msgtype]
            prefix = '[%s%s%s] ' % (style, symb, __reset)
            progress._spinner_handle.update(prefix)


class Progress(object):
    def __init__(self, logger, msg, status, level, args, kwargs):
        self._logger = logger
        self._msg = msg
        self._status = status
        self._level = level
        self._stopped = False
        self.last_status = 0
        self.rate = kwargs.pop('rate', 0)
        self._log(status, args, kwargs, 'status')
        self.last_status = 0

    def _log(self, status: str, args, kwargs, msgtype):
        if self._stopped:
            return
        msg = self._msg
        if msg and status:
            msg += ': '
        msg += status
        self._logger._log(self._level, msg, args, kwargs, msgtype, self)

    def status(self, status, *args, **kwargs):
        now = time.time()
        if (now - self.last_status) > self.rate:
            self.last_status = now
            self._log(status, args, kwargs, 'status')

    def success(self, status='Done', *args, **kwargs):
        self._log(status, args, kwargs, 'success')
        self._stopped = True

    def error(self, status='Failed', *args, **kwargs):
        self._log(status, args, kwargs, 'error')
        self._stopped = True

    def __enter__(self):
        return self

    def __exit__(self, exc_typ, exc_val, exc_tb):
        if exc_typ is None:
            self.success()
        else:
            self.failure()


class Logger(object):
    def __init__(self, name, logger=logging.Logger):
        self._logger = logger(name)
        self._handler = Handler()
        self._formatter = Formatter()

        self._handler.setLevel(_log_level)
        self._handler.setFormatter(self._formatter)
        self._logger.addHandler(self._handler)

    def _log(self, level, msg: str, args, kwargs, msgtype, progress=None):
        extra = kwargs.get('extra', {})
        extra.setdefault('msgtype', msgtype)
        extra.setdefault('progress', progress)
        kwargs['extra'] = extra
        self._logger.log(level, msg, *args, **kwargs)

    def progress(self, message, status='', *args, **kwargs):
        level = _log_level
        return Progress(self, message, status, level, args, kwargs)

    def success(self, message, *args, **kwargs):
        self._log(logging.INFO, message, args, kwargs, 'success')

    def debug(self, message, *args, **kwargs):
        self._log(logging.DEBUG, message, args, kwargs, 'debug')

    def info(self, message, *args, **kwargs):
        self._log(logging.INFO, message, args, kwargs, 'info')

    def warn(self, message, *args, **kwargs):
        self._log(logging.WARNING, message, args, kwargs, 'warn')

    def error(self, message, *args, **kwargs):
        self._log(logging.ERROR, message, args, kwargs, 'error')
        raise Exception(message % args)

    def log(self, level, message, *args, **kwargs):
        self._log(level, message, args, kwargs, None)

    def isEnabledFor(self, level):
        effectiveLevel = self._logger.getEffectiveLevel()

        if effectiveLevel == 1:
            effectiveLevel = _log_level
        return effectiveLevel <= level

    def setLevel(self, level):
        __log_level = level
        self._logger.setLevel(__log_level)

    def addHandler(self, handler):
        self._logger.addHandler(handler)

    def removeHandler(self, handler):
        self._logger.removeHandler(handler)


log = Logger(__name__)