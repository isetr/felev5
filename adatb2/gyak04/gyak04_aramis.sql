 -- Copy only the the scheme
 create table teszt as select * from nikovits.szeret where 1=0;
 
 select * from teszt;
 
 select rownum from nikovits.szeret;
 
 select rownum, szeret.* from nikovits.szeret;
 select rowid, szeret.* from nikovits.szeret;
 
 SELECT dbms_rowid.rowid_object(ROWID) adatobj, 
        dbms_rowid.rowid_relative_fno(ROWID) fajl,
        dbms_rowid.rowid_block_number(ROWID) blokk,
        dbms_rowid.rowid_row_number(ROWID) sor,
        nev
 FROM nikovits.szeret;
 
 -- wrong
 select distinct dbms_rowid.rowid_block_number(rowid) from nikovits.cikk;
 
 -- wrong
 select owner, table_name, blocks
 from dba_tables 
 where owner = 'NIKOVITS'
 and table_name = 'CIKK';
 
 -- correct
 select owner, segment_name, blocks
 from dba_extents
 where owner = 'NIKOVITS'
 and segment_name = 'CIKK';
 
 -- correct
 select owner, segment_name, blocks
 from dba_segments
 where owner = 'NIKOVITS'
 and segment_name = 'CIKK';
 
 select count(*) from
 (select distinct dbms_rowid.rowid_block_number(rowid) 
 from nikovits.cikk);