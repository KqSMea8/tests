#!/usr/bin/python
import sys
import os

if __name__ == '__main__':
    cmd="python -u train.py --src_vocab_fpath thirdparty/vocab.wordpiece.en-fr --trg_vocab_fpath thirdparty/vocab.wordpiece.en-fr  --special_token '<s>' '<e>' '<unk>' --token_delimiter '\\x01' --train_file_pattern thirdparty/train.wordpiece.en-fr --val_file_pattern thirdparty/newstest2014.wordpiece.en-fr --use_token_batch True --batch_size  3200 --sort_type pool --pool_size 200000"
    print cmd
    exit(os.system(cmd))
