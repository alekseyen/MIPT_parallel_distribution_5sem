from pyspark import SparkContext, SparkConf
import re

# find bigrams that have 'narodnaya'
def bigrams(s):
    words = re.findall(r"[\w']+", re.sub('\d', ' ', s))
    ans = []
    for i in range(len(words)):
        if i != len(words) - 1 and words[i] == 'narodnaya':
            ans.append(('{}_{}'.format(words[i], words[i+1]), 1))
    return ans

lower_func = lambda x: x.strip().lower()

config = SparkConf().setAppName('narodnaya').setMaster('yarn')
sc = SparkContext(conf = config)

rdd = sc.textFile('/data/wiki/en_articles_part')
rdd = rdd.map(lower_func).flatMap(bigrams)

rdd = rdd.reduceByKey(lambda a,b : a + b)
rdd = rdd.sortBy(lambda a: a[0])

for item in rdd.take(10):
    print('{}\t{}'.format(item[0], str(item[1])))