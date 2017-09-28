 select file_name, bytes
 from dba_data_files
 union
 select file_name, bytes
 from dba_temp_files
 order by bytes desc;
 
 select * from dba_tablespaces;
 
 select tablespace_name, sum(bytes), count(file_name) from
 (select tablespace_name, bytes, file_name
 from dba_data_files
 union
 select tablespace_name, bytes, file_name
 from dba_temp_files)
 group by tablespace_name;
 
 create view data_files as (
   (select tablespace_name, bytes, file_name
   from dba_data_files
   union
   select tablespace_name, bytes, file_name
   from dba_temp_files)
 );
 
 select tablespace_name from dba_tablespaces
 minus
 select tablespace_name from dba_data_files;
 
 select * from dba_segments;
 select * from dba_extents;
 
 select * from
 (select segment_name, owner, sum(bytes) 
 from dba_extents
 where segment_type = 'TABLE'
 group by segment_name, owner
 order by sum(bytes) desc)
 where rownum = 1;
 
 select * from
 (select segment_name, owner, sum(bytes) 
 from dba_extents
 where segment_type = 'INDEX'
 group by segment_name, owner
 order by sum(bytes) desc)
 where rownum = 1;
 
 select owner from
 (select owner, sum(bytes)
 from dba_extents
 group by owner
 order by sum(bytes) desc)
 where rownum <= 2;
 
 select *
 from dba_data_files join dba_extents on dba_extents.file_id = dba_data_files.file_id
 where file_name like '%users01.dbf';
 
 select dba_data_files.file_id, sum(dba_data_files.bytes)
 from dba_data_files join dba_free_space on dba_free_space.FILE_ID = dba_data_files.FILE_ID
 where file_name like '%users01.dbf'
 group by dba_data_files.file_id;
 
 select dba_data_files.file_id, sum(dba_data_files.bytes)/sum(dba_extents.bytes)
 from dba_data_files join dba_extents on dba_extents.FILE_ID = dba_data_files.FILE_ID
 where file_name like '%users01.dbf'
 group by dba_data_files.file_id;
 
 -- Missing sum(byte) but meh
 create or replace PROCEDURE regi_tabla(p_user VARCHAR2) IS 
  cursor curs is select * from dba_objects where owner = p_user and object_type = 'TABLE' order by created desc;
  rec curs%rowtype;
 begin
  open curs;  
  fetch curs into rec;
  dbms_output.put_line(rec.object_name || ' | ' || rec.created);
  close curs;
 end;
 
 SET SERVEROUTPUT ON;
 CALL regi_tabla('SH');