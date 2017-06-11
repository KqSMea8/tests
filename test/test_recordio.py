def convert(output_path, reader, num_shards, name_prefix):
    import recordio
    import cPickle as pickle
    """
    Convert data from reader to recordio format files.

    :param output_path: directory in which output files will be saved.
    :param reader: a data reader, from which the convert program will read data instances.
    :param num_shards: the number of shards that the dataset will be partitioned into.
    :param name_prefix: the name prefix of generated files.
    """

    def open_needs(idx):
        n = "%s/%s-%05d" % (output_path, name_prefix, idx)
        w = recordio.writer(n)
        f = open(n, "w")
        idx += 1

        return w, f, idx

    def close_needs(w, f):
        if w is not None:
            w.close()

        if f is not None:
            f.close()

    idx = 0
    w = None
    f = None

    for i, d in enumerate(reader()):
        if w is None:
            w, f, idx = open_needs(idx)

        w.write(pickle.dumps(d, pickle.HIGHEST_PROTOCOL))

        if i % num_shards == 0 and i >= num_shards:
            close_needs(w, f)
            w, f, idx = open_needs(idx)

    close_needs(w, f)

import tempfile
import glob
def test_convert():
    def test_reader():
        def reader():
            for x in xrange(10):
                yield x

        return reader

    path = tempfile.mkdtemp()

    convert(
        path,
        test_reader(), 4, 'random_images')

    files = glob.glob(path + '/random_images-*')
    print len(files) == 3

test_convert()
