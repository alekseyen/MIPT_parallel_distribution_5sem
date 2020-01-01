ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;

USE podkidysheval;

SELECT region,
SUM(IF(sex="male", 1, 0)) AS male,
SUM(IF(sex="female", 1, 0)) AS female
FROM Logs, Users, IPRegions
WHERE Logs.ip=Users.ip AND Logs.ip = IPRegions.ip
GROUP BY region;