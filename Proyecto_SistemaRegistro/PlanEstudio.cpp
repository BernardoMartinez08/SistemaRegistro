#include "PlanEstudio.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<PlanEstudio*> Planes;

PlanEstudio::PlanEstudio(const char* _codigo, const char* _nombre) : raices(nullptr) {
	cantidadRaices = 0;

	nombrePlan = new char[strlen(_nombre)];
	strcpy_s(nombrePlan, strlen(_nombre) + 1, _nombre);
	codigoPlan = new char[strlen(_codigo)];
	strcpy_s(codigoPlan, strlen(_codigo) + 1, _codigo);

	//GUARDAR EN ARCHIVO LA NUEVA MATERIA
	string name = (string)_codigo + "_plan.dat";

	if (existePlan(_codigo) == false) {
		ofstream PlanFile(name, ios::out | ios::app | ios::binary);

		if (!PlanFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
			return;
		}

		PlanArchivo nuevoArchivo;
		strcpy_s(nuevoArchivo.codigoPlan, strlen(_codigo) + 1, _codigo);
		strcpy_s(nuevoArchivo.nombrePlan, strlen(_nombre) + 1, _nombre);

		nuevoArchivo.totalUnidades = totalUnidades;
		nuevoArchivo.totalClases = totalClases;
		nuevoArchivo.totalBloques = totalBloques;
		nuevoArchivo.totalRaices = cantidadRaices;

		PlanFile.write(reinterpret_cast<const char*>(&nuevoArchivo), sizeof(PlanArchivo));

		PlanFile.close();

		cout << "\nPLAN: "  << nombrePlan << " AGREGADO CON EXITO!!!!!\n";
	}
	else {
		ifstream PlanFile(name, ios::in | ios::binary);

		if (!PlanFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
		}
		
		PlanArchivo actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));

		codigoPlan = new char[strlen(_codigo)];
		strcpy_s(codigoPlan, strlen(_codigo) + 1, _codigo);

		codigoPlan = new char[strlen(_nombre)];
		strcpy_s(nombrePlan, strlen(_nombre) + 1, _nombre);
		
		totalClases = actualPlan.totalClases;
		totalUnidades = actualPlan.totalUnidades;
		totalBloques = actualPlan.totalBloques;
		cantidadRaices = actualPlan.totalRaices-1;


		MateriaArchivo actualMateria;

		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
		
		while (!PlanFile.eof()) {
			materia** padres;

			if (actualMateria.cantidadPadres > 0) {
				padres = new materia * [actualMateria.cantidadPadres];

				for (int i = 0; i < actualMateria.cantidadPadres; i++) {
					//materia* padre = buscarMateria(actualMateria.padres[i]);
					HijoFile hijoFile;
					PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));

					materia* padre = buscarMateria(hijoFile.codigo);
					padres[i] = padre;
				}
			}
			else
				padres = nullptr;

			agregarMateriaFromFile(codigoPlan,nombrePlan,actualMateria.codigo, actualMateria.nombre, actualMateria.uv, actualMateria.anio, actualMateria.periodo,padres,actualMateria.cantidadPadres);
			
			PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
		}

		PlanFile.close();

		cout << "\nPLAN: "  << nombrePlan << " CARGADO CON EXITO!!!!!\n";
	}

	//Planes.push_back(this);
}

bool PlanEstudio::existePlan(const char* _codigoPlan) {
	string name = (string)_codigoPlan + "_plan.dat";

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

char* PlanEstudio::nombrePlanFile(const char* _codigoPlan) {
	string name = (string)_codigoPlan + "_plan.dat";

	ifstream PlanFile(name, ios::in | ios::binary | ios::_Nocreate);

	if (!PlanFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return nullptr;
	}
	else {
		PlanArchivo actualPlan;
		PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));
		return actualPlan.nombrePlan;
	}

	return nullptr;

	PlanFile.close();
}

PlanEstudio::PlanEstudio() :raices(nullptr) {
	cantidadRaices = 0;
	nombrePlan = "N/A";
	codigoPlan = 0;
}

bool PlanEstudio::estaVacio() {
	return raices == nullptr;
}

