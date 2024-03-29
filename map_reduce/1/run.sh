#!/usr/bin/env bash
OUT_DIR="streaming_mr_result"
NUM_REDUCERS=8

hadoop fs -rm -r -skipTrash $OUT_DIR* > /dev/null  

yarn jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
    -D mapreduce.job.name="num_110_podkidysheval" \
    -D mapreduce.job.reduces=${NUM_REDUCERS} \
    -files mapper.py,reducer.py \
    -mapper mapper.py \
    -reducer reducer.py \
    -input /data/ids \
    -output $OUT_DIR > /dev/null

hdfs dfs -cat ${OUT_DIR}/part-00000 | head -n 50