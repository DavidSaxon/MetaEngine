#!/bin/bash

make && valgrind --leak-check=full ./build/linux_x86/tests --single_proc
