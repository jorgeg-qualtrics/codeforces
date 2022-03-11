#include <bits/stdc++.h>
using namespace std;

const long long MAXAI = 1000000000000ll;


// Mas informacion sobre estas funciones en: https://en.wikipedia.org/wiki/Find_first_set
int get_first_bit(long long n) {
    if (n == 0)
        return 0;
 
    int msb = 0;
    n = n / 2;
    while (n != 0) {
        n = n / 2;
        msb++;
    }
 
    return (1 << msb);
}

int get_bit_count(long long n) {
	int contador = 0;
	while(n) {
		contador += (n & 1);
	}
	return contador;
}

vector<pair<long long, long long>> dinamica() {
		// Precalcula los factoriales iterativamente
		vector<long long> fact;
		long long factorial = 6, number = 4;
		while(factorial <= MAXAI){
			fact.push_back(factorial);
			factorial *= number;
			number++;
		}

		// Usa una mascara de bits para probar todos los subconjuntos
		vector<pair<long long, long long>> fact_sum(1 << fact.size());
		fact_sum[0] = {0, 0};
		for(int mask = 1; mask < (1 << fact.size()); mask++){
			auto first_bit = get_first_bit(mask);
			// el xor le apaga el bit a la mascara, para utilizar los valores calculados anteriormente
			fact_sum[mask].first = fact_sum[mask ^ (1 << first_bit)].first + fact[first_bit];
			fact_sum[mask].second = get_bit_count(mask); // guarda cuantos factoriales se estan utilizando
		}
}

int main() {
	int t;
	cin >> t;

	// Precalcula los resultados para todos los estados dentro de los limites
	vector<pair<long long, long long>> memo = dinamica();

	for(int test_number = 0; test_number < t; test_number++) {
		long long n;
		cin >> n;
		
		long long res = get_bit_count(n); // Una posible respuesta es ver cuantos bits estan prendidos

		// Intentar con todas las demas combinaciones posibles
		for(auto i : memo) {
			// sobreescribir el minimo siempre y cuando la suma de factoriales sea menor o igual a la n del caso de prueba
			if(i.first <= n) {
				// i.second tiene el numero de factoriales usados y i.first tiene la suma de esos factoriales
				res = min(res, i.second + get_bit_count(n - i.first));
			}
		}
		cout << res << endl;
	}
	return 0;
}
