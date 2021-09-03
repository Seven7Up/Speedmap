#!/usr/bin/env python3

import argparse
from os import path
import random
import socket
import subprocess
import threading
import time
import sys

from colorama import Fore
from pwn import log
# import xmltodict
# import json


def banner():
    colors = [Fore.LIGHTGREEN_EX, Fore.GREEN, Fore.LIGHTGREEN_EX, Fore.LIGHTYELLOW_EX, Fore.YELLOW, Fore.RED,
              Fore.LIGHTRED_EX, Fore.LIGHTMAGENTA_EX, Fore.MAGENTA, Fore.BLUE, Fore.LIGHTBLUE_EX, Fore.CYAN, Fore.LIGHTCYAN_EX]

    ascii_banner = [
        r"""_______________________________________________  _________________
__  ___/__  __ \__  ____/__  ____/__  __ \__   |/  /__    |__  __ \
_____ \__  /_/ /_  __/  __  __/  __  / / /_  /|_/ /__  /| |_  /_/ /
____/ /_  ____/_  /___  _  /___  _  /_/ /_  /  / / _  ___ |  ____/
/____/ /_/     /_____/  /_____/  /_____/ /_/  /_/  /_/  |_/_/""",
        r"""   _______  ___________  __  ______   ___ 
  / __/ _ \/ __/ __/ _ \/  |/  / _ | / _ \
 _\ \/ ___/ _// _// // / /|_/ / __ |/ ___/
/___/_/  /___/___/____/_/  /_/_/ |_/_/""",
        r"""  .dBBBBP dBBBBBb  dBBBP  dBBBP  dBBBBb  dBBBBBBb dBBBBBb   dBBBBBb
  BP          dB'                   dB'   '   dB'      BB       dB'
  `BBBBb  dBBBP' dBBP   dBBP   dBP dB' dB'dB'dB'   dBP BB   dBBBP' 
     dBP dBP    dBP    dBP    dBP dB' dB'dB'dB'   dBP  BB  dBP     
dBBBBP' dBP    dBBBBP dBBBBP dBBBBB' dB'dB'dB'   dBBBBBBB dBP""",
        r"""   .dMMMb  dMMMMb  dMMMMMP dMMMMMP dMMMMb  dMMMMMMMMb  .aMMMb  dMMMMb 
  dMP" VP dMP.dMP dMP     dMP     dMP VMP dMP"dMP"dMP dMP"dMP dMP.dMP 
  VMMMb  dMMMMP" dMMMP   dMMMP   dMP dMP dMP dMP dMP dMMMMMP dMMMMP"  
dP .dMP dMP     dMP     dMP     dMP.aMP dMP dMP dMP dMP dMP dMP       
VMMMP" dMP     dMMMMMP dMMMMMP dMMMMP" dMP dMP dMP dMP dMP dMP""",
        r"""  ___ ___ ___ ___ ___  __  __   _   ___ 
 / __| _ \ __| __|   \|  \/  | /_\ | _ \
 \__ \  _/ _|| _|| |) | |\/| |/ _ \|  _/
 |___/_| |___|___|___/|_|  |_/_/ \_\_|"""
    ]

    ascii_banner = random.choice(ascii_banner)
    ascii_banner = [colors[i % len(colors)]+ascii_banner[i]
                    for i in range(len(ascii_banner))]
    print("%s%s\n" % ("".join(ascii_banner), Fore.RESET))


def status_banner(target, initial=False, nmap=False, final=False):
    if initial:
        print("-" * 75)
        log.info("Scanning Target: " + target)
        log.info("Scanning started at: " + str(time.ctime()))
        print("-" * 75)
    if nmap:
        print("-" * 75)
        log.info("Running nmap command!")
        log.info("Nmap Scanning started at: " + str(time.ctime()))
        print("-" * 75)
    if final:
        print("-" * 75)
        log.info("Done!")


def request(target, port, timeout):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.setdefaulttimeout(timeout)
    response = s.connect_ex((socket.gethostbyname(target), port))
    s.close()
    return response


def debug(message: str, debug_option):
    if debug_option:
        log.info("%sDEBUG:%s %s" % (Fore.MAGENTA, Fore.RESET, message))


