 -- 5. feladat
 -- a)
 
 explain plan set statement_id='f5a'
 for
 select /*+ use_hash(sz, szo, c) */ sum(sz.mennyiseg) 
 from nikovits.szallit sz
 join nikovits.cikk c on sz.ckod = c.CKOD
 join nikovits.szallito szo on sz.szkod = szo.szkod
 where c.szin = 'piros'
 and szo.telephely = 'Pecs';
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f5a'));
 
 -- Végrehajtási terv:
 ---------------------------------------------------------------------------------
 | Id  | Operation            | Name     | Rows  | Bytes | Cost (%CPU)| Time     |
 ---------------------------------------------------------------------------------
 |   0 | SELECT STATEMENT     |          |     1 |    41 |    20   (5)| 00:00:01 |
 |   1 |  SORT AGGREGATE      |          |     1 |    41 |            |          |
 |*  2 |   HASH JOIN          |          |   181 |  7421 |    20   (5)| 00:00:01 |
 |*  3 |    TABLE ACCESS FULL | SZALLITO |     3 |    30 |     3   (0)| 00:00:01 |
 |*  4 |    HASH JOIN         |          |   905 | 28055 |    17   (6)| 00:00:01 |
 |*  5 |     TABLE ACCESS FULL| CIKK     |    91 |   910 |     3   (0)| 00:00:01 |
 |   6 |     TABLE ACCESS FULL| SZALLIT  |  9944 |   203K|    13   (0)| 00:00:01 |
 ---------------------------------------------------------------------------------
 
 Predicate Information (identified by operation id):
 ---------------------------------------------------
 
    2 - access("SZ"."SZKOD"="SZO"."SZKOD")
    3 - filter("SZO"."TELEPHELY"='Pecs')
    4 - access("SZ"."CKOD"="C"."CKOD")
    5 - filter("C"."SZIN"='piros')
    
 -- Eredmény:
 SUM(SZ.MENNYISEG)
 -----------------
              9995
              
 -- 5. feladat
 -- b)
 
 explain plan set statement_id='f5b'
 for
 select /*+ index(sz), no_index(szo, c) */ sum(sz.mennyiseg) 
 from nikovits.szallit sz
 join nikovits.cikk c on sz.ckod = c.CKOD
 join nikovits.szallito szo on sz.szkod = szo.szkod
 where c.szin = 'piros'
 and szo.telephely = 'Pecs';
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f5b'));
 
 -- Végrehajtási terv:
 --------------------------------------------------------------------------------------------
 | Id  | Operation                      | Name      | Rows  | Bytes | Cost (%CPU)| Time     |
 --------------------------------------------------------------------------------------------
 |   0 | SELECT STATEMENT               |           |     1 |    41 |    16   (7)| 00:00:01 |
 |   1 |  SORT AGGREGATE                |           |     1 |    41 |            |          |
 |*  2 |   HASH JOIN                    |           |   181 |  7421 |    16   (7)| 00:00:01 |
 |*  3 |    TABLE ACCESS FULL           | CIKK      |    91 |   910 |     3   (0)| 00:00:01 |
 |   4 |    NESTED LOOPS                |           |       |       |            |          |
 |   5 |     NESTED LOOPS               |           |  1989 | 61659 |    12   (0)| 00:00:01 |
 |*  6 |      TABLE ACCESS FULL         | SZALLITO  |     3 |    30 |     3   (0)| 00:00:01 |
 |*  7 |      INDEX RANGE SCAN          | SZT_SZKOD |    32 |       |     1   (0)| 00:00:01 |
 |   8 |     TABLE ACCESS BY INDEX ROWID| SZALLIT   |   663 | 13923 |     3   (0)| 00:00:01 |
 --------------------------------------------------------------------------------------------
 
 Predicate Information (identified by operation id): 
 ---------------------------------------------------
 
    2 - access("SZ"."CKOD"="C"."CKOD")
    3 - filter("C"."SZIN"='piros')
    6 - filter("SZO"."TELEPHELY"='Pecs')
    7 - access("SZ"."SZKOD"="SZO"."SZKOD")
    
 -- Eredmény:
 SUM(SZ.MENNYISEG)
 -----------------
              9995
 
 -- 6. feladat
 
 explain plan set statement_id='f6'
 for
 select /*+ use_hash(co, cu) */ co.country_name
 from sh.customers cu
 natural join sh.countries co
 where cu.cust_year_of_birth = 'single'
 group by co.country_name;
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f6'));
 
 -- Végrehajtási terv:
 PLAN_TABLE_OUTPUT                                                                                                                                                                                                                                                                                           
 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Plan hash value: 932479326
 
 ----------------------------------------------------------------------------------------------------
 | Id  | Operation                      | Name              | Rows  | Bytes | Cost (%CPU)| Time     |
 ----------------------------------------------------------------------------------------------------
 |   0 | SELECT STATEMENT               |                   |    23 |   552 |   150   (1)| 00:00:02 |
 |   1 |  HASH GROUP BY                 |                   |    23 |   552 |   150   (1)| 00:00:02 |
 |*  2 |   HASH JOIN                    |                   |   740 | 17760 |   149   (0)| 00:00:02 |
 |   3 |    TABLE ACCESS FULL           | COUNTRIES         |    23 |   345 |     3   (0)| 00:00:01 |
 |   4 |    TABLE ACCESS BY INDEX ROWID | CUSTOMERS         |   740 |  6660 |   146   (0)| 00:00:02 |
 |   5 |     BITMAP CONVERSION TO ROWIDS|                   |       |       |            |          |
 |*  6 |      BITMAP INDEX SINGLE VALUE | CUSTOMERS_YOB_BIX |       |       |            |          |
 ----------------------------------------------------------------------------------------------------
 
 Predicate Information (identified by operation id):
 ---------------------------------------------------
 
    2 - access("CU"."COUNTRY_ID"="CO"."COUNTRY_ID")
    6 - access("CU"."CUST_YEAR_OF_BIRTH"=TO_NUMBER('single'))
    
 -- 7. feladat:

 explain plan set statement_id='f7'
 for
 select /*+ full(c), hash_sj */ c.cust_id
 from sh.customers c
 right join sh.sales s on c.cust_id = s.cust_id
 group by c.cust_id
 order by sum(s.amount_sold);
 
 select plan_table_output from table(dbms_xplan.display('plan_table', 'f7'));
 
 -- Végrehajtási terv:
 PLAN_TABLE_OUTPUT                                                                                                                                                                                                                                                                                           
 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Plan hash value: 677263557
 
 -----------------------------------------------------------------------------------------------------
 | Id  | Operation               | Name      | Rows  | Bytes | Cost (%CPU)| Time     | Pstart| Pstop |
 -----------------------------------------------------------------------------------------------------
 |   0 | SELECT STATEMENT        |           | 55500 |   812K|   951   (7)| 00:00:12 |       |       |
 |   1 |  SORT ORDER BY          |           | 55500 |   812K|   951   (7)| 00:00:12 |       |       |
 |   2 |   HASH GROUP BY         |           | 55500 |   812K|   951   (7)| 00:00:12 |       |       |
 |*  3 |    HASH JOIN RIGHT OUTER|           |   918K|    13M|   897   (2)| 00:00:11 |       |       |
 |   4 |     TABLE ACCESS FULL   | CUSTOMERS | 55500 |   270K|   405   (1)| 00:00:05 |       |       |
 |   5 |     PARTITION RANGE ALL |           |   918K|  8973K|   489   (2)| 00:00:06 |     1 |    28 |
 |   6 |      TABLE ACCESS FULL  | SALES     |   918K|  8973K|   489   (2)| 00:00:06 |     1 |    28 |
 -----------------------------------------------------------------------------------------------------
 
 Predicate Information (identified by operation id):
 ---------------------------------------------------
 
    3 - access("C"."CUST_ID"(+)="S"."CUST_ID")