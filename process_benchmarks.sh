#!/bin/bash


sed 's/benchmark_m_policy//g' $1 |
sed 's/"//g' |
sed 's/<\([^>]*\)>/\1/g' |
sed 's/_sample/sample_/g' |
sed 's/_update/update_/g' |
sed 's/\/iterations:10\/manual_time//g' |
sed 's#/#,#g' |
sed 's#,,,,,##g' >>$2  

