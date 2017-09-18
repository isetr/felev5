select table_name, tablespace_name from user_tables;

select table_name, tablespace_name from all_tables;

SELECT sum(darab) FROM nikovits.hivas, nikovits.kozpont, nikovits.primer
WHERE hivas.kozp_azon_hivo=kozpont.kozp_azon AND kozpont.primer=primer.korzet
AND primer.varos = 'Szentendre' 
AND datum = next_day(to_date('2012.01.31', 'yyyy.mm.dd'),'hétfõ') - 1;

SELECT sum(darab) FROM nikovits.hivas, nikovits.kozpont, nikovits.primer
WHERE hivas.kozp_azon_hivo=kozpont.kozp_azon AND kozpont.primer=primer.korzet
AND primer.varos = 'Szentendre' 
AND datum + 1 = next_day(to_date('2012.01.31', 'yyyy.mm.dd'),'hétfõ');

select * from all_tables
where table_name like 'C%'
and owner = 'NIKOVITS';

select * from dba_objects;

select distinct owner from dba_tab_columns
where table_name = 'DBA_TABLES';

select owner from dba_objects
where object_name = 'DUAL';

select owner from dba_objects
where object_name = 'DBA_TABLES' and object_type = 'SYNONYM';

select distinct object_type from dba_objects
where owner = 'ORAUSER';

select count(distinct object_type) from dba_objects;

select count(*) from dba_tab_columns
where table_name = 'EMP' and owner = 'NIKOVITS';