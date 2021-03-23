#include "Matricula.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "PlanEstudio.h"
using namespace std;

Matricula::Matricula(const char* _codigo) : raices(nullptr) {
	cantidadRaices = 0;

	codigoPlan = new char[strlen(_codigo)];
	strcpy_s(codigoPlan, strlen(_codigo) + 1, _codigo);

	string name = (string)_codigo + "_matri.dat";

	if (existePlan(_codigo) == false) {
		ofstream PlanFile(name, ios::out | ios::app | ios::binary);

		if (!PlanFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
			return;
		}

		Plan nuevoArchivo;
		strcpy_s(nuevoArchivo.codigoPlan, strlen(_codigo) + 1, _codigo);

		PlanFile.write(reinterpret_cast<const char*>(&nuevoArchivo), sizeof(Plan));

		PlanFile.close();

		cout << "\nPLAN AGREGADO CON EXITO!!!!!\n";
	}
	else {
		ifstream PlanFile(name, ios::in | ios::binary);

		if (!PlanFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
		}

		Plan actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(Plan));

		codigoPlan = new char[strlen(_codigo)];
		strcpy_s(codigoPlan, strlen(_codigo) + 1, _codigo);

		MateriaArchivo2 actualMateria;

		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo2));

		while (!PlanFile.eof()) {
			materiaMatri** padres;

			if (actualMateria.cantidadPadres > 0) {
				padres = new materiaMatri * [actualMateria.cantidadPadres];

				for (int i = 0; i < actualMateria.cantidadPadres; i++) {
					HijoFile2 hijoFile;
					PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile2));

					materiaMatri* padre = buscarMateria(hijoFile.codigo);
					padres[i] = padre;
				}
			}
			else
				padres = nullptr;

			agregarMateriaFromFile(codigoPlan, actualMateria.codigo, actualMateria.nota, actualMateria.anio, actualMateria.periodo, padres, actualMateria.cantidadPadres);

			PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo2));
		}

		PlanFile.close();

		cout << "\nPLAN CARGADO CON EXITO!!!!!\n";
	}
}

Matricula::Matricula() : raices(nullptr) {
	codigoPlan = 0;
}

bool Matricula::existePlan(const char* _codigoPlan) {
	string name = (string)_codigoPlan + "_matri.dat";

	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return false;
	}
	else {
		return true;
	}

	PlanFile.close();
}

bool Matricula::estaVacio() {
	return raices == nullptr;
}

void Matricula::agregarMateria(const char* _codigoPlan, const char* _codigoMateria, int _nota, int _anio, int _periodo, materiaMatri** _padres, int _cantidadPadres) {
	if (buscarMateria(_codigoMateria) == nullptr) {
		materiaMatri* nueva = new materiaMatri(_codigoMateria, _nota, _anio, _periodo, _padres, _cantidadPadres);

		if (estaVacio()) {
			raices = new materiaMatri * [1];
			raices[0] = nueva;
			cantidadRaices = cantidadRaices + 1;
			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!\n";
		}
		else if (_padres == nullptr && !estaVacio()) {
			materiaMatri** tmp = new materiaMatri * [cantidadRaices + 1];

			for (int i = 0; i < cantidadRaices; i++) {
				tmp[i] = raices[i];
			}

			tmp[cantidadRaices] = nueva;

			if (raices != nullptr) {
				delete raices;
			}

			//raices = new materia * [cantidadRaices + 1];
			raices = tmp;
			cantidadRaices = cantidadRaices + 1;

			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!\n";
		}
		else {
			for (int k = 0; k < _cantidadPadres; k++) {
				materiaMatri* padre = _padres[k];

				if (padre == nullptr) {
					cout << "\nCLASE REQUISITO NO EXISTE!!!\n";
					delete nueva;
					return;
				}
				else {
					materiaMatri** tmp = new materiaMatri * [padre->cantidadHijos + 1];

					for (int i = 0; i < padre->cantidadHijos; i++) {
						tmp[i] = padre->hijos[i];
					}

					tmp[padre->cantidadHijos] = nueva;

					if (padre->hijos != nullptr) {
						delete padre->hijos;
					}

					padre->hijos = tmp;
					padre->cantidadHijos++;

					cout << "\nCLASE AGREGADA EXITOSAMENTE!!!\n";
				}
			}
		}


		//GUARDAR EN ARCHIVO LA NUEVA MATERIA
		/*string name = (string)_codigoPlan + "_matri.dat";

		ofstream materiaFile(name, ios::out | ios::app | ios::binary);

		if (!materiaFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
			return;
		}

		MateriaArchivo nuevaArchivo;
		strcpy_s(nuevaArchivo.codigo, strlen(_codigoMateria) + 1, _codigoMateria);
		nuevaArchivo.nota = _nota;
		nuevaArchivo.periodo = _periodo;
		nuevaArchivo.anio = _anio;
		nuevaArchivo.cantidadHijos = nueva->cantidadHijos;
		nuevaArchivo.cantidadPadres = nueva->cantidadPadres;

		materiaFile.write(reinterpret_cast<const char*>(&nuevaArchivo), sizeof(MateriaArchivo));

		HijoFile hijoFile;
		for (int i = 0; i < nueva->cantidadPadres; i++) {
			strcpy_s(hijoFile.codigo, strlen(nueva->padres[i]->codigo) + 1, nueva->padres[i]->codigo);
			materiaFile.write(reinterpret_cast<const char*>(&hijoFile), sizeof(HijoFile));
		}

		materiaFile.close();*/

		reescribirMaterias();
	}
	else
		cout << "\nMATERIA YA EXISTE!!!\n";
}

