#pragma once
#ifndef MATRICULA_H
#define MATRICULA_H
#include <iostream>
using namespace std;

struct materiaMatri
{
	char* codigo;
	int nota;
	short int anio;
	short int periodo;
	int cantidadHijos;
	int cantidadPadres;
	materiaMatri** hijos;
	materiaMatri** padres;

	materiaMatri(const char* _codigo, int _nota, int _anio, int _periodo, materiaMatri** _padres, int _cantidadPadres) {
		codigo = new char[strlen(_codigo)];
		strcpy_s(codigo, strlen(_codigo) + 1, _codigo);

		nota = _nota;
		anio = _anio;
		periodo = _periodo;
		cantidadHijos = 0;
		cantidadPadres = _cantidadPadres;
		hijos = nullptr;

		padres = _padres;
	}

	void getDatos() {
		cout << "\nDatos de Asignatura:\nCodigo: " << codigo << "\nNota: " << nota << "\tAnio: " << anio << "\tPeriodo: " << periodo;
		cout << "\nDatos de Requisitos de " << codigo << ":\n";

		if (cantidadPadres > 0) {
			for (int i = 0; i < cantidadPadres; i++) {
				cout << "\nCodigo: " << padres[i]->codigo << "\tNota : " << padres[i]->nota;
			}
		}
		else
			cout << "***SIN REQUISITOS***";
		cout << "\n\n";
	}

	void getHijos() {
		if (cantidadHijos > 0) {
			for (int i = 0; i < cantidadHijos; i++) {
				if(hijos[i]->nota < 60 && this->nota > 60)
					cout << "\nCodigo: " << hijos[i]->codigo;
			}
		}
		cout << "\n\n";
	}
	
	void setNota(int _nota) {
		this->nota = _nota;
		cout << "\nACTUALIZADO!!\n";
	}
};

struct Plan {
	char codigoPlan[4];
};

struct MateriaArchivo2 {
	char codigo[8];
	int nota;
	short int anio;
	short int periodo;
	int cantidadHijos;
	int cantidadPadres;
};

struct HijoFile2 {
	char codigo[8];
};

class Matricula {
public:
	Matricula(const char*);
	Matricula();

	//codigoPlan, codigoMateria, notaMateria, anioMateria, periodoMateria, padres, cantidadPadres
	void agregarMateria(const char*, const char*, int, int, int, materiaMatri**, int);
	void agregarMateriaFromFile(const char*, const char*, int, int, int, materiaMatri**, int);

	materiaMatri* buscarMateria(const char*);
	materiaMatri** buscarPadres(materiaMatri**, int, const char*);
	materiaMatri** buscarHijos(materiaMatri**, materiaMatri*, int);

	void imprimir();
	void imprimirDisponibles();

	char* getCodigo();

	int buscarMateriaArchivo(const char*, const char*);
	bool existePlan(const char*);

	void reescribirMaterias();

private:
	materiaMatri** raices;
	int cantidadRaices;

	char* codigoPlan;

	bool estaVacio();
	materiaMatri* buscarMateriaRec(materiaMatri**, int, const char*);

	void imprimirRec(materiaMatri**, materiaMatri*);
	void imprimirDisponiblesRec(materiaMatri**, materiaMatri*);

	void reescribirMateriasRec(materiaMatri**, materiaMatri*, int);
};
#endif // !MATRICULA_H