import recordio

w = recordio.writer("/tmp/test_recordio_creator.dat")
for i in range(0,10):
    print i
    w.write(str(i))
w.close()
