#!/bin/sh

mkdir -m 700 -p ~/.kde/share/config
mkdir -m 700 -p ~/.kde/share/apps/kppp/Rules
mkdir -m 700 -p ~/.kde/share/apps/kppp/Log

exec $0.bin "$@"
