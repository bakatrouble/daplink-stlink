#!/bin/bash

progen generate -t cmake_gcc_arm -o generator=ninja -p stm32f103xb_bl -b
progen generate -t cmake_gcc_arm -o generator=ninja -p stm32f103xb_if -b
