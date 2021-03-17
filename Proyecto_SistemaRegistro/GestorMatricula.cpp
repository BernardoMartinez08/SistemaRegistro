#include "GestorMatricula.h"
#include <fstream>
#include <iostream>
using namespace std;
PlanEstudio* plan;

void EntidadEducativa::agregarAlumnos() {
	ofstream alumnoFile("alumnos.bin", ios::out | ios::app | ios::binary);

	if (!alumnoFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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
	ifstream alumnosFile("alumnos.bin", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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
	ifstream alumnosFile("alumnos.bin", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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
	ifstream alumnosFile("alumnos.bin", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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
	ifstream alumnosFile("alumnos.bin", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return nullptr;
	}

	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _Cuenta) {
			
			alumnosFile.close();
			cout << "Codigoooooooo: " << actualAlumno.codigoPlan;
			return actualAlumno.codigoPlan;
		}

		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
	}

	alumnosFile.close();
	return nullptr;
}




void EntidadEducativa::agregarNotas(int _cuentaAlu, const char* _codigoClas, float _nota, int _anio, int _periodo) {
	ofstream notasFile("notas.bin", ios::out | ios::app | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return;
	}

	nota nuevaNota;

	char* codigo = nullptr; 
	ifstream alumnosFile("alumnos.bin", ios::in | ios::binary);

	if (!alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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

	PlanEstudio aux;
	char nombre[20];
	string name = (string)codigo + "_plan.dat";

	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
		return;
	}
	else {
		PlanArchivo actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));
		strcpy_s(nombre,strlen(actualPlan.nombrePlan)+1, actualPlan.nombrePlan);
	}

	PlanFile.close();

	cout << "\nPLAN: " << nombre;

	PlanEstudio PlanClases(codigo, nombre);

	materia* materia = PlanClases.buscarMateria(_codigoClas);

	if (materia != nullptr) {
		if (existeAlumno(_cuentaAlu) == true && materia != nullptr) {
			strcpy_s(nuevaNota.codigoClase, strlen(_codigoClas) + 1, _codigoClas);
			nuevaNota.cuentaAlumno = _cuentaAlu;
			nuevaNota.notaClase = _nota;
			nuevaNota.anioMatricula = _anio;
			nuevaNota.periodoMatricula = _periodo;

			notasFile.write(reinterpret_cast<const char*>(&nuevaNota), sizeof(nota));
			actualizarPromedio(_cuentaAlu);
		}

		cout << "\nNOTA AGREGADA CON EXITO!!!!\n";
	}
	else {
		cout << "\nERROR MATERIA NO EXISTE!!!!\n";
	}

	notasFile.close();
}

void EntidadEducativa::consultarNotas() {
	ifstream notasFile("notas.bin", ios::in | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return;
	}

	cout << " ***** C O N S U L T A  D E  N O T A S ***** \n\n";

	notasFile.seekg(0, ios::beg);

	nota actual;
	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		alumno* alumno = buscarAlumno(actual.cuentaAlumno);

		cout << "Nota { Alumno: " << actual.cuentaAlumno << " " << alumno->nombre << ",\tClase: "
			<< actual.codigoClase << ", Nota: " << actual.notaClase << " }\n";

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	notasFile.close();
}

int EntidadEducativa::obtenerUV(const char* _codigoPlan, const char* _codigoMateria) {
	string name = (string)_codigoPlan + "_plan.dat";
	ifstream materiasFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!materiasFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return -1;
	}

	int posicion = plan->buscarMateriaArchivo(_codigoPlan, _codigoMateria);
	materiasFile.seekg(posicion);

	materia* actualMateria = new materia(" ", " ", 0, 0, 0, nullptr, 0);
	materiasFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(materia));

	return actualMateria->uv;
		
	materiasFile.close();
	return -1;
}

void EntidadEducativa::actualizarPromedio(int _cuentaAlum) {
	ifstream notasFile("notas.bin", ios::in | ios::binary);
	fstream alumnosFile("alumnos.bin", ios::out | ios::in | ios::binary);

	if (!notasFile || !alumnosFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
		return;
	}

	notasFile.seekg(0, ios::beg);

	nota actual;
	float totalNotas = 0;
	int totalUV = 0;

	notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));

	while (!notasFile.eof()) {
		if (actual.cuentaAlumno == _cuentaAlum) {
			int UV = obtenerUV(plan->getCodigo(),actual.codigoClase);

			totalNotas += (actual.notaClase * UV);
			totalUV += UV;
		}

		notasFile.read(reinterpret_cast<char*>(&actual), sizeof(nota));
	}

	notasFile.close();

	float promedio = totalNotas / totalUV;

	int posicionLectura = alumnosFile.tellg();
	alumnosFile.seekg(0, ios::beg);

	alumno actualAlumno;
	alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));

	while (!alumnosFile.eof()) {
		if (actualAlumno.cuenta == _cuentaAlum) {
			actualAlumno.indiceAcademico = promedio;
			alumnosFile.seekg(posicionLectura);
			alumnosFile.write(reinterpret_cast<const char*>(&actualAlumno), sizeof(alumno));
		}
		alumnosFile.read(reinterpret_cast<char*>(&actualAlumno), sizeof(alumno));
		posicionLectura = alumnosFile.tellg();
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

	
	
	
	ofstream matriculaFile("matricula.bin", ios::out | ios::app | ios::binary);

	if (!matriculaFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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
	ifstream notasFile("notas.bin", ios::in | ios::binary);

	if (!notasFile) {
		cout << "Error al intentar abrir el archivo .bin\n\n";
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