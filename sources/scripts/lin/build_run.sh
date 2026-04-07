#!/usr/bin/env bash

#set -e

./build.sh
kill -9 $(pidof Sunset)

#./start_x.sh
