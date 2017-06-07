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

    indx_f = 0
    writer = None
    f = None

    def open():
        f_name = "%s/%s-%05d" % (output_path, name_prefix, index_f)
        writer = recordio.writer(f_name)
        f = open(f_name, "w")
        index_f += 1

    def close():
        if writer is not None:
            writer.close()

        if f is not None:
            f.close()


    for i, d in enumerate(reader()):
        if writer is None:
            open()

        writer.write(pickle.dump(d, pickle.HIGHEST_PROTOCOL))

        if i % num_shards == 0 and i >= num_shards:
            close()
            open()

    close()

import tempfile
import glob
def test_convert():
    def test_reader():
        def reader():
            for x in xrange(10):
                yield x

        return reader

    _, temp_path = tempfile.mkstemp()

    convert(
        temp_path,
        test_reader(), 4, 'random_images')

    files = glob.glob(temp_path + '/random_images-*')
    self.assertEqual(len(files), 3)

test_convert()