def basic_scan(target_index, target, ports, timeout):
    working_ports = []

    ports_progress = log.progress("Trying ports")
    try:
        for port in range(ports[0], ports[1]+1):
            port_percentage = (((port-ports[0])*100)//(ports[1]-ports[0]))
            ports_progress.status(f"{port}/{ports[1]} ~= {port_percentage}%")
            result = request(target, port, timeout)
            if result == 0:
                log.info("Port %s is open" % (port))
                working_ports.append(port)

    except socket.gaierror:
        log.failure("Hostname Could Not Be Resolved !!!!")
        sys.exit(1)
    except socket.error:
        log.failure("Server not responding !!!!")
        sys.exit(1)

    ports_progress.success("Done!")
    return working_ports


def stream_process(process):
    go = process.poll() is None
    for line in process.stdout:
        print(line.rstrip().decode())
    return go


def nmap(target, ports: str, options: list, nmap_output, debug_option):
    status_banner(target, nmap=True)
    # + ["-oX", "/tmp/speedmap-nmap.xml"]
    command = ["nmap", "-p", ports, target, "-oN", nmap_output]
    command += options
    # command += ["--version-trace", "--script-trace"]
    debug("command = '%s'" %
          "".join([f"{i} " for i in command])[:-1], debug_option)
    process = subprocess.Popen(
        command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while stream_process(process):
        time.sleep(0.1)
    # xml_output = open("/tmp/speedmap-nmap.xml", "r").read().rstrip()
    # return json.loads(json.dumps(xmltodict.parse(xml_output)))


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


def sub_main(thread_index, results, target, ports, timeout, debug_option):
    debug(
        f"Thread thread_{thread_index}: ports={ports[0]}-{ports[1]}", debug_option)
    working_ports = basic_scan(thread_index, target, ports, timeout)
    if len(working_ports) == 0:
        results[thread_index] = working_ports
        return
    debug_working_ports = "".join([f"{i}," for i in working_ports])[:-1]
    results[thread_index] = working_ports


def main():
    banner()
    parser = MyParser(description="Speedmap is python script aiming to be fast and ... speed?! I AM SPEED!",
                      add_help=False, formatter_class=CapitalisedHelpFormatter)
    parser._positionals.title = 'Positional arguments'
    parser._optionals.title = 'Optional arguments'
    parser.add_argument('-h',
                        '--help',
                        action='help',
                        default=argparse.SUPPRESS,
                        help='Show this help message and exit.'
                        )
    parser.add_argument('-v',
                        '--version',
                        action='version',
                        version='%(prog)s 2.0',
                        help="Show program's version and exit."
                        )
    parser.add_argument("targets",
                        metavar="targets",
                        type=str,
                        help="Set Scan Destination. Separated by comma ','"
                        )
    parser.add_argument("-t",
                        "--timeout",
                        type=int,
                        default=1,
                        dest="timeout",
                        help="Set a Timeout to Wait peer Request. Default is 1"
                        )
    parser.add_argument("-d",
                        "--debug",
                        action='store_true',
                        dest="debug_option",
                        help="Turn Debuging on"
                        )
    parser.add_argument("-p",
                        "--ports",
                        type=str,
                        default="1-65535",
                        dest="ports",
                        help="Specify which Ports to Scan, Use a START-END for a range (Ex: 1-1000, Ex2: 200-200). Default is 1-65535"
                        )
    parser.add_argument("-n",
                        "--nmap",
                        type=str,
                        default=["_A", "_sC", "_vv"],
                        nargs='+',
                        dest="nmap_options",
                        help="Set Nmap flags. Use '_' on place of '-'. Must be the last argument. Default is '_A _sC _vv'"
                        )
    parser.add_argument("-oN",
                        "--nmap-output",
                        type=str,
                        required=False,
                        dest="nmap_output",
                        help="Name the Nmap output file."
                        )
    parser.add_argument("-T",
                        "--threads",
                        type=int,
                        default=10,
                        dest="max_threads",
                        help="Set the maximum threads to create"
                        )
    args = parser.parse_args()
    debug_option = args.debug_option
    targets = args.targets
    ports = [int(i) for i in args.ports.split("-")]
    if len(ports) != 2:
        log.failure("Ports should be 2 items (Ex: 21-1000)")
        sys.exit(1)
    max_threads = args.max_threads
    debug(f"{debug_option=}", debug_option)
    debug(f"{targets=}", debug_option)
    debug(f"{ports=}", debug_option)
    debug(f"{max_threads=}", debug_option)
    if ports[0] < 1 or ports[1] > 65535:
        log.failure("Invalid ports range!")
        sys.exit(1)
    timeout = args.timeout
    nmap_options = [i.replace("_", "-") for i in args.nmap_options]
    threads = []
    for target in targets.split(","):
        if target == "":
            continue
        if not args.nmap_output:
            nmap_output = [
                f"speedmap_{target}_{time.gmtime()[0]}-{time.gmtime()[1]}-{time.gmtime()[2]}_", 1, ".txt"]
            while path.exists("".join([str(i) for i in nmap_output])):
                nmap_output[1] += 1
            nmap_output = "".join([str(i) for i in nmap_output])
        else:
            nmap_output = args.nmap_output
        debug(f"{nmap_output=}", debug_option)
        threads_ports = [(ports[1]-ports[0])//max_threads]*(max_threads-1)
        threads_ports += [((ports[1]-ports[0])//max_threads) +
                          ((ports[1]-ports[0]) % max_threads)]
        threads = [None] * max_threads
        results = [None] * max_threads
        port_index = ports[0]
        status_banner(target, initial=True)
        first_time = time.time()
        for thread_index in range(len(threads)):
            threads[thread_index] = ThreadWithReturnValue(target=sub_main, args=(thread_index, results, target, [
                                                          port_index, port_index+threads_ports[thread_index]], timeout, debug_option))
            threads[thread_index].daemon = True
            threads[thread_index].start()
            port_index += threads_ports[thread_index]
        try:
            # while threading.active_count() > 1:
            #     time.sleep(0.1)
            for thread_index in range(len(threads)):
                threads[thread_index].join()
        except KeyboardInterrupt:
            # for thread_index in range(len(threads)):
            #     threads[thread_index].kill()
            log.failure("Exitting Program !!!!")
            sys.exit(0)
        if len([c for i in results for c in i]) == 0:
            log.failure("There is no open port in this machine!")
            continue
        secound_time = time.time()
        log.info(
            f"The Time taken to Scan all ports is {int((secound_time-first_time)*1000)} ms")
        working_ports = "".join([f"{c}," for i in results for c in i])[:-1]
        log.info(f"working_ports = {working_ports}")
        nmap(target, working_ports, nmap_options, nmap_output, debug_option)
        status_banner(target, final=True)


if __name__ == "__main__":
    main()
