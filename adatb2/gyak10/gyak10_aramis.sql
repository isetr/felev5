 A következõ naplóbejegyzés-sorozat a T és U két tranzakcióra vonatkozik:

 <start T> 
 <T, A, 10> 
 <start U> 
 <U, B, 20> 
 <T, C, 30> 
 <U, D, 40>
 <T, A, 11>
 <U, B, 21>  
 <COMMIT U>
 <T, E, 50> 
 <COMMIT T>
 
 Adjuk meg a helyreállítás-kezelõ tevékenységeit, ha az utolsó lemezre került naplóbejegyzés:
 a) <start U> (undo)
        Write(A, 10)
        Output(A)
        <ABORT, T>
        <ABORT, U>
        FLUSH LOG
 b) <COMMIT U> (undo)
        Write(A, 11)
        Output(A)
        Write(C, 30)
        Output(C)
        Write(A, 10)
        Output(A)
        <ABORT T>
        FLUSH LOG
 b) <COMMIT U> (redo)
        Write(B, 20)
        Write(D, 40)
        Write(B, 21)
        Output(B)
        Output(D)
        <END, U>
        <ABORT, T>
        FLUSH LOG
        