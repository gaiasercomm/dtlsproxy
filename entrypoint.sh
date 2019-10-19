#!/bin/sh

ARGS=""

if [ -n "$LISTEN" ]; then
    ARGS="$ARGS -l $LISTEN"
fi

if [ -n "$BACKEND" ]; then
    ARGS="$ARGS -b $BACKEND"
fi

if [ -n "$KEYS" ]; then
    ARGS="$ARGS -k `echo $KEYS | base64 -d`"
fi

/usr/local/bin/dtlsproxy $ARGS $*
