{\rtf1\ansi\ansicpg1251\cocoartf2511
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fmodern\fcharset0 Courier;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;}
\paperw11900\paperh16840\margl1440\margr1440\vieww16820\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\sl280\partightenfactor0

\f0\fs24 \cf2 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec2 #! /usr/bin/env bash\
\
BLOCK_ID=`hdfs fsck $1 -files -blocks -locations | egrep -o 'blk_[0-9]*'`\
hdfs fsck -blockId $BLOCK_ID | grep -oP '(?<=rack: ).*(?=/default)' | head -1\
}