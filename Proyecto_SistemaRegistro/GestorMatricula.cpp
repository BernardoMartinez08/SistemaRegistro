#include "GestorMatricula.h"
#include <fstream>
#include <iostream>
#include "PlanEstudio.h"
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

		}
		else {
			cout << "\nERROR NO EXISTE ALUMNO!!!!\n";
		}

		actualizarAprobadas(_cuentaAlu);
		actualizarPromedio(_cuentaAlu);
		cout << "\nNOTA AGREGADA CON EXITO!!!!\n";
	}
	else {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
	}

	notasFile.close();
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
	int _cuentaAlu = 0;
	char _codigoPlan[4];
	char _clase1[6] = " ";
	char _clase2[6] = " ";
	char _clase3[6] = " ";

	int numclases = 0;

	cout << " ***** M A T R I C U L A ***** \n\n";
	
	while (numclases == 0 || numclases > 3) {
		cout << "Cuantas clases desea matricular: ";
		cin >> numclases;

		if (numclases > 3)
			cout << "\nEl maximo de clases para matricular es disponible es: " << 3;
	}

	alumno* estudiante = nullptr;
	
	while (_cuentaAlu == 0) {
		cout << "\nIngrese su numero de cuenta: ";
		cin >> _cuentaAlu;

		if (existeAlumno(_cuentaAlu) == false)
			_cuentaAlu = 0;
		else
			estudiante = buscarAlumno(_cuentaAlu);

	}

	char* codigoPlan{ estudiante->codigoPlan };
	char* nombrePlan{ plan->nombrePlanFile(codigoPlan) };

	PlanEstudio planClases(codigoPlan, nombrePlan);
	int clasesListas = 0;
	while (clasesListas > 0) {
		if (estudiante != nullptr) {
			if (numclases == 1) {
				cout << "\nIngrese codigo de clase: ";
				cin >> _clase1;

				if (planClases.buscarMateria(_clase1) == nullptr) {
					numclases--;

					if (numclases == 2) {
						cout << "\nIngrese codigo de clase: ";
						cin >> _clase2;

						if (planClases.buscarMateria(_clase2) == nullptr) {
							numclases--;

							if (numclases == 3) {
								cout << "\nIngrese codigo de clase: ";
								cin >> _clase3;

								if (planClases.buscarMateria(_clase3) == nullptr)
									numclases--;
							}
						}
					}
				}
			}
		}
		else
			break;
	}

	
	
	
	ofstream matriculaFile("matricula.dat", ios::out | ios::app | ios::binary);

	if (!matriculaFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	matriculas nuevaMatricula;
	
	if (existeAlumno(_cuentaAlu) == true) {
		strcpy_s(nuevaMatricula.codigoPlan, strlen(_codigoPlan) + 1, _codigoPlan);
		nuevaMatricula.cuentaAlumno = _cuentaAlu;
		strcpy_s(nuevaMatricula.codigoClase1, strlen(_clase1) + 1, _clase1);
		strcpy_s(nuevaMatricula.codigoClase2, strlen(_clase2) + 1, _clase2);
		strcpy_s(nuevaMatricula.codigoClase3, strlen(_clase3) + 1, _clase3);


		matriculaFile.write(reinterpret_cast<const char*>(&nuevaMatricula), sizeof(matriculas));
	}

	matriculaFile.close();
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