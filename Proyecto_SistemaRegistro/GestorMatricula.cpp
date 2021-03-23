#include "GestorMatricula.h"
#include <fstream>
#include <iostream>
#include "PlanEstudio.h"
#include "Matricula.h"
using namespace std;
PlanEstudio* plan;

void EntidadEducativa::agregarAlumnos() {
	ofstream alumnoFile("alumnos.dat", ios::out | ios::app | ios::binary);

	if (!alumnoFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	cout << " ***** I N G R E S O  D E  A L U M N O S ***** \n\n";

	cout << "Indique cantidad de alumnos a ingresar: ";
	int cantidadAlumnos = 0;
	cin >> cantidadAlumnos;

	alumno nuevo;

	for (int i = 0; i < cantidadAlumnos; i++) {
		char* codigo = new char[6];

		cout << "\nAlumno " << (i + 1) << ": \n";

		cout << "Indique numero de Cuenta del Alumno: ";
		cin >> nuevo.cuenta;

		cout << "Indique el nombre del Alumno:";
		cin >> nuevo.nombre;

		bool cancel = false;
		while (plan->existePlan(codigo) != true && cancel == false) {
			cout << "Indique el Codigo del Plan de Clases del Alumno:";
			cin >> codigo;

			if (strcmp(codigo, "-1"))
				cancel == true;

			if (cancel != true) {
				if (plan->existePlan(codigo)) {
					strcpy_s(nuevo.codigoPlan, strlen(codigo) + 1, codigo);
					nuevo.indiceAcademico = 0;
					nuevo.clasesAprobadas = 0;

					alumnoFile.write(reinterpret_cast<const char*>(&nuevo), sizeof(alumno));

					cout << "\nALUMNO AGREGADO CON EXITO!!!!\n";
				}
				else {
					cout << "\nNO EXISTE EL PLAN DE CLASES\nEscriba -1 para CANCELAR INSCRIPCION\n";
				}
			}
			else {
				cout << "\nINSCRIPCION CANCELADA\n";
			}
		}
	}

	alumnoFile.close();
}

void EntidadEducativa::consultarAlumnos() {
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	cout << " ***** C O N S U L T A  D E  A L U M N O S ***** \n\n";
	
	alumnosFile.seekg(0, ios::beg);

	alumno actual;
	alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));

	while (!alumnosFile.eof()) {
		cout << "Alumno: {Cuenta: " << actual.cuenta << ",\tNombre: " << actual.nombre << ", Promedio: " << actual.indiceAcademico << ", Clases Aprobadas: " << actual.clasesAprobadas << ", Plan de Clases: " << actual.codigoPlan << " }\n";

		alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));
	}

	alumnosFile.close();
}

alumno* EntidadEducativa::buscarAlumno(int _codigo) {
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return nullptr;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno* actual;
	alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actual->cuenta == _codigo)
			return actual;

		alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));
	}

	alumnosFile.close();
	return nullptr;
}


bool EntidadEducativa::existeAlumno(int _Cuenta) {
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return false;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _Cuenta) {
			alumnosFile.close();
			return true;
		}

		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
	return false;
}

char* EntidadEducativa::getPlanAlumno(int _Cuenta) {
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return nullptr;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _Cuenta) {
			
			alumnosFile.close();
			return actualAlumno.codigoPlan;
		}

		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
	return nullptr;
}




