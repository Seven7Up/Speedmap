# SPEEDMAP

![Python Versions](https://img.shields.io/badge/python-3.7|3.8|3.9-yellow.svg)
![GitHub branch checks state](https://img.shields.io/github/checks-status/Seven-7Up/speedmap/master?color=green&logo=master&style=flat-square)
![Build](https://img.shields.io/badge/build-passing-brightgreen)

## About:

Speedmap is a python script which scans a target ports and use nmap for a huge and win time for a full ports scan aimed to be fast and ... speed?! I AM SPEED! ;)

## Setup:

Install required packages with pip as:

```
pip install -r requirements.txt
```

or:

```
pip install colorama pwntools
```

## Usage:

To print help use:

```
./speedmap.py -h
```

basic test:

```
./speedmap.py -d localhost
```

All infos are in the help message:

```
_______________________________________________  _________________
__  ___/__  __ \__  ____/__  ____/__  __ \__   |/  /__    |__  __ \
_____ \__  /_/ /_  __/  __  __/  __  / / /_  /|_/ /__  /| |_  /_/ /
____/ /_  ____/_  /___  _  /___  _  /_/ /_  /  / / _  ___ |  ____/
/____/ /_/     /_____/  /_____/  /_____/ /_/  /_/  /_/  |_/_/

Usage: speedmap.py [-h] [-v] [-t TIMEOUT] [-d] [-p PORTS] [-n NMAP_OPTIONS [NMAP_OPTIONS ...]] [-oN NMAP_OUTPUT] targets

Speedmap is python script aiming to be fast and ... speed?! I AM SPEED!

Positional arguments:
  targets               Set Scan Destination. Separated by comma ','

Optional arguments:
  -h, --help            Show this help message and exit.
  -v, --version         Show program's version and exit.
  -t TIMEOUT, --timeout TIMEOUT
                        Set a Timeout to Wait peer Request. Default is 1
  -d, --debug           Turn Debuging on
  -p PORTS, --ports PORTS
                        Specify which Ports to Scan, Use a START-END for a range (Ex: 1-1000, Ex2: 200-200). Default is 1-65535
  -n NMAP_OPTIONS [NMAP_OPTIONS ...], --nmap NMAP_OPTIONS [NMAP_OPTIONS ...]
                        Set Nmap flags. Use '_' on place of '-'. Default is '_A _sC _vv'
  -oN NMAP_OUTPUT, --nmap-output NMAP_OUTPUT
                        Name the Nmap output file.
```
