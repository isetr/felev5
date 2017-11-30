#include <cstdlib>

#include "pvm3.h"

int main(int argc, char** argv) {
    int tid = pvm_mytid();
    int ptid = pvm_parent();
    int sum;
    int setsize;
    int* set;
    int result;
    
    pvm_recv(ptid, -1);
    pvm_upkint(&setsize, 1, 1);
    pvm_upkint(set, setsize, 1);
    pvm_upkint(&sum, 1, 1);
    pvm_upkint(&result, 1, 1);

    if (sum == 0) {
        result *= 0;
    } else if(setsize == 0) {
        result *= 1;
    } else {
        int r1, r2;    
        int tid1, tid2;
        int ss = setsize - 1;
        int sum1 = sum - set[setsize - 1];
        
        if(ss == 0) {
            r1 = (sum1 == 0)?0:1;
        } else {
            if(pvm_spawn(const_cast<char*>("child"), (char**)0, 0, nullptr, 1, &tid1) < 1) std::exit(-1);
            pvm_initsend(PvmDataDefault);
            pvm_pkint(&ss, 1, 1);
            pvm_pkint(set, setsize - 1, 1);
            pvm_pkint(&sum1, 1, 1);
            pvm_pkint(0, 1, 1);
            pvm_send(tid1, 0);

            pvm_recv(tid1, -1);
            pvm_upkint(&r1, 1, 1);
        }

        if(sum1 == 0) {
            r2 = 0;
        } else {
            if(pvm_spawn(const_cast<char*>("child"), (char**)0, 0, nullptr, 1, &tid2) < 1) std::exit(-1);
            pvm_initsend(PvmDataDefault);
            pvm_pkint(&ss, 1, 1);
            pvm_pkint(set, setsize - 1, 1);
            pvm_pkint(&sum, 1, 1);
            pvm_pkint(0, 1, 1);
            pvm_send(tid2, 0);

            pvm_recv(tid2, -1);
            pvm_upkint(&r2, 1, 1);
        }
        
        result *= r1 * r2;
    }

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&result, 1, 1);
    pvm_send(ptid, 1);
    pvm_exit();
    return 0;
}