void Matricula::agregarMateriaFromFile(const char* _codigoPlan, const char* _codigoMateria, int _nota, int _anio, int _periodo, materiaMatri** _padres, int _cantidadPadres) {
	if (buscarMateria(_codigoMateria) == nullptr) {
		materiaMatri* nueva = new materiaMatri(_codigoMateria, _nota, _anio, _periodo, _padres, _cantidadPadres);

		if (estaVacio()) {
			raices = new materiaMatri * [1];
			raices[0] = nueva;
			//cantidadRaices = cantidadRaices + 1;
			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!TO ROOTS\n";
		}
		else if (_padres == nullptr && !estaVacio()) {
			materiaMatri** tmp = new materiaMatri * [cantidadRaices+1];

			for (int i = 0; i < cantidadRaices; i++) {
				tmp[i] = raices[i];
			}

			tmp[cantidadRaices] = nueva;

			if (raices != nullptr) {
				delete raices;
			}

			raices = new materiaMatri * [cantidadRaices + 1];
			raices = tmp;
			cantidadRaices = cantidadRaices + 1;

			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!TO ROOTS\n";
		}
		else {
			for (int k = 0; k < _cantidadPadres; k++) {
				materiaMatri* padre = _padres[k];

				if (padre == nullptr) {
					cout << "\nCLASE REQUISITO NO EXISTE!!!\n";
					delete nueva;
					return;
				}
				else {
					materiaMatri** tmp = new materiaMatri * [padre->cantidadHijos + 1];

					for (int i = 0; i < padre->cantidadHijos; i++) {
						tmp[i] = padre->hijos[i];
					}

					tmp[padre->cantidadHijos] = nueva;

					if (padre->hijos != nullptr) {
						delete padre->hijos;
					}

					padre->hijos = tmp;
					padre->cantidadHijos++;

					cout << "\nCLASE AGREGADA EXITOSAMENTE!!!\n";
				}
			}
		}
	}
	else
		cout << "\nMATERIA YA EXISTE!!!\n";
}

materiaMatri* Matricula::buscarMateria(const char* _codigo) {
	return buscarMateriaRec(raices, cantidadRaices, _codigo);
}

materiaMatri* Matricula::buscarMateriaRec(materiaMatri** _raices, int _tamanio, const char* _codigo) {
	if (_raices == nullptr) {
		return nullptr;
	}

	for (int k = 0; k < cantidadRaices; k++) {
		if (strcmp(_raices[k]->codigo, _codigo) == 0) {
			return _raices[k];
		}

		for (int i = 0; i < _raices[k]->cantidadHijos; i++) {
			materiaMatri* tmp = buscarMateriaRec(_raices[k]->hijos, _raices[k]->cantidadHijos, _codigo);

			if (tmp != nullptr)
				return tmp;
		}

	}

	return nullptr;
}

void Matricula::imprimir() {
	cout << "\n";
	imprimirRec(raices, nullptr);
}

void Matricula::imprimirRec(materiaMatri** _raices, materiaMatri* _raiz) {
	if (_raices == nullptr)
		return;

	cout << "\n* * *  D A T O S  * * *\n";
	for (int j = 0; j < cantidadRaices; j++) {
		_raices[j]->getDatos();

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			imprimirRec(_raices[j]->hijos, _raices[j]->hijos[i]);
		}
	}
}

materiaMatri** Matricula::buscarPadres(materiaMatri** _raices, int tamanio, const char* codigoMateria) {

	if (tamanio > 0) {
		for (int j = 0; j < tamanio; j++) {
			for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
				if (strcmp(codigoMateria, _raices[j]->hijos[i]->codigo)) {
					return _raices;
				}
				else {
					materiaMatri** tmp = buscarPadres(_raices[j]->hijos, _raices[j]->cantidadHijos, codigoMateria);

					if (tmp != nullptr)
						return tmp;
				}
			}
		}
	}
	return nullptr;
}

