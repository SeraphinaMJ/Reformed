#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit 1
fi

pip3 install sphinx sphinxcontrib-plantuml sphinx_rtd_theme m2r
