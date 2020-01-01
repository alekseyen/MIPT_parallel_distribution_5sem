#! /usr/bin/env bash

hive -f log.sql
hive -f user.sql
hive -f ip.sql