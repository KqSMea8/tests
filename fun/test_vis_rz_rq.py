import time
import pandas as pd
from visualdl import LogWriter

logdir = "./tmp"
logger = LogWriter(logdir, sync_cycle=10)

class RZRQ(object):
    def get_data(self, file_name):
        d = pd.read_csv(file_name)
        rzye = d['rzmre']
        #for t in rzye:
        #    print t
        #print rzye
        #rzye.plot()
        #time.sleep(1800)
        #for r in rzye.itertuples():
        #    print r
        #for column_name, column in d.transpose().iterrows():
        #    print column_name
        return rzye
    def plot(self, data):
        # mark the components with 'train' label.
        with logger.mode("train"):
            # create a scalar component called 'scalars/scalar0'
            scalar0 = logger.scalar("scalars/scalar0")

        #print data
        # add some records during DL model running.
        for i, d in enumerate(data):
            #print d
            scalar0.add_record(i, d/10000)


o = RZRQ()
d = o.get_data('600276')
o.plot(d)

