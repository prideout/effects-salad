#!/bin/sh
cd jsoncpp-src-0.5.0 \
&& scons platform=linux-gcc \
&& cp include/json/* ../ \
&& cp libs/linux-gcc-4.2.1/* ../ \
&& cd -