void EntidadEducativa::agregarNotas(int _cuentaAlu, const char* _codigoClas, float _nota, int _anio, int _periodo) {
	ofstream notasFile("notas.dat", ios::out | ios::app | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	nota nuevaNota;

	//BUSCAR EL CODIGO DE MATERIA EN EL ARCHIVO DE ALUMNOS
	char* codigo = nullptr; 
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _cuentaAlu) {
			codigo = actualAlumno.codigoPlan;
			alumnosFile.close();
		}

		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
	
	if (codigo == nullptr) {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
		return;
	}

	//VERIFICAR SI DICHO PLAN EXISTE
	PlanEstudio aux;
	char nombre[20];
	string name = (string)codigo + "_plan.dat";

	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "\nERROR PLAN NO EXISTE!!!!\n";
		return;
	}
	else {
		PlanArchivo actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));
		strcpy_s(nombre,strlen(actualPlan.nombrePlan)+1, actualPlan.nombrePlan);
	}


	PlanEstudio PlanClases(codigo, nombre);

	//BUSCAR MATERIA EN EL ARCHIVO
	bool Existeclase = false;
	MateriaArchivo actualMateria;

	PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));

	while (!PlanFile.eof()) {
		if (strcmp(actualMateria.codigo, _codigoClas) == 0) {
			Existeclase = true;
		}

		for (int i = 0; i < actualMateria.cantidadPadres; i++) {
			HijoFile hijoFile;
			PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));
		}

		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
	}

	PlanFile.close();


	if (Existeclase != false) {
		if (existeAlumno(_cuentaAlu) == true) {
			strcpy_s(nuevaNota.codigoClase, strlen(_codigoClas) + 1, _codigoClas);
			nuevaNota.cuentaAlumno = _cuentaAlu;
			nuevaNota.notaClase = _nota;
			nuevaNota.anioMatricula = _anio;
			nuevaNota.periodoMatricula = _periodo;

			notasFile.write(reinterpret_cast<const char*>(&nuevaNota), sizeof(nota));
			notasFile.close();

			actualizarAprobadas(_cuentaAlu);
			actualizarPromedio(_cuentaAlu);
		}
		else {
			cout << "\nERROR NO EXISTE ALUMNO!!!!\n";
		}
		cout << "\nNOTA AGREGADA CON EXITO!!!!\n";
	}
	else {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
	}
}

void EntidadEducativa::consultarNotas() {
	ifstream notasFile("notas.dat", ios::in | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	cout << " ***** C O N S U L T A  D E  N O T A S ***** \n\n";

	notasFile.seekg(0, ios::beg);

	nota actual;
	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		cout << "Nota { Alumno: " << actual.cuentaAlumno << " " << ",\tClase: "
			<< actual.codigoClase << ", Nota: " << actual.notaClase << " }\n";

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	notasFile.close();
}

int EntidadEducativa::obtenerUV(const char* _codigoPlan, const char* _codigoMateria) {
	string name = (string)_codigoPlan + "_plan.dat";
	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return -1;
	}

	//BUSCAR MATERIA EN EL ARCHIVO
	int UV = 0;
	MateriaArchivo actualMateria;

	PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));

	while (!PlanFile.eof()) {
		if (strcmp(actualMateria.codigo, _codigoMateria) == 0) {
			return actualMateria.uv;
		}

		for (int i = 0; i < actualMateria.cantidadPadres; i++) {
			HijoFile hijoFile;
			PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));
		}

		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
	}

	PlanFile.close();

	return -1;
}



void EntidadEducativa::actualizarPromedio(int _cuentaAlum) {
	ifstream notasFile("notas.dat", ios::in | ios::binary);
	fstream alumnosFile("alumnos.dat", ios::out | ios::in | ios::binary);

	if (!notasFile || !alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return;
	}

	float totalNotas = 0;
	int totalUV = 0;


	//BUSCAR EL CODIGO DE MATERIA EN EL ARCHIVO DE ALUMNOS
	char* codigo = nullptr;
	ifstream alumnos("alumnos.dat", ios::in | ios::binary);

	if (!alumnos) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	alumnos.seekg(0, ios::beg);

	alumno aux;
	alumnos.read(reinterpret_cast<char*>(&aux), sizeof(alumno));

	while (!alumnos.eof()) {
		if (aux.cuenta == _cuentaAlum) {
			codigo = aux.codigoPlan;
			alumnos.close();
		}

		alumnos.read(reinterpret_cast<char*>(&aux), sizeof(alumno));
	}

	alumnos.close();

	notasFile.seekg(0, ios::beg);
	nota actual;
	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		if (actual.cuentaAlumno == _cuentaAlum) {
			//Buscar las UV de la clase
			int UV = 0;
			if (codigo == nullptr) {
				cout << "FATAL ERROR!!\n\n";
				return;
			}

			string name = (string)codigo + "_plan.dat";
			ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

			if (!PlanFile) {
				cout << "No se pudo encontrar la clase!!\n\n";
				return;
			}

			//BUSCAR MATERIA EN EL ARCHIVO
			MateriaArchivo actualMateria;
			PlanArchivo planArchivo;

			PlanFile.read(reinterpret_cast<char*>(&planArchivo), sizeof(PlanArchivo));
			PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));

			while (!PlanFile.eof()) {
				if (strcmp(actualMateria.codigo, actual.codigoClase) == 0) {
					UV = actualMateria.uv;
				}

				for (int i = 0; i < actualMateria.cantidadPadres; i++) {
					HijoFile hijoFile;
					PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));
				}

				PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
			}

			PlanFile.close();
			totalNotas += (actual.notaClase * UV);
			totalUV += UV;
		}

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	notasFile.close();

	float promedio = 0;
	if (totalNotas != 0 && totalUV != 0) {
		promedio = totalNotas / totalUV;
		cout << "\n\nPROMEDIO ES IGUAL A: " << totalNotas << " / " << totalUV << " ---> " << promedio;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	int posicionLectura = alumnosFile.tellg();
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _cuentaAlum) {
			actualAlumno.indiceAcademico = promedio;
			alumnosFile.seekg(posicionLectura);
			alumnosFile.write(reinterpret_cast<const char*>(&actualAlumno), sizeof(alumno));
			alumnosFile.close();
			return;
		}

		posicionLectura = alumnosFile.tellg();
		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
}


