import random
from visualdl import LogWriter

logdir = "./tmp"
logger = LogWriter(logdir, sync_cycle=10)

# mark the components with 'train' label.
with logger.mode("train"):
    # create a scalar component called 'scalars/scalar0'
    scalar0 = logger.scalar("scalars/scalar0")

# add some records during DL model running.
for step in range(100):
    scalar0.add_record(step, random.random())
