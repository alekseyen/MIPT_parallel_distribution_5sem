ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;

USE podkidysheval;

SELECT date_time, COUNT(*) as cnt_ FROM Logs
GROUP BY date_time
ORDER BY cnt_ DESC;