void EntidadEducativa::matricula() {
	cout << "\nIndique el Numero de Cuenta del Alumno:";
	int ncuenta;
	cin >> ncuenta;

	//BUSCAR EL CODIGO DE MATERIA EN EL ARCHIVO DE ALUMNOS
	char* codigo = nullptr;
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == ncuenta) {
			codigo = actualAlumno.codigoPlan;
			alumnosFile.close();
		}

		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();

	if (codigo == nullptr) {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
		return;
	}

	//VERIFICAR SI DICHO PLAN EXISTE
	PlanEstudio aux;
	char nombre[20];
	string name = (string)codigo + "_plan.dat";

	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "\nERROR PLAN NO EXISTE!!!!\n";
		return;
	}
	else {
		PlanArchivo actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));
		strcpy_s(nombre, strlen(actualPlan.nombrePlan) + 1, actualPlan.nombrePlan);
	}


	PlanEstudio PlanClases(codigo, nombre);
	Matricula Matricula(codigo);

	Matricula.imprimirDisponibles();

	int cantidadClases = -1;
	while (cantidadClases > 3 || cantidadClases < 0) {
		cout << "\nIndique la cantidad de clases a matricular Maximo 3: ";
		cin >> cantidadClases;
	}

	if (cantidadClases == 0) {
		cout << "\nSE CANCELO EL PROCESO DE MATRICULA!!!!\n";
		return;
	}

	if (cantidadClases <= 3) {
		for (int t = 0; t < cantidadClases; t++) {
			cout << "Indique el Codigo de la Materia: ";
			char codigoclass[8];
			cin >> codigoclass;

			//BUSCAR MATERIA EN EL ARCHIVO
			bool Existeclase = false;
			MateriaArchivo actualMateria;
			PlanFile.seekg(0, ios::beg);
			PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));

			materia** padres = nullptr;
			materiaMatri** padresMatri = nullptr;

			while (!PlanFile.eof()) {
				padres = new materia * [actualMateria.cantidadPadres];
				padresMatri = new materiaMatri * [actualMateria.cantidadPadres];

				if (strcmp(actualMateria.codigo, codigoclass) == 0) {
					Existeclase = true;
				}

				for (int i = 0; i < actualMateria.cantidadPadres; i++) {
					HijoFile hijoFile;
					PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));

					materia* padre = PlanClases.buscarMateria(hijoFile.codigo);
					materiaMatri* padreMatri = Matricula.buscarMateria(hijoFile.codigo);
					padres[i] = padre;
					padresMatri[i] = padreMatri;
				}

				PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
			}

			cout << "Indique la Nota Asignada:";
			float nota;
			cin >> nota;

			cout << "Indique el Año:";
			int anio;
			cin >> anio;

			cout << "Indique el periodo:";
			int periodo;
			cin >> periodo;

			Matricula.agregarMateria(codigo, codigoclass, nota, anio, periodo, padresMatri, actualMateria.cantidadPadres);

			EntidadEducativa::agregarNotas(ncuenta, codigoclass, nota, anio, periodo);
		}
	}
	PlanFile.close();
}


