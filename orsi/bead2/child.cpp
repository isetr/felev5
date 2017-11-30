#include <vector>

#include "pvm3.h"

int main(int argc, char** argv) {
    int tid = pvm_mytid();
    int ptid = pvm_parent();
    int sum;
    int setsize;
    std::vector<int> set;
    
    pvm_recv(ptid, -1);
    pvm_upkint(&setsize, 1, 1);
    for(int i = 0; i < setsize; ++i) {
        int tmp;
        pvm_upkint(&tmp, 1, 1);
        set.push_back(tmp);
    }
    pcm_upkint(&sum, 1, 1);

    int result;

    if (sum == 0) {
        result = 0;
    } else if(setsize == 0) {
        result = 1;
    } else {
        
    }

    pvm_initsend(PvmDataDefault);
    pvm_pkint(&result, 1, 1);
    pvm_send(ptid, 1);
    pvm_exit();
    return 0;
}