#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <NTL/ZZ.h>
#include <stdlib.h>

using namespace std;

NTL_CLIENT

void test_open(ifstream& fin) {
	while (!fin.is_open()) {
		cout << "Ошибка! Повторите ввод.\n";
		cin.clear(); 
		cin.ignore(1000, '\n');
	}
}

void Generation(long& e, long& d, ZZ& n) {
	vector<int> prime, key_e;
	int size, p, q, f;

	for (int i = 11; i < 1000; i++)
		for (int j = 2; j * j <= i; j++) {
			if (i % j == 0)
				break;
			else
				if (j + 1 > sqrt(i))
					prime.push_back(i);
		}

	size = prime.size();
	p = prime[rand() % size];
	q = prime[rand() % size];

	n = p * q;
	f = (p - 1) * (q - 1);

	for (auto e : prime)
		if ((e < f) && (f % e != 0))
			key_e.push_back(e);
	size = key_e.size();
	e = key_e[rand() % size];
	cout << "Открытый ключ: (" << e << ", " << n << ")\n";

	d = 0;
	while (true) {
		d++;
		if ((d * e) % f == 1)
			break;
	}
	cout << "Закрытый ключ: (" << d << ", " << n << ")\n";

	ofstream fout_key_e("key_e.txt", ios::binary);
	ofstream fout_key_d("key_d.txt", ios::binary);
	fout_key_e << e << "\t" << n;
	fout_key_d << d << "\t" << n;
	fout_key_e.close();
	fout_key_d.close();
	cout << "Ключи сохранены в key_e.txt и key_d.txt!\n";
}

void Load(long& e, long& d, ZZ& n) {
	ifstream fin_key_e("key_e.txt", ios::binary);
	ifstream fin_key_d("key_d.txt", ios::binary);
	test_open(fin_key_e);
	test_open(fin_key_d);
	fin_key_e >> e >> n;
	fin_key_d >> d >> n;
	fin_key_e.close();
	fin_key_d.close();
	cout << "Ключи загружены!\n";
}

ZZ binpow(ZZ a, long n, ZZ m) {
	ZZ res; 
	res = 1;
	while (n) {
		if (n & 1) {
			res *= a; 
			res %= m;
		}
		a *= (a % m); 
		a %= m; 
		n >>= 1;
	}
	return res % m;
}

void Crypt(long& e, long& d, ZZ& n, int action) {
	string filename;
	char s1, s2, s3;
	unsigned char us1, us2, us3;
	ZZ code, const_code;
	int number;

	cout << "Введите имя файла для считывания текста:\n";
	cin >> filename;
	ifstream fin(filename, ios::binary);
	test_open(fin);
	if (action == 3) {
		ofstream fout_e("encoded.txt", ios::binary);
		while (fin.get(s1)) {
			us1 = s1;
			code = int(us1);
			code = binpow(code, e, n);
			conv(number, code);
			fout_e << unsigned char((number / 256) / 256) << unsigned char((number / 256) % 256) << unsigned char(number % 256);
		}
		fin.close();
		fout_e.close();
		cout << "Зашифрованный текст сохранён в encoded.txt!\n";
	}
	else {
		ofstream fout_d("decoded.txt", ios::binary);
		while ((fin.get(s1)) && (fin.get(s2)) && (fin.get(s3))) {
			us1 = s1; us2 = s2; us3 = s3;
			code = int(us3) + (int(us2) + int(us1) * 256) * 256;
			code = binpow(code, d, n);
			conv(number, code);
			fout_d << unsigned char(number);
		}
		fin.close();
		fout_d.close();
		cout << "Расшифрованный текст сохранён в decoded.txt!\n";
	}
}

void Menu() {
	cout << "\n--------------------------\n";
	cout << "1) Сгенерировать пару открытый/закрытый ключ\n";
	cout << "2) Загрузить из файла пару открытый/закрытый ключ\n";
	cout << "3) Зашифровать текст\n";
	cout << "4) Расшифровать текст\n";
	cout << "0) Выйти\n";
	cout << "--------------------------\n\n";
}

void Choose() {
	int action;
	long e, d;
	ZZ n;
	while (true) {
		Menu();
		cin >> action;
		switch (action) {
		case 1: {Generation(e, d, n); break; }
		case 2: {Load(e, d, n); break; }
		case 3: {Crypt(e, d, n, action); break; }
		case 4: {Crypt(e, d, n, action); break; }
		case 0: {return; break; }
		default: {cout << "Нет такого пункта!!!\n"; break; }
		}
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(static_cast<unsigned int>(time(NULL)));
	Choose();
	return 0;
}