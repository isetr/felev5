 select data_type
 from dba_tab_columns 
 where owner = 'NIKOVITS' 
 and table_name = 'EMP'
 and column_id = 6;
 
 select distinct owner, table_name
 from dba_tab_columns
 where column_name like 'Z%';
 
 select table_name, count(*)
 from dba_tab_columns
 where data_type = 'DATE'
 group by table_name, owner
 having count(*) >= 8;
 
 select table_name, owner
 from dba_tab_columns
 where data_type = 'VARCHAR2'
 and (column_id = 1 or column_id = 4)
 group by table_name, owner
 having count(*) = 2;
 
 -- SYNONYMS
 
 select * from dba_synonyms;
 
 create synonym s for branyi.sör;
 
 select * from user_synonyms;
 
 create synonym n for nikovits.emp;
 
 select * from szinonima1;
 
 select * from dba_synonyms
 where synonym_name = 'SZINONIMA1';
 
 select * from dba_objects
 where object_name = 'DUAL';
 
 select * from dba_synonyms
 where synonym_name = 'DUAL';
 
 create synonym d for sys.dual;
 
 -- SEQUENCES
 
 select * from dba_sequences;
 
 create sequence plus3 
 start with 10
 increment by 4 -- Kappa
 nocycle;
 
 select * from user_sequences;
 
 select plus3.currval from dual;
 
 create table b (A number);
 
 select * from b;
 
 insert into b values (3);
 
 insert into b values (plus3.currval);
 
 insert into b values (plus3.nextval);
 
 select plus3.currval from dual;
 
 -- LINKS
 
 create database link dblink1 
 connect to h8l59s identified by h8l59s 
 using 'eszakigrid';
 
 select * from nikovits.emp@dblink1;