materiaMatri** Matricula::buscarHijos(materiaMatri** _raices, materiaMatri* _padre, int tamanio) {
	if (tamanio > 0) {
		for (int j = 0; j < tamanio; j++) {
			for (int i = 0; i < _raices[j]->cantidadPadres; i++) {
				if (_padre == _raices[j]->padres[i]) {
					return _raices[j]->hijos;
				}
				else {
					materiaMatri** tmp = buscarHijos(_raices[j]->hijos, _padre, _raices[j]->cantidadHijos);

					if (tmp != nullptr)
						return tmp;
				}
			}
		}
	}
	return nullptr;
}

char* Matricula::getCodigo() {
	return this->codigoPlan;
}

int Matricula::buscarMateriaArchivo(const char* _codigoPlan, const char* _codigoMateria) {
	string name = (string)codigoPlan + "_matri.dat";
	ifstream PlanFile(name, ios::in | ios::binary);

	if (!PlanFile) {
		return -1;
	}

	int posicionLectura = PlanFile.tellg();
	PlanFile.seekg(0, ios::beg);

	Matricula actualPlan;
	PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(Plan));

	posicionLectura = PlanFile.tellg();
	materiaMatri* actualMateria = new materiaMatri(" ", 0, 0, 0, nullptr, 0);

	PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo2));

	while (!PlanFile.eof()) {
		if (actualMateria->codigo == _codigoMateria) {
			PlanFile.close();
			return posicionLectura;
		}

		for (int i = 0; i < actualMateria->cantidadPadres; i++) {
			HijoFile2 hijoFile;
			PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile2));
		}

		posicionLectura = PlanFile.tellg();
		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo2));
	}

	PlanFile.close();
	return -1;
}

void Matricula::imprimirDisponibles() {
	cout << "\n";
	imprimirDisponiblesRec(raices, nullptr);
}

void Matricula::imprimirDisponiblesRec(materiaMatri** _raices, materiaMatri* _raiz) {
	if (_raices == nullptr) {
		char* nombre;

		PlanEstudio aux;
		string name = (string)codigoPlan + "_plan.dat";

		ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

		if (!PlanFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
			return;
		}
		else {
			PlanArchivo actualPlan;
			PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));
			nombre = actualPlan.nombrePlan;
		}

		PlanFile.close();

		if (nombre != nullptr) {
			PlanEstudio* PlanClases = new PlanEstudio(codigoPlan, nombre);
			PlanClases->imprimirRaices();
		}
	
		else
			cout << "\nNO EXISTE ESTE PLAN!!!";
		return;
	}

	cout << "\n* * *  C L A S E S  D I S P O N I B L E S  * * *\n";
	for (int j = 0; j < cantidadRaices; j++) {
		if (_raices[j]->nota >= 60) {
			_raices[j]->getHijos();
		}else
			_raices[j]->getDatos();

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			imprimirRec(_raices[j]->hijos, _raices[j]->hijos[i]);
		}
	}
}

void Matricula::reescribirMaterias() {
	reescribirMateriasRec(raices, nullptr, 8);
}

void Matricula::reescribirMateriasRec(materiaMatri** _raices, materiaMatri* _raiz, int posicion) {
	if (_raices == nullptr)
		return;

	for (int j = 0; j < cantidadRaices; j++) {
		//GUARDAR EN ARCHIVO LA NUEVA MATERIA
		string name = (string)codigoPlan + "_matri.dat";

		ofstream materiaFile(name, ios::out | ios::app | ios::binary);

		if (!materiaFile) {
			cout << "Error al intentar abrir el archivo .matri\n\n";
			return;
		}

		materiaFile.seekp(posicion);
		MateriaArchivo2 nuevaArchivo;
		strcpy_s(nuevaArchivo.codigo, strlen(_raices[j]->codigo) + 1, _raices[j]->codigo);
		nuevaArchivo.nota = _raices[j]->nota;
		nuevaArchivo.periodo = _raices[j]->periodo;
		nuevaArchivo.anio = _raices[j]->anio;
		nuevaArchivo.cantidadHijos = _raices[j]->cantidadHijos;
		nuevaArchivo.cantidadPadres = _raices[j]->cantidadPadres;

		materiaFile.write(reinterpret_cast<const char*>(&nuevaArchivo), sizeof(MateriaArchivo2));
		int posicionEscritura = materiaFile.tellp();

		HijoFile2 hijoFile;
		for (int i = 0; i < _raices[j]->cantidadPadres; i++) {
			cout << _raices[j]->padres[i]->codigo << "\n";
			strcpy_s(hijoFile.codigo, strlen(_raices[j]->padres[i]->codigo) + 1, _raices[j]->padres[i]->codigo);
			materiaFile.write(reinterpret_cast<const char*>(&hijoFile), sizeof(HijoFile2));
			posicionEscritura = materiaFile.tellp();
		}

		materiaFile.close();

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			reescribirMateriasRec(_raices[j]->hijos, _raices[j]->hijos[i], posicionEscritura);
		}

		/*for (int j = 0; j < cantidadRaices; j++) {
			for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
				reescribirMateriasRec(_raices[j]->hijos, _raices[j]->hijos[i], posicionEscritura);
			}
		}*/
	}
}
