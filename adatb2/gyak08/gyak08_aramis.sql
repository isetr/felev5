 -- expl.txt
 create table PLAN_TABLE (
        statement_id       varchar2(30),
        plan_id            number,
        timestamp          date,
        remarks            varchar2(4000),
        operation          varchar2(30),
        options            varchar2(255),
        object_node        varchar2(128),
        object_owner       varchar2(30),
        object_name        varchar2(30),
        object_alias       varchar2(65),
        object_instance    numeric,
        object_type        varchar2(30),
        optimizer          varchar2(255),
        search_columns     number,
        id                 numeric,
        parent_id          numeric,
        depth              numeric,
        position           numeric,
        cost               numeric,
        cardinality        numeric,
        bytes              numeric,
        other_tag          varchar2(255),
        partition_start    varchar2(255),
        partition_stop     varchar2(255),
        partition_id       numeric,
        other              long,
        distribution       varchar2(30),
        cpu_cost           numeric,
        io_cost            numeric,
        temp_space         numeric,
        access_predicates  varchar2(4000),
        filter_predicates  varchar2(4000),
        projection         varchar2(4000),
        time               numeric,
        qblock_name        varchar2(30),
        other_xml          clob
 );
 
 explain plan set statement_id='utl'
 for
 select avg(fizetes) from nikovits.dolgozo;
 
 select * from plan_table;
 
 SELECT LPAD(' ', 2*(level-1))||operation||' + '||options||' + '||object_name terv
 FROM plan_table
 START WITH id = 0 AND statement_id = 'ut1'                 -- az utasítás neve szerepel itt
 CONNECT BY PRIOR id = parent_id AND statement_id = 'ut1'   -- meg itt
 ORDER SIBLINGS BY position;

 select plan_table_output from table(dbms_xplan.display('plan_table','ut1','all'));

 select plan_table_output from table(dbms_xplan.display());
 
 -- hintek.txt
 explain plan set statement_id='test1'
 for
 select * from nikovits.osztaly o, nikovits.dolgozo d
 where o.oazon = d.oazon;
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'test1'));
 
 explain plan set statement_id='test2'
 for
 select /*+ use_nl(o, d) */ * from nikovits.osztaly o, nikovits.dolgozo d
 where o.oazon = d.oazon;
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'test2'));
 
 delete from plan_table
 where statement_id='test2';
 
 explain plan set statement_id='f1'
 for
 SELECT sum(darab) FROM nikovits.hivas, nikovits.kozpont, nikovits.primer
 WHERE hivas.kozp_azon_hivo=kozpont.kozp_azon AND kozpont.primer=primer.korzet
 AND primer.varos = 'Szentendre' 
 AND datum + 1 = next_day(to_date('2012.01.31', 'yyyy.mm.dd'),'hétfõ');
 
 explain plan set statement_id='f2'
 for
 SELECT sum(darab) FROM nikovits.hivas, nikovits.kozpont, nikovits.primer
 WHERE hivas.kozp_azon_hivo=kozpont.kozp_azon AND kozpont.primer=primer.korzet
 AND primer.varos = 'Szentendre' 
 AND datum = next_day(to_date('2012.01.31', 'yyyy.mm.dd'),'hétfõ') - 1;
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f1'));
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f2'));
 
 -- feladat7.txt
 create table dolgozo as select * from nikovits.dolgozo;
 create table osztaly as select * from nikovits.osztaly;
 create table fiz_kategoria as select * from nikovits.fiz_kategoria;
 
 explain plan set statement_id='fizkat1'
 for
 select onev
 from(
 select d.oazon, f.kategoria, o.onev, count(*)
 from dolgozo d 
 join osztaly o on d.oazon = o.oazon 
 join fiz_kategoria f on d.fizetes between f.also and f.felso
 group by d.oazon, f.kategoria, o.onev
 having f.kategoria = 1
 and count(*) >= 1); 
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'fizkat1'));
 
 create index oazon_index on dolgozo (oazon);
 create index ooazon_index on osztaly (oazon);
 create index kat_index on fiz_kategoria (kategoria);
 
 explain plan set statement_id='fizkat2'
 for
 select onev
 from(
 select d.oazon, f.kategoria, o.onev, count(*)
 from dolgozo d 
 join osztaly o on d.oazon = o.oazon 
 join fiz_kategoria f on d.fizetes between f.also and f.felso
 group by d.oazon, f.kategoria, o.onev
 having f.kategoria = 1
 and count(*) >= 1); 
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'fizkat2', 'all'));
 