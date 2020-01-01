add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;
add jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;

set hive.exec.max.dynamic.partitions=5000;
set hive.exec.max.dynamic.partitions.pernode=1000;

--DROP DATABASE IF EXISTS podkidysheval CASCADE
USE podkidysheval;

DROP TABLE IF EXISTS Logs_help;

CREATE EXTERNAL TABLE Logs_help (
        ip STRING,
        date_time INT,
        http_request STRING,
        size INT,
        http_status INT,
        browser STRING
) 
ROW FORMAT SERDE 'org.apache.hadoop.hive.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
        "input.regex" = '^(\\S*)\\t\\t\\t(\\d{8})\\S*\\t(\\S*)\\t(\\d*)\\t(\\d*)\\t(\\S*?(?=\\s)).*$'
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/user_logs_M';

--------------------PARTITIONING--------------------
SET hive.exec.dynamic.partition.mode=nonstrict;

DROP TABLE IF EXISTS Logs;

CREATE EXTERNAL TABLE Logs (
    ip STRING,
    http_request STRING,
    size INT,
    http_status INT,
    browser STRING
)
PARTITIONED BY (date_time INT)
STORED AS TEXTFILE;

INSERT OVERWRITE TABLE Logs PARTITION (date_time)
SELECT ip STRING,
        http_request string, 
        size int, 
        http_status int, 
        browser string,
        date_time int
        FROM Logs_help;

select * from Logs limit 10;
