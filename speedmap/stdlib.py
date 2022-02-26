#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import sys
import threading


class CapitalisedHelpFormatter(argparse.HelpFormatter):
    def add_usage(self, usage, actions, groups, prefix=None):
        if prefix is None:
            prefix = 'Usage: '
        return super(CapitalisedHelpFormatter, self).add_usage(
            usage, actions, groups, prefix)


class MyParser(argparse.ArgumentParser):
    def error(self, message):
        print('Error: %s' % message)
        self.print_help()
        sys.exit(1)

# https://stackoverflow.com/questions/6893968/how-to-get-the-return-value-from-a-thread-in-python


class ThreadWithReturnValue(threading.Thread):
    def __init__(self, group=None, target=None, name=None,
                 args=(), kwargs={}, Verbose=None):
        threading.Thread.__init__(self, group, target, name, args, kwargs)
        self._return = None

    def run(self):
        if self._target is not None:
            self._return = self._target(*self._args,
                                        **self._kwargs)

    # def globaltrace(self, frame, event, arg):
    #     if event == 'call':
    #         return self.localtrace
    #     else:
    #         return None

    # def localtrace(self, frame, event, arg):
    #     if self.killed:
    #         if event == 'line':
    #             raise SystemExit()
    #     return self.localtrace

    # def kill(self):
    #     self.killed = True

    def join(self, *args):
        threading.Thread.join(self, *args)
        return self._return
