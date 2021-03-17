#pragma once
#ifndef GESTOR_MATRICULA_H
#define GESTOR_MATRICULA_H

#include <iostream>
#include "PlanEstudio.h"
using namespace std;

struct alumno {
	int cuenta;
	char nombre[20];
	char codigoPlan[4];
	float indiceAcademico;
	int clasesAprobadas;
};

struct nota {
	int cuentaAlumno;
	char codigoClase[6];
	float notaClase = 0;
	short int anioMatricula = 0;
	short int periodoMatricula = 0;
};

struct matriculas {
	char codigoPlan[4];
	int cuentaAlumno;
	char codigoClase1[6];
	char codigoClase2[6];
	char codigoClase3[6];
	
};


class EntidadEducativa {
public:
	static void agregarAlumnos();
	static void consultarAlumnos();

	static alumno* buscarAlumno(int);

	static void agregarNotas(int, const char*, float, int, int);
	static void consultarNotas();

	static void matricula();

	static bool aprobada(int,const char*);

	static bool existeAlumno(int);
	static char* getPlanAlumno(int);

	static int obtenerUV(const char*,const char*);
	static void actualizarPromedio(int);
};
#endif // !1
