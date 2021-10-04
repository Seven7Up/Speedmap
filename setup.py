#!/usr/bin/env python
from __future__ import print_function
import os
import platform
import subprocess
import sys

from setuptools import setup, find_packages

if platform.python_version_tuple()[0] == '2':
    print("This script is only tested on Python3! Exitting...")
    sys.exit(-1)


# Convert README.md to reStructuredText for PyPI
long_description = ''
try:
    long_description = subprocess.check_output(
        ['pandoc', 'README.md', '--to=rst'], universal_newlines=True)
except:
    print("Failed to convert README.md through pandoc, proceeding anyway", file=sys.stderr)
    long_description = open(os.path.join(
        os.path.dirname(__file__), "README.md")).read()
    # traceback.print_exc()

setup(
    name='speedmap',
    python_requires='>=3.7',
    packages=find_packages(include=["speedmap", "speedmap.*"]),
    version='1.1',
    description="Speedmap is a python script which scans ports and nmap them.",
    long_description=long_description,
    entry_points={'console_scripts': [
        'speedmap=speedmap:main',
    ]},
    author="Seven7Up",
    author_email="seven7up@tutanota.com",
    download_url="https://github.com/Seven7Up/Speedmap",
    install_requires=['pwntools>=4.6.0'],
    keywords='speedmap nmap python portscanner',
)
