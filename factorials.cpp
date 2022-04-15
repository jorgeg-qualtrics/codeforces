#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long int i64;

pair <i64,i64> conjuntos[1<<12]; 

i64 max_bit_cal(i64 mask){
    if(mask == 0) return 0;
    i64 cont = 0;
    
    mask>>=1;

    while (mask){
        mask>>=1;
        cont++;
    }

    return cont;
}

i64 get_bitcount_man(i64 act){
    i64 cont = 0;
    while (act){
        cont+=(act & 1);
        act>>=1;
    }
    return cont;
}

void pre_calc(){
    i64 fact[14];
    fact[0] = 6;
    i64 posicion = 4;
    
    for (int i = 1; i < 14; i++){
        fact[i] = fact[i-1]*posicion;
        posicion++;
    }
    conjuntos[0].first = 0; 
    conjuntos[0].second = 0; 
    for (int mask = 1; mask < (1 << 12); ++mask){
        i64 first_bit = max_bit_cal(mask);
        i64 calculado = mask ^ (1<<first_bit);
        conjuntos[mask].first = conjuntos[calculado].first + fact[first_bit];
        conjuntos[mask].second = conjuntos[calculado].second + 1;
    }
}

i64 solver(i64 act){
    i64 res = get_bitcount_man(act);

    for (int i = 0; i < (1<<12); ++i){
        if(conjuntos[i].first <= act)res = min(res,conjuntos[i].second + get_bitcount_man(act - conjuntos[i].first));    
        else break;
    }
    return res;
}

int main(){
    int n; cin>>n;

    pre_calc();

    while (n--){
    i64 aux;
        cin>>aux;
        cout<<solver(aux)<<endl;
    }
    
    return 0;
}
