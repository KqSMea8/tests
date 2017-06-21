import recordio

def RecordIO(path):
    """
    Creates a data reader that outputs record one one by one from given recordio file
    :path: path of recordio file
    :returns: data reader of recordio file
    """

    def reader():
        f = recordio.reader(path)
        while True:
            r = f.read()
            if r is None:
                break
            yield r
        f.close()
    return reader

class TestRecordIO(unittest.TestCase):
    def test_RecordIO(self):
        path = os.path.join(os.path.dirname(__file__), "test_recordio_creator.dat")
        reader = paddle.v2.reader.creator.RecordIO(path)
        for idx, r in enumerate(reader()):
            self.assertSequenceEqual(r, str(idx))
