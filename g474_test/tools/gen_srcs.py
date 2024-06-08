import os
import sys

dir1 = 'lvgl'
dir2 = 'src'

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
        truncted_file = src_file.split('lvgl', 1)[-1]
        truncted_file_list.append(truncted_file)
        print("lvgl" + truncted_file + ' \\')

if __name__ == '__main__':
    src_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..',  dir1, dir2))
    find_srcs(src_dir)