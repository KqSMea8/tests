#!/usr/bin/python
import sys
import os

if __name__ == '__main__':
    #en-fr
    cmd="python -u train.py --src_vocab_fpath thirdparty/vocab.wordpiece.en-fr --trg_vocab_fpath thirdparty/vocab.wordpiece.en-fr  --special_token '<s>' '<e>' '<unk>' --token_delimiter '\\x01' --train_file_pattern train_data/train.wordpiece.en-fr.\* --val_file_pattern thirdparty/newstest2014.wordpiece.en-fr --use_token_batch True --batch_size  3200 --sort_type pool --pool_size 200000 --update_method nccl2"

    #en-de
    #cmd = "python -u train.py --src_vocab_fpath thirdparty/vocab.bpe.32000 --trg_vocab_fpath thirdparty/vocab.bpe.32000 --special_token '<s>' '<e>' '<unk>' --train_file_pattern thirdparty/train.tok.clean.bpe.32000.en-de --token_delimiter ' ' --use_token_batch True --batch_size 2048 --sort_type pool --pool_size 200000 --update_method nccl2"

    #Just for test
    '''
    cmd = """
python -u train.py \
--src_vocab_fpath thirdparty/vocab.wordpiece.en-fr \
--trg_vocab_fpath thirdparty/vocab.wordpiece.en-fr \
--special_token '<s>' '<e>' '<unk>' \
--train_file_pattern thirdparty/train.wordpiece.en-fr \
--token_delimiter ' ' \
--use_token_batch True \
--batch_size 2048 \
--sort_type pool \
--pool_size 200000 \
--update_method nccl2
"""
'''
    print "cmd:", cmd
    exit(os.system(cmd))
