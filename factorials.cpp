#include <iostream>
#include <vector>
using namespace std;

typedef long long lli;
const lli MAXAI = 1000000000000ll;

// Obtener la potencia de dos mas grande que cabe en n
// Lo que es equivalente es a el bit prendido mas a la izquierda.
// Le llaman el most significant bit (msb)
// https://stackoverflow.com/questions/17027878/algorithm-to-find-the-most-significant-bit
// https://www.geeksforgeeks.org/find-significant-set-bit-number/
// Mas informacion sobre estas funciones en: https://en.wikipedia.org/wiki/Find_first_set
int get_first_bit(lli n) {
	if (n == 0) return 0;

	// 1
	// 2^0 = 1 se divide una vez antes del while loop para considerar este caso
	int msb = 0;
	n = n / 2; // n >>= 1; ... equivalente a n = n >> 1;
	while (n != 0) {
		n = n / 2;
		msb++;
	}
	return msb;
}

int get_bit_count(lli n) {
	int contador = 0;
	while(n) {
		contador += (n & 1);
		n >>= 1;
	}
	return contador;
}

// Precalcula los resultados para todos los estados dentro de los limites
lli fact_sum[1<<16][2];

void dinamica() {
	// Precalcula los factoriales iterativamente sin 1, 2 para no contar duplicados con potencias de dos.
	lli fact[14];
	lli factorial = 6, number = 4;
	int idx = 0;
	while(factorial <= MAXAI) {
		fact[idx] = factorial;
		factorial *= number;
		number++;
		idx++;
	}

	// Usa una mascara de bits para probar todos los subconjuntos
	fact_sum[0][0] = 0;
	fact_sum[0][1] = 0;
	for(int mask = 1; mask < (1 << 12); mask++) {
		int first_bit = get_first_bit(mask);
		// el xor le apaga el bit a la mascara, para utilizar los valores calculados anteriormente
		/// mask = 110101010101  -> el first_bit = 12; porque viene de:100000000000  el xor le quita el bit mas grande -> 010101010101
		int mask_anterior = mask ^ (1 << first_bit); // mask_anterior < mask
		fact_sum[mask][0] = fact_sum[mask_anterior][0] + fact[first_bit];
		fact_sum[mask][1] = get_bit_count(mask); // guarda cuantos factoriales se estan utilizando
	}
}

// Dado un numero tienes que decir cual es la menor cantidad de elementos que tienes que sumar para llegar a n
// Formula recursiva:
/*

12 = 1100 --> 2
11 = 1011 --> 3
10 = 1010 --> 2
9  = 1001 --> 2
6  = 0110 --> 2
5  = 0101 --> 2
4  = 0100 --> 1
3  = 0011 --> 2

Complejidad en ejecucion O(2^n)

   p(n) = por cada subconjunto de factoriales menor a n, guarda entre sumar los factoriales del subconjunto y sumar los bits de lo que sobra

   p(12) tenemos, 1, 2, 6
   Subconjuntos posibles:
    0   1    2    3     4      5      6       7
   {}, {1}, {2}, {6}, {1,2}, {1,6}, {2,6}, {1,2,6}
c   0   1    1    1     2      2     2         3
s	0	1	2	  6      3     7       8       9
r   12  11  10    6     9      5       4       3  ---> contar los bit prendidos
t   2   4   3     3     4      4       3       5  ---> suma de elementos en el subconjunto + bit prendidos en el resto

la solution es el minimo de la ultima fila que en este caso es 2.

	n	1	2	3	4
	fac 1	2	6	24


   Problema parentesis: dado un conjunto de numeros imprimir todos los subconjuntos posbiles.
   Por ejemeplo, dado:
   {1, 2, 3, 4}
    0  1  1  1 --> {2,3,4}

   son:
   0   1     2    3
   {}, {1}, {2}, {3}, {4}, {1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}, {1,2,3}, {1,2,4}, {1,3,4}, {2,3,4}, {1,2,3,4}

   en general dado un conjunto de tamaño n, el número de subconjuntos posibles es 2^n.

   for (m = 0 ; m < (1<<n); m++){ // 1<<n es 2^n
	   si el bit esta prendido en la posicion i, usamos el elemento en la posicion i
   }

   Como imprimirlos
   f(arreglo, usados, indice)
   	cada posicion tiene la opcion de usarlo o no usarlo

C = [a,b]

for (m = 0 ; m < (1<<2); m++){
	cout << "{";
	for(int i = 0; i < 2;i++)
		if (m & (1<<i))
			cout << C[i] << " ";
	cout << "}"
}

00: 0  -> {}
01: 1  -> {a }
10: 2  -> {b }
11: 3  -> {a b }

*/

int main() {
	int t;
	cin >> t;

	for(int test_number = 0; test_number < t; test_number++) {
		lli n;
		cin >> n;
		
		lli res = get_bit_count(n); // Una posible respuesta es ver cuantos bits estan prendidos

		// Intentar con todas las demas combinaciones posibles
		for(int i = 0; i < (1 << 12) ; i++){
			// sobreescribir el minimo siempre y cuando la suma de factoriales sea menor o igual a la n del caso de prueba
			if(fact_sum[i][0] <= n) {
				// i.second tiene el numero de factoriales usados y i.first tiene la suma de esos factoriales
				res = min(res, fact_sum[i][1] + get_bit_count(n - fact_sum[i][0]));
			}
		}
		cout << res << endl;
	}
	return 0;
}
