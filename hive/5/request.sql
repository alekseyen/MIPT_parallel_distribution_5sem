ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-serde.jar;
ADD jar /opt/cloudera/parcels/CDH/lib/hive/lib/hive-contrib.jar;

USE podkidysheval;

SELECT TRANSFORM(ip, date_time, http_request, size, http_status, browser)
USING "sed -r 's|.ru/|.com/|'" AS ip, date_time, http_request, size, http_status, browser
FROM Logs
LIMIT 10;