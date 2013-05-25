# coding: UTF-8
import os
import time



def word_count():
    indir = os.path.dirname(os.path.abspath(__file__))
    page_path = os.path.join(indir,'page.html')
    with open(page_path, 'r') as fh:
        lines = fh.readlines()
        word_count = sum([len(l) for l in lines])
        return word_count
    
    
#time.sleep(6)
execute_func = word_count

if __name__ == '__main__':
    
    print execute_func()
