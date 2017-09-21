select * from user_tables;

create database link dblink1 
connect to h8l59s identified by h8l59s 
using 'aramis';

select * from user_db_links;