void PlanEstudio::agregarMateria(const char* _codigoPlan, const char* _nombrePlan, const char* _codigoMateria, const char* _nombreMateria, int _uv, int _anio, int _periodo, materia** _padres, int _cantidadPadres) {
	if (buscarMateria(_codigoMateria) == nullptr) {
		materia* nueva = new materia(_codigoMateria, _nombreMateria, _uv, _anio, _periodo, _padres, _cantidadPadres);

		if (estaVacio()) {
			raices = new materia * [1];
			raices[0] = nueva;
			cantidadRaices = cantidadRaices + 1;
			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!\n";
		}
		else if (_padres == nullptr && !estaVacio()) {
			materia** tmp = new materia * [cantidadRaices + 1];

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
				materia* padre = _padres[k];

				if (padre == nullptr) {
					cout << "\nCLASE REQUISITO NO EXISTE!!!\n";
					delete nueva;
					return;
				}
				else {
					materia** tmp = new materia * [padre->cantidadHijos + 1];

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

		

		////GUARDAR EN ARCHIVO LA NUEVA MATERIA
		//string name = (string)_codigoPlan + "_plan.dat";

		//ofstream materiaFile(name, ios::out | ios::app | ios::binary);

		//if (!materiaFile) {
		//	cout << "Error al intentar abrir el archivo .dat\n\n";
		//	return;
		//}

		//MateriaArchivo nuevaArchivo;
		//strcpy_s(nuevaArchivo.codigo, strlen(_codigoMateria) + 1, _codigoMateria);
		//strcpy_s(nuevaArchivo.nombre, strlen(_nombreMateria) + 1, _nombreMateria);
		//nuevaArchivo.uv = _uv;
		//nuevaArchivo.periodo = _periodo;
		//nuevaArchivo.anio = _anio;
		//nuevaArchivo.cantidadHijos = nueva->cantidadHijos;
		//nuevaArchivo.cantidadPadres = nueva->cantidadPadres;

		//materiaFile.write(reinterpret_cast<const char*>(&nuevaArchivo), sizeof(MateriaArchivo));

		//HijoFile hijoFile;
		//for (int i = 0; i < nueva->cantidadPadres; i++) {
		//	strcpy_s(hijoFile.codigo, strlen(nueva->padres[i]->codigo) + 1, nueva->padres[i]->codigo);
		//	materiaFile.write(reinterpret_cast<const char*>(&hijoFile), sizeof(HijoFile));
		//}

		//materiaFile.close();

		reescribirArchivoPlan();
	}
	else
		cout << "\nMATERIA YA EXISTE!!!\n";
}

void PlanEstudio::agregarMateriaFromFile(const char* _codigoPlan, const char* _nombrePlan, const char* _codigoMateria, const char* _nombreMateria, int _uv, int _anio, int _periodo, materia** _padres, int _cantidadPadres) {
	if (buscarMateria(_codigoMateria) == nullptr) {
		materia* nueva = new materia(_codigoMateria, _nombreMateria, _uv, _anio, _periodo, _padres, _cantidadPadres);
		if (estaVacio()) {
			raices = new materia * [1];
			raices[0] = nueva;
			cantidadRaices = cantidadRaices + 1;
			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!TO ROOTS\n";
		}
		else if (_padres == nullptr && !estaVacio()) {
			materia** tmp = new materia * [cantidadRaices];

			for (int i = 0; i < cantidadRaices; i++) {
				tmp[i] = raices[i];
			}

			tmp[cantidadRaices] = nueva;

			if (raices != nullptr) {
				delete raices;
			}

			raices = new materia * [cantidadRaices + 1];
			raices = tmp;
			cantidadRaices = cantidadRaices + 1;

			cout << "\nCLASE AGREGADA EXITOSAMENTE!!!TO ROOTS\n";
		}
		else {
			for (int k = 0; k < _cantidadPadres; k++) {
				materia* padre = _padres[k];

				if (padre == nullptr) {
					cout << "\nCLASE REQUISITO NO EXISTE!!!\n";
					delete nueva;
					return;
				}
				else {
					materia** tmp = new materia * [padre->cantidadHijos + 1];

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
	}else
		cout << "\nMATERIA YA EXISTE!!!\n";
}

materia* PlanEstudio::buscarMateria(const char* _codigo) {
	return buscarMateriaRec(raices, cantidadRaices, _codigo);
}

materia* PlanEstudio::buscarMateriaRec(materia** _raices, int _tamanio,  const char* _codigo) {
	if (_raices == nullptr) {
		return nullptr;
	}
	
	for (int k = 0; k < cantidadRaices; k++) {
		if (strcmp(_raices[k]->codigo, _codigo) == 0) {
			return _raices[k];
		}
			
		for (int i = 0; i < _raices[k]->cantidadHijos; i++) {
			materia* tmp = buscarMateriaRec(_raices[k]->hijos, _raices[k]->cantidadHijos, _codigo);

			if (tmp != nullptr)
				return tmp;
		}
		
	}

	return nullptr;
}

materia* PlanEstudio::buscarRaiz(const char* _codigo) {
	for (int i = 0; i < cantidadRaices; i++) {
		if (raices[i]->codigo == _codigo) {
			return raices[i];
		}
	}

	return nullptr;
}

void PlanEstudio::imprimir() {
	cout << "\n";
	imprimirRec(raices,nullptr);
}

void PlanEstudio::imprimirRec(materia** _raices, materia* _raiz) {
	if (_raices == nullptr)
		return;

	cout << "\n* * *  D A T O S  * * *\n";
	for (int j = 0; j < cantidadRaices; j++) {
		_raices[j]->getDatos();

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			imprimirRec(_raices[j]->hijos,_raices[j]->hijos[i]);
		}
	}
}

void PlanEstudio::imprimirRaices() {
	if (raices == nullptr)
		return;

	cout << "\n* * *  C L A S E S  D I S P O N I B L E S  * * *\n";
	for (int j = 0; j < cantidadRaices; j++) {
		raices[j]->getDatos();
	}
}

materia** PlanEstudio::buscarPadres(materia** _raices, int tamanio, const char* codigoMateria) {
	
	if (tamanio > 0) {
		for (int j = 0; j < tamanio; j++) {
			for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
				if (strcmp(codigoMateria, _raices[j]->hijos[i]->codigo)) {
					return _raices;
				}
				else {
					materia** tmp = buscarPadres(_raices[j]->hijos,_raices[j]->cantidadHijos, codigoMateria);

					if (tmp != nullptr)
						return tmp;
				}
			}
		}
	}
	return nullptr;
}

materia** PlanEstudio::buscarHijos(materia** _raices,materia* _padre, int tamanio) {
	if (tamanio > 0) {
		for (int j = 0; j < tamanio; j++) {
			for (int i = 0; i < _raices[j]->cantidadPadres; i++) {
				if (_padre == _raices[j]->padres[i]) {
					return _raices[j]->hijos;
				}
				else {
					materia** tmp = buscarHijos(_raices[j]->hijos, _padre,_raices[j]->cantidadHijos);

					if (tmp != nullptr)
						return tmp;
				}
			}
		}
	}
	return nullptr;
}

void PlanEstudio::eliminarMateria(const char* codigoMateria) {
	materia** padres = buscarPadres(raices, cantidadRaices, codigoMateria);
	materia* clase = buscarMateria(codigoMateria);

	bool esRaiz = false;

	if (clase == nullptr) {
		clase = buscarRaiz(codigoMateria);
		esRaiz = true;
	}

	if (clase != nullptr && esRaiz == false) {
		for (int y = 0; y < clase->cantidadPadres; y++) {
			materia* padre = padres[y];
			materia** hijos = buscarHijos(raices, padre, cantidadRaices);

			if (padre != nullptr)
			{
				for (int i = 0; i < padre->cantidadHijos; i++)
				{
					if (strcmp(padre->hijos[i]->codigo, codigoMateria))
					{
						if (padre->hijos[i]->cantidadHijos > 0)
						{
							//-1 el que vamos a eliminar y le agrego la cantidad de hijos de su hijo
							materia** nuevosHijos = new materia * [padre->cantidadHijos - 1 + padre->hijos[i]->cantidadHijos];
							int k = 0;
							//hemos copiado todos los hijos menos el que se va a eliminar
							for (int j = 0; j < padre->cantidadHijos; j++)
							{
								if (i != j)
								{
									nuevosHijos[k] = padre->hijos[j];
									k++;//solo en el caso se haga una copia 
								}
							}

							for (int l = 0; l < padre->hijos[i]->cantidadHijos; l++)
							{
								nuevosHijos[k] = padre->hijos[i]->hijos[l];
								k++;
							}

							delete padre->hijos;

							padre->hijos = nuevosHijos;
							padre->cantidadHijos = padre->cantidadHijos - 1 + padre->hijos[i]->cantidadHijos;
							return;
						}
						//cuando el nodo que se va eliminar no tiene hijos
						else
						{
							int k = 0;
							materia** nuevosHijos = new materia * [padre->cantidadHijos - 1];

							for (int j = 0; j < padre->cantidadHijos; j++)
							{
								if (i != j)
								{
									nuevosHijos[k] = padre->hijos[j];
									k++;

								}
							}

							padre->cantidadHijos = padre->cantidadHijos - 1;
							delete padre->hijos;
							padre->hijos = nuevosHijos;

							return;
						}

					}
				}




				//Elimanar de padres de cada hijo
				for (int i = 0; i < clase->cantidadHijos; i++)
				{
					if (clase->hijos[i]->cantidadPadres > 0)
					{
						//-1 el que vamos a eliminar y le agrego la cantidad de hijos de su hijo
						materia** nuevosPadres = new materia * [clase->hijos[i]->cantidadPadres - 1];
						int k = 0;
						//hemos copiado todos los hijos menos el que se va a eliminar
						for (int j = 0; j < clase->hijos[i]->cantidadPadres; j++)
						{
							if (clase->hijos[i]->padres[j] != clase)
							{
								nuevosPadres[k] = clase->hijos[i]->padres[j];
								k++;//solo en el caso se haga una copia 
							}
						}

						delete clase->hijos[i]->padres;

						clase->hijos[i]->padres = nuevosPadres;
						clase->hijos[i]->cantidadPadres = clase->hijos[i]->cantidadPadres - 1;
						return;
					}
					//cuando el nodo que se va eliminar no tiene hijos
					else
					{
						//Agergar clases independientes.
						int k = 0;
						materia** nuevosHijos = new materia * [padre->cantidadHijos - 1];

						for (int j = 0; j < padre->cantidadHijos; j++)
						{
							if (i != j)
							{
								nuevosHijos[k] = padre->hijos[j];
								k++;

							}
						}

						padre->cantidadHijos = padre->cantidadHijos - 1;
						delete padre->hijos;
						padre->hijos = nuevosHijos;

						return;
					}

				}

			}
		}
	}
}

void PlanEstudio::actualizarTotalClases(materia** _raices, materia* _raiz) {
	int total = 0;

	if (_raices == nullptr) {
		return;
	}
	
	for (int j = 0; j < cantidadRaices; j++) {

		total++;

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			actualizarTotalClases(_raices, _raices[j]->hijos[i]);
		}
	}
	

	totalClases = total;
}

void PlanEstudio::actualizarTotalUV(materia** _raices, materia* _raiz) {
	int total = 0;

	if (_raices == nullptr) {
		return;
	}
	
	for (int j = 0; j < cantidadRaices; j++) {

		total += _raices[j]->uv;

		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			actualizarTotalUV(_raices, _raices[j]->hijos[i]);
		}
	}
	

	totalUnidades = total;
}

void PlanEstudio::actualizarTotalBloques() {
	totalBloques = cantidadRaices;
}

int PlanEstudio::buscarMateriaArchivo(const char* _codigoPlan, const char* _codigoMateria) {
	string name = (string)codigoPlan + "_plan.dat";
	ifstream PlanFile(name, ios::in | ios::binary);

	if (!PlanFile) {
		return -1;
	}

	int posicionLectura = PlanFile.tellg();
	PlanFile.seekg(0, ios::beg);

	PlanEstudio actualPlan;
	PlanFile.read(reinterpret_cast<char*>(&actualPlan), sizeof(PlanArchivo));

	posicionLectura = PlanFile.tellg();
	materia* actualMateria = new materia(" "," ", 0, 0, 0, nullptr, 0);

	PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));

	while (!PlanFile.eof()) {
		if (actualMateria->codigo == _codigoMateria) {
			PlanFile.close();
			return posicionLectura;
		}

		for (int i = 0; i < actualMateria->cantidadPadres; i++) {
			HijoFile hijoFile;
			PlanFile.read(reinterpret_cast<char*>(&hijoFile), sizeof(HijoFile));
		}

		posicionLectura = PlanFile.tellg();
		PlanFile.read(reinterpret_cast<char*>(&actualMateria), sizeof(MateriaArchivo));
	}

	PlanFile.close();
	return -1;
}

