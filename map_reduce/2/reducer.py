#!/usr/bin/env python

import sys

select_second_field = lambda x: -x[1]
MAX_WORDS_IN_LINE = 5

# out ans in next as follows:
# key count WORD1:COUNT1, WORD2:COUNT2, ..
def out_ans(words_arr):
    outstr = ""
    for form, form_count in sorted(words_arr, key=select_second_field)[:MAX_WORDS_IN_LINE]:
        outstr += "{}:{};".format(form, str(form_count))
    print("{}\t{}\t{}".format(str(sum_count), curr_key, outstr))

curr_key = ""
sum_count = 0
words = dict()

## pair comes with the same key comes to the text reader 
for line in sys.stdin:
    key, word, word_count = line.split('\t')
    word_count = int(word_count)
    if key != curr_key:
    # block ended
        if curr_key:
            out_ans(words.items())
        curr_key = key
        sum_count = word_count
        words = {word : word_count}
    else:
        # We inside the block
        sum_count += word_count
        if word in words.keys():
            words[word] += word_count
        else:
            words[word] = word_count

if curr_key:
    out_ans(words.items())