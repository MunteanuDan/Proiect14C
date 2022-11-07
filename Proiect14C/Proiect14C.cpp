#include <iostream>
#include<iomanip> // ce puntem intre << si >>
#include<fstream>
using namespace std;

class Angajat
{
	char* nume; // vector de caractere
	float salariu;
	const int tipContract; // 1 - determinata, 0 - nedeterminata
	int* pontaj;
	int numarZile;

public:
	Angajat() : tipContract(1)
	{
		nume = NULL;
		salariu = 0;
		pontaj = NULL;
		numarZile = 0;
	}

	Angajat(const char* n, float s, int t) : tipContract(t)
	{
		if (t == 0 || t == 1)
		{
			nume = new char[strlen(n) + 1];
			strcpy(nume, n);
			salariu = s;
			pontaj = NULL;
			numarZile = 0;
		}
		else
		{
			throw new exception("Tipul contractului poate fi doar 1 sau 0.");
		}
	}

	Angajat(const char *n, float s, int t, int* p, int nr) : tipContract(t)
	{
		if (t == 0 || t == 1)
		{
		nume = new char[strlen(n) + 1];
		strcpy(nume, n);
		salariu = 5;
		if (nr > 0 && p != NULL)
		{
			pontaj = new int[nr]; // asa aloc memorie pt *int, adica pt vectorii de intregi
			numarZile = nr;
			for (int i = 0; i < nr; i++)
			{
				pontaj[i] = p[i];
			}
		}
		else
		{
			throw new exception("Pontajul nu poate fi vid.");
		}
		}
		else
		{
			throw new exception("Tipul contractului poate fi doar 1 sau 0.");
		}
	}

	// constructor de copiere
	Angajat(const Angajat& a) : tipContract(a.tipContract)
	{
		if (a.tipContract == 0 || a.tipContract == 1)
		{
			nume = new char[strlen(a.nume) + 1];
			strcpy(nume, a.nume);
			salariu = a.salariu;
			if (a.numarZile > 0 && a.pontaj != NULL)
			{
				pontaj = new int[a.numarZile];
				numarZile = a.numarZile;
				for (int i = 0; i < numarZile; i++)
				{
					pontaj[i] = a.pontaj[i];
				}
			}
			else
			{
				throw new exception("Pontajul nu poate fi vid!");
			}
		}
		else
		{
			throw new exception("Tipul contractului poate fi doar 1 sau 0.");
		}

	}

	~Angajat()
	{
		if (nume!= NULL)
		{
			delete[] nume;
		}
		if (pontaj != NULL)
		{
			delete[] pontaj;
		}
	}

	Angajat& operator=(const Angajat& a)
	{
		if (nume != NULL)
		{
			delete[] nume;
		}
		nume = new char[strlen(a.nume) + 1];
		strcpy(nume, a.nume);
		salariu = a.salariu;
		if (a.numarZile > 0 && a.pontaj != NULL)
		{
			if (pontaj != NULL)
			{
				delete[] pontaj;
			}
			pontaj = new int[a.numarZile];
			numarZile = a.numarZile;
			for (int i = 0; i < numarZile; i++)
			{
				pontaj[i] = a.pontaj[i];
			}
		}
		else
		{
			throw new exception("Pontajul nu poate fi vid!");
		}
		return *this;
	}

	int& operator[](int i) // operatorul []
	{
		if (i >= 0 && i < numarZile)
		{
			return pontaj[i];
		}
		else throw new exception("Index invalid!");
	}

	char* getNume()
	{
		return nume;
	}

	void setNume(char* n)
	{
		if (nume != NULL) // dezaloc memorie pt vechiul nume
		{
			delete[] nume;
		}
		nume = new char[strlen(n) + 1];
		strcpy(nume, n);
	}

	float getSalariu()
	{
		return salariu;
	}

	void setSalariu(float s)
	{
		salariu = s;
	}

	int getTipContract()
	{
		return tipContract;
	}

	friend istream& operator>>(istream&, Angajat&); // pt a citi angajati de la tastatura
	friend ifstream& operator>>(ifstream&, Angajat&); // pt a citi din fisier
	friend ofstream& operator<<(ofstream&, Angajat); // pt a scrie in fisier

	void scrierePontajFisier()
	{
		fstream f;
		f.open("pontaj.dat", ios::out | ios::binary);
		if (f)
		{
			for (int i = 0; i < numarZile; i++)
			{
				f.write((char*)&pontaj[i], sizeof(int));
			}
		}
		f.close();
	}

	static void citirePontajFisier()
	{
		fstream f;
		f.open("pontaj.dat", ios::in | ios::binary);
		cout << "Pontaj: ";
		if (f)
		{
			int x;
			while (f.read((char*)&x, sizeof(int)))
			{
				cout << x << " ";
			}
		}
		cout << endl;
		f.close();
	}

	static void citirePontajFisier(int i)
	{
		fstream f;
		f.open("pontaj.dat", ios::in | ios::binary);
		if (f)
		{
			if (i > 0)
			{
				int x;
				f.seekg(i * sizeof(int), ios::beg);
				f.read((char*)&x, sizeof(int));
				cout << "Pontajul pe ziua " << (i + 1) << " este: " << x;
			}
		}
		cout << endl;
		f.close();
	}


};

istream& operator>>(istream& intrare, Angajat& a) // citirea de la tastatura, operatorul >>
{
	cout << "Nume: ";
	char n[40];
	intrare.getline(n, 40); // primul param este variabila in care se citeste, al doilea este lungimea in octeti a spatiului de citire
	a.setNume(n);
	return intrare;
}

ifstream& operator>>(ifstream& f, Angajat& a) // citirea dintr-un fisier
{
	f.open("nume.txt", ios::in | ios::_Nocreate); // deschid fisierul, fara modalitatea de creare daca acesta nu exista
	if (f)
	{
		char n[40];
		f.getline(n, 40);
		a.setNume(n);
	}
	f.close();
	return f;
}

ofstream& operator<<(ofstream& f, Angajat a)
{
	f.open("afisare.txt", ios::out); // deschid fisierul
	if (f) // verific daca s-a reusit deschiderea
	{
		f << "Nume: " << a.nume << endl;
		f << "Salariu: " << setfill('$') << setw(4) << a.salariu; // setw(4) - setez latimea de afisare 4
															// setfill('$') - umple cu valoarea asta daca nu avem valori
	}
	f.close();
	return f;
}

void main()
{
	Angajat a;
	cin >> a; // citesc angajatul, in cazul nostru numele, de la tastatura
	Angajat b("Ionescu", 680, 1);
	int v[5] = { 4, 4, 8, 4, 8 };
	Angajat c("Popescu", 800, 0, v, 5);
	cout.put(c.getNume()[0]);
	cout << endl << setfill('$') << setw(10) << c.getSalariu() << endl; // aici avem nevoie de iomanip
	Angajat d = c;
	Angajat e;
	e = c;
	Angajat g;
	ifstream f;
	f >> g;
	ofstream x;
	x << c;
	c.scrierePontajFisier();
	Angajat::citirePontajFisier();
	Angajat::citirePontajFisier(2);

}
