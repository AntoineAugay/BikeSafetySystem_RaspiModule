#!/bin/bash

sudo gatttool -t random -b D5:32:AB:6C:5E:48 -I

echo "connect"

connect

echo "quit"

quit