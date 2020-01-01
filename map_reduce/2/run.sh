#!/usr/bin/env bash
 
OUT_DIR="out"
NUM_REDUCERS=4
 
hadoop fs -rm -r -skipTrash ${OUT_DIR}.tmp > /dev/null
 
hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="Task114 counting" \
    -D mapreduce.job.reduces=$NUM_REDUCERS \
    -files mapper.py,reducer.py \
    -mapper mapper.py \
    -reducer reducer.py \
    -input /data/wiki/en_articles \
    -output ${OUT_DIR}.tmp > /dev/null
 
 
hadoop fs -rm -r -skipTrash ${OUT_DIR} > /dev/null
 
hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="Task114 sorting" \
    -D mapreduce.job.reduces=1 \
    -D mapreduce.job.separator=\t \
    -D mapreduce.partition.keycomparator.options=-k1,1nr \
    -D mapreduce.job.output.key.comparator.class=org.apache.hadoop.mapreduce.lib.partition.KeyFieldBasedComparator \
    -files sort.py \
    -mapper /bin/cat \
    -reducer sort.py \
    -input ${OUT_DIR}.tmp \
    -output ${OUT_DIR} > /dev/null
 
hadoop fs -cat ${OUT_DIR}/part-00000 | head -n 10