add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;

--DROP DATABASE IF EXISTS podkidysheval CASCADE;
USE podkidysheval;

DROP TABLE IF EXISTS IPRegions;

CREATE EXTERNAL TABLE IPRegions (
        ip STRING,
        region STRING
)
ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
        "input.regex" = '^(\\S*)\\t(\\S*).*$',
        "output.format.string" = "%1$s %2$s"
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/ip_data_M';

select * from IPRegions limit 10;
