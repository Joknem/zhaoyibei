#!/usr/bin/env python3
import os
import sys

dir1 = 'user'
dir2 = 'drivers'
dir3 = 'u8g2'

def find_srcs(src_dir):
    src_list = []
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            if file.endswith('.c'):
                src_list.append(os.path.join(root, file))
        pass
    pass
    truncted_file_list = []
    for src_file in src_list:
        truncted_file = src_file.split('g474_test/', 1)[-1]
        truncted_file_list.append(truncted_file)
        print(truncted_file + ' \\')

if __name__ == '__main__':
    src_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..',  dir1, dir2, dir3))
    find_srcs(src_dir)