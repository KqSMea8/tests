
import tushare as tf

ret = tf.sh_margin_details(start='2015-01-01', end='2018-01-01', symbol='600276')

ret.to_csv('600276', encoding='utf8')