char* PlanEstudio::getCodigo() {
	return this->codigoPlan;
}

char* PlanEstudio::getNombre() {
	return this->nombrePlan;
}

void PlanEstudio::reescribirArchivoPlan() {
	//actualizarTotalClases(raices, nullptr);
	//actualizarTotalUV(raices, nullptr);
	//actualizarTotalBloques();

	string name = (string)codigoPlan + "_plan.dat";

	ofstream PlanFile(name, ios::out | ios::binary);

	if (!PlanFile) {
		cout << "Error al intentar abrir el archivo .dat\n\n";
		return;
	}

	PlanArchivo nuevoArchivo;
	
	strcpy_s(nuevoArchivo.codigoPlan, strlen(codigoPlan) + 1, codigoPlan);
	cout << "\nNOMBRE REESCRITO: " << nombrePlan;
	strcpy_s(nuevoArchivo.nombrePlan, strlen(nombrePlan) + 1, nombrePlan);

	nuevoArchivo.totalUnidades = totalUnidades;
	nuevoArchivo.totalClases = totalClases;
	nuevoArchivo.totalBloques = totalBloques;
	nuevoArchivo.totalRaices = cantidadRaices;

	//PlanFile.seekp(0);
	PlanFile.write(reinterpret_cast<const char*>(&nuevoArchivo), sizeof(PlanArchivo));
	int posicionEscritura = PlanFile.tellp();
	PlanFile.close();

	reescribirMaterias(posicionEscritura);
}

