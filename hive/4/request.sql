ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;

USE podkidysheval;

SELECT TRANSFORM(ip)
USING "sed 's/\([0-9]*\).\([0-9]*\).\([0-9]*\).\([0-9]*\)/\4.\3.\2.\1/'" AS ip
FROM Logs
ORDER BY ip
LIMIT 10;