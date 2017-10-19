 create table emp as (select * from nikovits.emp);
 
 CREATE UNIQUE INDEX  emp1 ON emp (ename);
 CREATE INDEX         emp2 ON emp (empno, sal DESC);
 CREATE INDEX         emp3 ON emp (empno, sal) REVERSE;
 CREATE INDEX         emp4 ON emp (empno, ename, sal) COMPRESS 2;
 CREATE BITMAP INDEX  emp5 ON emp (deptno);
 CREATE INDEX         emp6 ON emp (SUBSTR(ename, 2, 2), job);
 
 