#!/bin/sh
# workaround for `docker` not being callable directly from a makefile in some systems
exec docker run $@
