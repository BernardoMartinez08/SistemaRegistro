#pragma once
#ifndef PLAN_ESTUDIO_H
#define PLAN_ESTUDIO_H
#include <iostream>
using namespace std;

struct materia
{
	char* codigo;
	char* nombre;
	short int uv;
	short int anio;
	short int periodo;
	int cantidadHijos;
	int cantidadPadres;
	materia** hijos;
	materia** padres;

	materia(const char* _codigo, const char* _nombre, int _uv, int _anio, int _periodo, materia** _padres, int _cantidadPadres) {
		codigo = new char[strlen(_codigo)];
		strcpy_s(codigo, strlen(_codigo) + 1, _codigo);

		nombre = new char[strlen(_nombre)];
		strcpy_s(nombre, strlen(_nombre) + 1, _nombre);

		uv = _uv;
		anio = _anio;
		periodo = _periodo;
		cantidadHijos = 0;
		cantidadPadres = _cantidadPadres;
		hijos = nullptr;

		padres = _padres;
	}

	void getDatos() {
		cout << "\nDatos de Asignatura:\nCodigo: " << codigo << "\tNombre: " << nombre << "\nUV's: " << uv << "\tAnio: " << anio << "\tPeriodo: " << periodo;
		cout << "\nDatos de Requisitos de " << nombre << ":\n";

		if (cantidadPadres > 0) {
			for (int i = 0; i < cantidadPadres; i++) {
				cout << "\nCodigo: " << padres[i]->codigo << "\tNombre : " << padres[i]->nombre;
			}
		}
		else
			cout << "***SIN REQUISITOS***";
		cout << "\n\n";
	}

	void setCodigo(const char* _codigo) {
		codigo = new char[strlen(_codigo)];
		strcpy_s(codigo, strlen(_codigo) + 1, _codigo);
		cout << "\nACTUALIZADO!!\n";
	}

	void setNombre(const char* _nombre) {
		nombre = new char[strlen(_nombre)];
		strcpy_s(nombre, strlen(_nombre) + 1, _nombre);
		cout << "\nACTUALIZADO!!\n";
	}

	void setAnio(int _anio) {
		this->anio = _anio;
		cout << "\nACTUALIZADO!!\n";
	}

	void setPeriodo(int _periodo) {
		this->periodo = _periodo;
		cout << "\nACTUALIZADO!!\n";
	}

	void setUV(int _uv) {
		this->uv = _uv;
		cout << "\nACTUALIZADO!!\n";
	}
};

struct PlanArchivo {
	char codigoPlan[4];
	char nombrePlan[20];
	int totalClases;
	int totalUnidades;
	int totalBloques;
	int totalRaices;
};

struct MateriaArchivo {
	char codigo[8];
	char nombre[25];
	short int uv;
	short int anio;
	short int periodo;
	int cantidadHijos;
	int cantidadPadres;
	//char** padres;
	//int hijos;
	//int padres;
};

struct HijoFile {
	char codigo[8];
};

class PlanEstudio {
public:
	PlanEstudio(const char*, const char*);
	PlanEstudio();

	//codigoPlan, nombrePlan, codigoMateria, nombreMateria, uvMateria, anioMateria, periodoMateria, padres, cantidadPadres
	void agregarMateria(const char*, const char*, const char*, const char*, int, int, int, materia**, int);
	void agregarMateriaFromFile(const char*, const char*, const char*, const char*, int, int, int, materia**, int);
	void eliminarMateria(const char*);
	void editarMateria(const char*);

	materia* buscarMateria(const char*);
	materia* buscarRaiz(const char*);
	materia** buscarPadres(materia**,int, const char*);
	materia** buscarHijos(materia**, materia*,int);

	void actualizarTotalClases(materia**, materia*);
	void actualizarTotalUV(materia**, materia*);
	void actualizarTotalBloques();

	void imprimir();

	char* getCodigo();
	char* getNombre();

	int buscarMateriaArchivo(const char*,const char*);
	bool existePlan(const char*);
	char* nombrePlanFile(const char*);


	void reescribirArchivoPlan();
	void reescribirMaterias(int);

private:
	materia** raices;
	int cantidadRaices;

	char* codigoPlan;
	char* nombrePlan;
	int totalClases;
	int totalUnidades;
	int totalBloques;

	bool estaVacio();
	materia* buscarMateriaRec(materia**, int, const char*);
	void imprimirRec(materia**, materia*);

	void reescribirMateriasRec(materia**, materia*, int);
};
#endif // !PLAN_ESTUDIO_H