void PlanEstudio::reescribirMaterias(int pocision) {
	reescribirMateriasRec(raices, nullptr, pocision);
}

void PlanEstudio::reescribirMateriasRec(materia** _raices, materia* _raiz, int posicion) {
	if (_raices == nullptr)
		return;

	for (int j = 0; j < cantidadRaices; j++) {
		//GUARDAR EN ARCHIVO LA NUEVA MATERIA
		string name = (string)codigoPlan + "_plan.dat";

		ofstream materiaFile(name, ios::out | ios::app | ios::binary);

		if (!materiaFile) {
			cout << "Error al intentar abrir el archivo .dat\n\n";
			return;
		}

		materiaFile.seekp(posicion);
		MateriaArchivo nuevaArchivo;
		strcpy_s(nuevaArchivo.codigo, strlen(_raices[j]->codigo) + 1, _raices[j]->codigo);
		strcpy_s(nuevaArchivo.nombre, strlen(_raices[j]->nombre) + 1, _raices[j]->nombre);
		nuevaArchivo.uv = _raices[j]->uv;
		nuevaArchivo.periodo = _raices[j]->periodo;
		nuevaArchivo.anio = _raices[j]->anio;
		nuevaArchivo.cantidadHijos = _raices[j]->cantidadHijos;
		nuevaArchivo.cantidadPadres = _raices[j]->cantidadPadres;

		materiaFile.write(reinterpret_cast<const char*>(&nuevaArchivo), sizeof(MateriaArchivo));
		int posicionEscritura = materiaFile.tellp();

		HijoFile hijoFile;
		for (int i = 0; i < _raices[j]->cantidadPadres; i++) {
			cout << _raices[j]->padres[i]->codigo << "\n";
			strcpy_s(hijoFile.codigo, strlen(_raices[j]->padres[i]->codigo) + 1, _raices[j]->padres[i]->codigo);
			materiaFile.write(reinterpret_cast<const char*>(&hijoFile), sizeof(HijoFile));
			posicionEscritura = materiaFile.tellp();
		}

		materiaFile.close();
	
		for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
			reescribirMateriasRec(_raices[j]->hijos, _raices[j]->hijos[i],posicionEscritura);
		}

		/*for (int j = 0; j < cantidadRaices; j++) {
			for (int i = 0; i < _raices[j]->cantidadHijos; i++) {
				reescribirMateriasRec(_raices[j]->hijos, _raices[j]->hijos[i], posicionEscritura);
			}
		}*/
	}
}