bool EntidadEducativa::aprobada(int _cuentaAlumno,const char* _codigoMateria) {
	ifstream notasFile("notas.dat", ios::in | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return false;
	}

	notasFile.seekg(0, ios::beg);

	nota actual;
	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		alumno* alumno = buscarAlumno(actual.cuentaAlumno);

		if (actual.notaClase >= 60 && actual.cuentaAlumno == _cuentaAlumno)
			return true;

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	return false;
	notasFile.close();
}


void EntidadEducativa::actualizarAprobadas(int _cuentaAlum) {
	ifstream notasFile("notas.dat", ios::in | ios::binary);
	fstream alumnosFile("alumnos.dat", ios::out | ios::in | ios::binary);

	if (!notasFile || !alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return;
	}

	float totalAprobadas = 0;


	//BUSCAR EL CODIGO DE MATERIA EN EL ARCHIVO DE ALUMNOS
	char* codigo = nullptr;
	ifstream alumnos("alumnos.dat", ios::in | ios::binary);

	if (!alumnos) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	alumnos.seekg(0, ios::beg);

	alumno aux;
	alumnos.read(reinterpret_cast<char*>(&aux), sizeof(alumno));

	while (!alumnos.eof()) {
		if (aux.cuenta == _cuentaAlum) {
			codigo = aux.codigoPlan;
			alumnos.close();
		}

		alumnos.read(reinterpret_cast<char*>(&aux), sizeof(alumno));
	}

	alumnos.close();

	notasFile.seekg(0, ios::beg);
	nota actual;
	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		if (actual.cuentaAlumno == _cuentaAlum) {
			if (actual.notaClase >= 60) {
				totalAprobadas++;
			}
		}

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	notasFile.close();

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	int posicionLectura = alumnosFile.tellg();
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _cuentaAlum) {
			actualAlumno.clasesAprobadas = totalAprobadas;
			alumnosFile.seekg(posicionLectura);
			alumnosFile.write(reinterpret_cast<const char*>(&actualAlumno), sizeof(alumno));
			alumnosFile.close();
			return;
		}

		posicionLectura = alumnosFile.tellg();
		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
}

void EntidadEducativa::consultarPromedio(int _codigoAlum) {
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actual;
	alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actual.cuenta == _codigoAlum) {
			cout << "EL PROMEDIO PARA EL ALUMNO: {Cuenta: " << actual.cuenta << ",\tNombre: " << actual.nombre << ", ES DE: " << actual.indiceAcademico << " }\n";
		}

		alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));
	}

	alumnosFile.close();
}

void EntidadEducativa::consultarHistorial(int _codigoAlum) {
	//DATOS DEL ALUMNO
	ifstream alumnosFile("alumnos.dat", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	bool seEncontro = false;

	alumnosFile.seekg(0, ios::beg);

	alumno actual;
	alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actual.cuenta == _codigoAlum) {
			cout << "\nDATOS DEL ALUMNO: \n";
			cout << "Alumno: {Cuenta: " << actual.cuenta << ",\tNombre: " << actual.nombre << ", Promedio: " << actual.indiceAcademico << ", Clases Aprobadas: " << actual.clasesAprobadas << ", Plan de Clases: " << actual.codigoPlan << " }\n";
			seEncontro = true;
		}

		alumnosFile.read(reinterpret_cast<char*>(&actual), sizeof(alumno));
	}

	alumnosFile.close();

	if (!seEncontro) {
		cout << "\n\nEL ALUMNO NO TIENE UN HISTORIAL AUN!!!\n";
		return;
	}

	//DATOS DE CLASES
	ifstream notasFile("notas.dat", ios::in | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	cout << " ***** H I S T O R I A L  A C A D E M I C O ***** \n\n";

	notasFile.seekg(0, ios::beg);

	nota actualNota;
	notasFile.read(reinterpret_cast<char*>(&actualNota), sizeof(nota));

	while (!notasFile.eof()) {
		if (actualNota.cuentaAlumno == _codigoAlum) {
			cout << "Nota { Alumno: " << actualNota.cuentaAlumno << " " << ",\tClase: "
				<< actualNota.codigoClase << ", Nota: " << actualNota.notaClase << " }\n";
		}

		notasFile.read(reinterpret_cast<char*>(&actualNota), sizeof(nota));
	}

	notasFile.close();
}