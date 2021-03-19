#include <iostream>
#include <conio.h>
#include "PlanEstudio.h"
#include "GestorMatricula.h"
#include <fstream>

int main() {
	/*PlanEstudio plan("8I-1","SistemasCompu");

	plan.agregarMateria("8I-1","SistemasCompu","MAT-110","IntroAlgebra",4,1,1,nullptr,0);
	plan.imprimir();

	materia** padres = new materia * [1];
	materia* padre = plan.buscarMateria("MAT-110");
	padres[0] = padre;

	cout << padre->nombre;

	plan.agregarMateria("8I-1", "SistemasCompu","MAT-111","Algebra", 4, 1, 2, padres, 1);

	plan.imprimir();*/

	cout << "* * * * *  B I E N V E N I D  O   A L  S I S T E M A  * * * * *\n\n";

	int opcionPrincipal = 0;
	while (opcionPrincipal != 4) {
		cout << "\n\nM E N U  P R I N C I P A L\n" <<
			"1. Opciones de Plan de Clases.\n2. Opciones de Matricula y Notas.\n3. Informacion Academica.\n4. Salir del Sistema.\n";
		cout << "\nSeleccione una de las opciones anteriores: ";
		cin >> opcionPrincipal;

		switch (opcionPrincipal)
		{
		case 1: {
			cout << "\n\n1. OPCIONES DEL PLAN DE CLASES.";

			int opcionesPlan = 0;
			while (opcionesPlan != 7) {
				cout << "\n\nM E N U  P L A N  D E  C L A S E S\n" <<
					"1. Agregar Nuevo Plan.\n2. Cargar un Plan.\n3. Agregar Materias a un Plan.\n4. Eliminar Materias de un Plan.\n5. Editar Materia de un Plan.\n6. Ver Datos de un Plan y Materias.\n7. Regresar al menu principal.\n";
				cout << "\nSeleccione una de las opciones anteriores: ";
				cin >> opcionesPlan;

				PlanEstudio* PlanClases;
				switch (opcionesPlan)
				{
				case 1: {
					cout << "\n\n1. AGREGAR NUEVO PLAN.\n";

					char codigo[6];
					char nombre[20];

					cout << "Digite el CODGIO del Plan (maximo 6 caracteres): ";
					cin >> codigo;

					cout << "\nEscriba el NOMBRE del Plan (maximo 20 caracteres): ";
					cin >> nombre;

					PlanClases = new PlanEstudio(codigo, nombre);

					break;
				}

				case 2: {
					cout << "\n\n2. CARGAR UN PLAN.\n";

					char codigo[6];
					char nombre[20];

					cout << "Digite el CODGIO del Plan (maximo 6 caracteres): ";
					cin >> codigo;

					PlanEstudio aux;
					strcpy_s(nombre, strlen(aux.nombrePlanFile(codigo)), aux.nombrePlanFile(codigo));

					if(nombre != nullptr)
						PlanClases = new PlanEstudio(codigo, nombre);
					else
						cout << "\nNO EXISTE ESTE PLAN!!!";

					break;
				}

				case 3: {
					cout << "\n\n3. AGREGAR NATERIA A UN PLAN.\n";

					PlanEstudio aux;
					if (PlanClases != nullptr) {

						char codigo[8];
						char nombre[25];
						int uv;
						int periodo;
						int anio;
						int cantidadPadres;
						materia** padres = nullptr;

						cout << "\nEscriba el Codgio de la Clase: ";
						cin >> codigo;

						cout << "\nEscriba el Nombre de la Clase: ";
						cin >> nombre;

						cout << "\nEscriba el Numero de UV's de la Clase: ";
						cin >> uv;

						cout << "\nEscriba el Periodo de la Clase: ";
						cin >> periodo;

						cout << "\nEscriba el Año de la Clase: ";
						cin >> anio;

						cout << "\nCuantos Padres Tendra: ";
						cin >> cantidadPadres;

						if (cantidadPadres > 0) {
							padres = new materia * [cantidadPadres];
							for (int i = 0; i < cantidadPadres; i++) {
								char codigoPadre[8];

								materia* padre = nullptr;

								while (padre == nullptr) {
									cout << "Digite el CODGIO de la Clase Pladre (maximo 8 caracteres): ";
									cin >> codigoPadre;

									padre = PlanClases->buscarMateria(codigoPadre);
									padres[i] = padre;
								}
							}
						}
						else {
							padres = nullptr;
						}

						if (padres == nullptr)
							cout << "\n\nPAPA NULO VOS\n";
						else
							cout << "\n\nSI TIENE PAPAS\n\n";

						PlanClases->agregarMateria(PlanClases->getCodigo(), PlanClases->getNombre(), codigo, nombre, uv, anio, periodo, padres, cantidadPadres);
					}
					else {
						cout << "\n\nAGREGUE UN NUEVO PLAN DE CLASES O ABRA UNO EXISTENTE ANTES DE USAR ESTA FUNCION!!!!!!\n\n";
					}

					break;
				}

				case 4: {
					cout << "\n\n5. Editar Materia de un Plan.\n";

					break;
				}

				case 5: {
					cout << "\n\n5. EDITAR UNA MATERIA DEL PLAN.\n";

					PlanEstudio aux;
					if (PlanClases != nullptr) {
						char codigo[8];

						cout << "Digite el CODGIO de la Clase (maximo 8 caracteres): ";
						cin >> codigo;

						materia* Materia = PlanClases->buscarMateria(codigo);

						if (Materia != nullptr) {
							int opcionesCambio = 0;

							while (opcionesCambio != 6) {
								cout << "\n\nM E N U  M O D I F I C A R  C L A S E \n" <<
									"1. Nuevo Codigo.\n2. Nuevo Nombre.\n3. Actualizar UV's.\n4. Actualizar Año.\n5. Actualizar Periodo.\n6. Volver al Menu de Plan de Clases.\n";
								cout << "\nSeleccione una de las opciones anteriores: ";
								cin >> opcionesCambio;

								switch (opcionesCambio)
								{
								case 1: {
									cout << "\n\n1. NUEVO CODIGO.\n";

									cout << "\nEscriba el Codgio de la Clase: ";
									cin >> codigo;

									Materia->setCodigo(codigo);

									PlanClases->reescribirArchivoPlan();
									break;
								}

								case 2: {
									cout << "\n\n2. NUEVO NOMBRE.\n";
									char nombre[25];

									cout << "\nEscriba el Nombre de la Clase: ";
									cin >> nombre;

									Materia->setNombre(nombre);

									PlanClases->reescribirArchivoPlan();
									break;
								}

								case 3: {
									cout << "\n\n3. ACTUALIZAR UV's.\n";
									int uv;

									cout << "\nEscriba las UV's de la Clase: ";
									cin >> uv;

									Materia->setUV(uv);

									PlanClases->reescribirArchivoPlan();
									break;
								}

								case 4: {
									cout << "\n\n3. ACTUALIZAR AÑO.\n";
									int anio;

									cout << "\nEscriba el año de la Clase: ";
									cin >> anio;

									Materia->setAnio(anio);

									PlanClases->reescribirArchivoPlan();
									break;
								}

								case 5: {
									cout << "\n\n3. ACTUALIZAR PERIODO.\n";
									int periodo;

									cout << "\nEscriba el periodo de la Clase: ";
									cin >> periodo;

									Materia->setAnio(periodo);

									PlanClases->reescribirArchivoPlan();
									break;
								}

								case 6: {
									cout << "\n\nS A L I E N D O ----> regresando a menu del plan.\n";
									break;
								}

								default: {
									cout << "\n\nINTENTA DENUEVO, OPCION NO VALIDA.......\n";
									break;
								}
								}
							}
						}
						else {
							cout << "\n\nINTENTE DENUEVO MATERIA NO VALIDA!!!!!!\n\n";
						}
					}
					else {
						cout << "\n\nAGREGUE UN NUEVO PLAN DE CLASES O ABRA UNO EXISTENTE ANTES DE USAR ESTA FUNCION!!!!!!\n\n";
					}

					break;
				}

				case 6: {
					cout << "\n\n6. VER DATOS DEL PLAN Y MATERIAS.\n";

					cout << PlanClases->getCodigo();
					PlanClases->imprimir();
					break;
				}

				case 7: {
					cout << "\n\nS A L I E N D O ----> regresando a menu principal.\n";
					break;
				}

				default: {
					cout << "\n\nINTENTA DENUEVO, OPCION NO VALIDA.......\n";
					break;
				}
				}
			}

			break;
		}

		case 2: {
			cout << "\n2. Opciones de Matricula y Notas.\n";

			int opcionesMatricula = 0;
			while (opcionesMatricula != 6) {
				cout << "\n\nM E N U  M A T R I C U L A\n" <<
					"1. Agregar Alumnos.\n2. Consultar Alumnos.\n3. Matricular Asignaturas.\n4. Actualizar Notas.\n5. Ver Notas.\n6. Regresar al menu principal.\n";
				cout << "\nSeleccione una de las opciones anteriores: ";
				cin >> opcionesMatricula;

				switch (opcionesMatricula)
				{
				case 1: {
					cout << "\n\n1. AGREGAR ALUMNOS.\n";
					EntidadEducativa::agregarAlumnos();
					
					break;
				}

				case 2: {
					cout << "\n\n2. CONSULTAR ALUMNOS.\n";

					EntidadEducativa::consultarAlumnos();

					break;
				}

				case 3: {
					cout << "\n\n3. MATRICULAR ASIGNATURA.\n";

					EntidadEducativa::matricula();

					break;
				}

				case 4: {
					cout << "\n\n4. ACTUALIZAR NOTAS.\n";

					cout << "\nIndique el Numero de Cuenta del Alumno:";
					int ncuenta;
					cin >> ncuenta;

					cout << "Indique el Codigo de la Materia: ";
					char codigoclass[8];
					cin >> codigoclass;

					cout << "Indique la Nota Asignada:";
					float nota;
					cin >> nota;

					cout << "Indique el Año:";
					int anio;
					cin >> anio;

					cout << "Indique el periodo:";
					int periodo;
					cin >> periodo;
					
					EntidadEducativa::agregarNotas(ncuenta, codigoclass, nota, anio, periodo);
					break;
				}

				case 5: {
					cout << "\n\n5. VER NOTAS.\n";

					EntidadEducativa::consultarNotas();

					break;
				}

				case 6: {
					cout << "\n\nS A L I E N D O ----> regresando a menu principal.\n";
					break;
				}

				default: {
					cout << "\n\nINTENTA DENUEVO, OPCION NO VALIDA.......\n";
					break;
				}

				}


			}
			break;
		}

		case 3: {
			cout << "\n3. INFORMACION ACADEMICA.\n";

			int opcionesAcademica = 0;
			while (opcionesAcademica != 3) {
				cout << "\n\nM E N U  I N FO  A C A D E M I C A\n" <<
					"1. Consultar el Promedio de un Alumno.\n2. Consultar el Historial academico de un Alumno.\n3. Regresar al menu principal.\n";
				cout << "\nSeleccione una de las opciones anteriores: ";
				cin >> opcionesAcademica;

				switch (opcionesAcademica)
				{
				case 1: {
					cout << "\n\n1. CONSULTAR EL PROMEDIO DE UN ALUMNO.\n";

					cout << "\nIndique el Numero de Cuenta del Alumno:";
					int ncuenta;
					cin >> ncuenta;

					EntidadEducativa::consultarPromedio(ncuenta);

					break;
				}

				case 2: {
					cout << "\n\n2. CONSULTAR EL HISTORIAL ACADEMICO DE UN ALUMNO.\n";

					cout << "\nIndique el Numero de Cuenta del Alumno:";
					int ncuenta;
					cin >> ncuenta;

					EntidadEducativa::consultarHistorial(ncuenta);

					break;
				}

				case 3: {
					cout << "\n\nS A L I E N D O ----> regresando a menu principal.\n";
					break;
				}

				default: {
					cout << "\n\nINTENTA DENUEVO, OPCION NO VALIDA.......\n";
					break;
				}
				}

				break;
			}
		}

		case 4: {
			cout << "\n\nS A L I E N D O ----->  ------>\nGracias por usar nuestro sistema.\n";
			break;
		}

		default: {
			cout << "\n\nINTENTA DENUEVO, OPCION NO VALIDA.......\n";
			break;
		}

		}
	}

	_getch();
}