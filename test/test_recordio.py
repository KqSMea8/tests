import tempfile
import glob
import recordio
import cPickle as pickle

def convert(output_path, reader, num_shards, name_prefix, max_lines_to_shuffle=10000):
    import recordio
    import cPickle as pickle
    import random
    """
    Convert data from reader to recordio format files.

    :param output_path: directory in which output files will be saved.
    :param reader: a data reader, from which the convert program will read data instances.
    :param num_shards: the number of shards that the dataset will be partitioned into.
    :param name_prefix: the name prefix of generated files.
    :param max_lines_to_shuffle: the max lines numbers to shuffle before writing.
    """

    assert num_shards >= 1
    assert max_lines_to_shuffle >= 1

    def open_writers():
        w = []
        for i in range(0, num_shards):
            n = "%s/%s-%05d-of-%05d" % (output_path, name_prefix, i, num_shards - 1)
            w.append(recordio.writer(n))

        return w

    def close_writers(w):
        for i in range(0, num_shards):
            w[i].close()

    def write_data(w, lines):
        random.shuffle(lines)
        for i, d in enumerate(lines):
            d = pickle.dumps(d, pickle.HIGHEST_PROTOCOL)
            w[i % num_shards].write(d)

    w = open_writers()
    lines = []

    for i, d in enumerate(reader()):
        lines.append(d)
        if i % max_lines_to_shuffle == 0  and i >= max_lines_to_shuffle:
            write_data(w, lines)
            lines = []
            continue

    write_data(w, lines)
    close_writers(w)

def test_convert():
    record_num = 10
    num_shards = 4

    def test_reader():
        def reader():
            for x in xrange(record_num):
                yield x

        return reader

    path = tempfile.mkdtemp()
    #paddle.v2.dataset.common.convert(path,
    convert(path,
                                     test_reader(), num_shards, 'random_images')

    files = glob.glob(path + '/random_images-*')
    #self.assertEqual(len(files), num_shards)
    print len(files) == num_shards

    recs = []
    for i in range(0, num_shards):
        n = "%s/random_images-%05d-of-%05d" % (path, i, num_shards - 1)
        r = recordio.reader(n)
        while True:
            d = r.read()
            if d is None:
                break
            recs.append(d)

    recs.sort()
    #self.assertEqual(total, record_num)
    print len(recs) == record_num



test_convert()
"""
s = pickle.dumps("1")
print s
print pickle.loads(s)
"""
