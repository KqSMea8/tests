import os
import sys
import re
import operator
import nltk

def get_all_txt_files(root):
    a = []
    for path, subdirs, files in os.walk(root):
        for file in files:
            if file.endswith(".txt"):
                print(os.path.join(root, file))
                a.append(os.path.join(root, file))
    return a

def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass

def load_words_dict(txt):
    s = set()
    for _, w in enumerate(open(txt, "r")):
        w = w.strip('\n\r\t ').lower()
        s.add(w)
    return s

def load_lemma(name):
    d = {}
    for _, l in enumerate(open(name,"r")):
        l = l.strip('\n\r\t ').lower().replace("->", '\t')
        #ws = re.split(r'[\s|\t]',l)
        ws = l.split('\t')
        a = []
        for w in ws:
            if len(w) <= 1:
                continue
            a.append(w)

        for i in range(1, len(a)):
            d.update({a[i]:a[0]})
    return d

def get_word_f(files, s, lemma):
    a = {}
    for f in files:
        for _,l in enumerate(open(f,"r")):
            """
            print l
            try:
                ws = nltk.word_tokenize(l)
                for w in ws:
                    print w
            except:
                pass
            """

            ws = re.split(r'[;, ."\':\n\(\)\?\!\#\]\[_]',l)
            for w in ws:
                if len(w) <= 2:
                    continue

                if w.isdigit():
                    continue

                if "/" in w or '\\' in w:
                    #print w
                    continue

                w = w.lower()
                if w not in s:
                    #print "not in", w
                    continue

                if w in lemma:
                    w = lemma[w]
                if w in a:
                    a[w] += 1
                else:
                    a[w] = 1

    #print a
    return sorted(a.items(), key=operator.itemgetter(1), reverse=True)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print "input format:exe dir"
        sys.exit(1)
    files = get_all_txt_files(sys.argv[1])
    s = load_words_dict("words.txt")
    lemma = load_lemma("./AntBNC_lemmas_ver_001.txt")
    words = get_word_f(files, s, lemma)
    for w in words:
        print w[1], w[0]



#get_all_txt_files("./pdf")
