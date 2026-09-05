// ============================================================================
//  PITA - Programa Integrado de Transacciones Academicas
//  UPC - Universidad Popular del Cesar
//  Taller 1: Estructura de Datos - Listas Doblemente Enlazadas Manuales
//  Normativa: Decreto 1279 de 2002 y Acuerdo 027 de 2024 (simulacion)
// ============================================================================
//  REGLAS CUMPLIDAS:
//  - Unico archivo fuente consolidado.
//  - UNICAMENTE listas doblemente enlazadas manuales. Cero arreglos dinamicos,
//    vectores, maps ni contenedores STL.
//  - CRUD completo: Crear, Listar (Incluir), Consultar, Modificar,
//    Desactivar (soft-delete) y Eliminar fisico.
//  - Persistencia en archivos .txt planos (uno por entidad).
//  - Carga opcional al iniciar.
//  - Menu de consola 100% manual y guiado.
//  - Docentes como objetos separados con calculo de nomina diferenciado.
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
using namespace std;

// ==================== UTILIDADES ====================
void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.get();
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================================
//  UTILIDADES DE PERSISTENCIA Y FORMATO
// ============================================================================

/**
 * Convierte un valor booleano a su representacion en string
 * @param valor - true o false
 * @return "1" si es true, "0" si es false
 */
string boolAString(bool valor) { //<--convierte un valor de 1 o 0 a un string
    return valor ? "1" : "0";
}


bool stringABool(const string& texto) { //<--Convierte un string a valor booleano
    return (texto == "1" || texto == "true");
}

void imprimirLinea(int longitud = 80, char caracter = '-') { //<--Imprime una linea decorativa en la consola
    cout << string(longitud, caracter) << "\n";
}


/**
 * Versiones alternativas con nombres mas descriptivos
 * para diferentes contextos
 */
void separador() {
    imprimirLinea(80, '=');
}

void separadorDelgado() {
    imprimirLinea(80, '-');
}

void titulo(const string& texto) {
    imprimirLinea(80, '=');
    cout << "  " << texto << "\n";
    imprimirLinea(80, '=');
}


// ==================== TAD LISTA GENERICA DOBLEMENTE ENLAZADA ====================
template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;
    Nodo(T _dato) : dato(_dato), siguiente(nullptr), anterior(nullptr) {}
};

template <typename T>
class Lista {
public:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamano;
    Lista() : cabeza(nullptr), cola(nullptr), tamano(0) {}
    ~Lista() {
        Nodo<T>* actual = cabeza;
        while (actual) {
            Nodo<T>* aux = actual;
            actual = actual->siguiente;
            delete aux;
        }
    }
    void agregar(T dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (!cabeza) { cabeza = cola = nuevo; }
        else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
        tamano++;
    }
    void limpiar() {
        Nodo<T>* actual = cabeza;
        while (actual) {
            Nodo<T>* aux = actual;
            actual = actual->siguiente;
            delete aux;
        }
        cabeza = cola = nullptr; tamano = 0;
    }
};

// ==================== ESTRUCTURAS DE DATOS ====================
struct Facultad {
    int id;
	string nombre; 
	string director; 
	bool activo;
};


struct Programa {
    int id; 
	string nombre; 
	int id_facultad; 
	bool activo;
};


struct Curso {
    int id; 
	string nombre; 
	int creditos; 
	int id_programa; 
	bool activo;
};


struct Profesor {
    int id; 
	string nombre; 
	string apellido; 
	string documento;
    string tipo_vinculacion;      //<--Planta | Ocasional | Catedratico
    string categoria_escalafon;   //<--Auxiliar | Asistente | Asociado | Titular
    int puntos_titulo;
    int puntos_experiencia;
    int puntos_productividad;
    double valor_punto;
    int horas_catedra;
    double valor_hora;
    double salario_pactado;
    bool activo;
};

struct Estudiante {
    int id; 
	string nombre; 
	string apellido; 
	string documento;
    int id_programa; 
	bool activo;
};

struct Administrativo {
    int id; 
	string nombre; 
	string apellido; 
	string documento;
    string cargo; 
	string tipo_contratacion; 
	double salario; 
	bool activo;
};


struct Matricula {
    int id_estudiante; 
	int id_curso; 
	double nota; 
	bool activo;
};

// ==================== VARIABLES GLOBALES ====================
Lista<Facultad>g_facultades;
Lista<Programa>g_programas;
Lista<Curso>g_cursos;
Lista<Profesor>g_profesores;
Lista<Estudiante>g_estudiantes;
Lista<Administrativo>g_administrativos;
Lista<Matricula>g_matriculas;


int nid_fac=1, nid_prog=1,
 nid_cur=1, nid_prof=1, 
 nid_est=1, nid_adm=1;

// ==================== VALIDACIONES o BUSQUEDAS ====================
bool existeFacultadActiva(int id) {
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p){ 
		if (p->dato.id==id && p->dato.activo){
			return true;
		}
		p=p->siguiente; 
	}
    return false;
}
bool existeProgramaActivo(int id){
    Nodo<Programa>* p = g_programas.cabeza;
    while (p){ 
		if (p->dato.id==id && p->dato.activo){
			return true;
		}
		p=p->siguiente;
	}
	return false;
}
    

bool existeCursoActivo(int id) {
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) { 
		if (p->dato.id==id && p->dato.activo){
			return true;
		}
		p=p->siguiente;  
	}
    return false;
}


bool existeEstudianteActivo(int id){
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) { 
		if (p->dato.id==id && p->dato.activo){
			return true;
		}
		p=p->siguiente;
	}	
    return false;
}

string nombreFacultad(int id) {
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p){ 
	if (p->dato.id==id){
		return p->dato.nombre;
	}
	p=p->siguiente;
	}
    return "N/A";
}

string nombrePrograma(int id){
    Nodo<Programa>* p = g_programas.cabeza;
    while (p){ 
	if (p->dato.id==id){
    		return p->dato.nombre;
	}
	p=p->siguiente;
	}
    return "N/A";
}

string nombreCurso(int id) {
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) { 
	if (p->dato.id==id){
		return p->dato.nombre;
	}
	p=p->siguiente;
 }
    return "N/A";
}


string nombreEstudiante(int id){
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) { 
	if (p->dato.id==id){
		return p->dato.nombre+" "+p->dato.apellido;
	}
	p=p->siguiente;
	}
    return "N/A";
}

// ==================== PERSISTENCIA ====================


void guardarTodo() {
	
	
    ofstream salidaFacultad("facultades.txt");
    Nodo<Facultad>* nodoFacultad = g_facultades.cabeza;
    while (nodoFacultad){ 
	salidaFacultad<<nodoFacultad->dato.id<<"|"<<nodoFacultad->dato.nombre<<"|"<<
	nodoFacultad->dato.director<<"|"<<boolAString(nodoFacultad->dato.activo)<<
	"\n"; nodoFacultad=nodoFacultad->siguiente; 
	}
    salidaFacultad.close();



    ofstream salidaProgramas("programas.txt");
    Nodo<Programa>* NodoProgramas = g_programas.cabeza;
    while (NodoProgramas) { salidaProgramas<<NodoProgramas->dato.id<<"|"<<NodoProgramas->dato.nombre<<"|"<<
	NodoProgramas->dato.id_facultad<<"|"<<boolAString(NodoProgramas->dato.activo)<<
	"\n"; NodoProgramas=NodoProgramas->siguiente;
	}
    salidaProgramas.close();
    
    

    ofstream salidaCursos("cursos.txt");
    Nodo<Curso>* NodoCursos = g_cursos.cabeza;
    while (NodoCursos) { salidaCursos<<NodoCursos->dato.id<<"|"<<NodoCursos->dato.nombre<<"|"<<
	NodoCursos->dato.creditos<<"|"<<NodoCursos->dato.id_programa<<"|"<<
	boolAString(NodoCursos->dato.activo)<<"\n"; NodoCursos=NodoCursos->siguiente;
	}
    salidaCursos.close();



    ofstream salidaProfesores("profesores.txt");
    Nodo<Profesor>* NodoProfesores = g_profesores.cabeza;
    while (NodoProfesores) {
        Profesor& pr = NodoProfesores->dato;
        salidaProfesores<<pr.id<<"|"<<pr.nombre<<"|"<<pr.apellido<<"|"<<pr.documento<<"|"
           <<pr.tipo_vinculacion<<"|"<<pr.categoria_escalafon<<"|"
           <<pr.puntos_titulo<<"|"<<pr.puntos_experiencia<<"|"<<pr.puntos_productividad<<"|"
           <<pr.valor_punto<<"|"<<pr.horas_catedra<<"|"<<pr.valor_hora<<"|"
           <<pr.salario_pactado<<"|"<<boolAString(pr.activo)<<"\n";
        NodoProfesores=NodoProfesores->siguiente;
    }
    salidaProfesores.close();



    ofstream salidaEstudiantes("estudiantes.txt");
    Nodo<Estudiante>* NodoEstudiantes = g_estudiantes.cabeza;
    while (NodoEstudiantes) {
        salidaEstudiantes<<NodoEstudiantes->dato.id<<"|"<<NodoEstudiantes->dato.nombre<<"|"
		<<NodoEstudiantes->dato.apellido<<"|"<<NodoEstudiantes->dato.documento<<"|"
		<<NodoEstudiantes->dato.id_programa<<"|"<<boolAString(NodoEstudiantes->dato.activo)<<"\n";
        NodoEstudiantes=NodoEstudiantes->siguiente;
    }
    salidaEstudiantes.close();
    
    

    ofstream salidaAdministrativos("administrativos.txt");
    Nodo<Administrativo>* NodoAdministrativos = g_administrativos.cabeza;
    while (NodoAdministrativos) {
        salidaAdministrativos<<NodoAdministrativos->dato.id<<"|"<<NodoAdministrativos->dato.nombre<<"|"
		<<NodoAdministrativos->dato.apellido<<"|"<<NodoAdministrativos->dato.documento<<
		"|"<<NodoAdministrativos->dato.cargo<<"|"<<NodoAdministrativos->dato.tipo_contratacion<<"|"
        <<NodoAdministrativos->dato.salario<<"|"<<boolAString(NodoAdministrativos->dato.activo)<<"\n";
        NodoAdministrativos=NodoAdministrativos->siguiente;
    }
    salidaAdministrativos.close();



    ofstream salidaMatriculas("matriculas.txt");
    Nodo<Matricula>* NodoMatriculas = g_matriculas.cabeza;
    while (NodoMatriculas) {
        salidaMatriculas<<NodoMatriculas->dato.id_estudiante<<"|"<<NodoMatriculas->dato.id_curso<<
		"|"<<NodoMatriculas->dato.nota<<"|"<<boolAString(NodoMatriculas->dato.activo)<<"\n";
        NodoMatriculas=NodoMatriculas->siguiente;
    }
    salidaMatriculas.close();


    cout << "[OK] Datos guardados en archivos .txt\n";
}

void cargarTodo() {
    ifstream fila1("facultades.txt");
    if (fila1.is_open()) {
        string line;
        while (getline(fila1, line)) {
            stringstream ss(line); 
			Facultad f; 
			string tmp;
            getline(ss, tmp, '|'); 
			f.id = stoi(tmp);
            getline(ss, f.nombre, '|');
            getline(ss, f.director, '|');
            getline(ss, tmp, '|'); 
			f.activo = stringABool(tmp);
            g_facultades.agregar(f);
            if (f.id >= nid_fac){
            	nid_fac = f.id + 1;
			}
        }
        fila1.close();
    }
    
    ifstream fila2("programas.txt");
    if (fila2.is_open()) {
        string line;
        while (getline(fila2, line)) {
            stringstream ss(line); 
			Programa p; 
			string tmp;
            getline(ss, tmp, '|'); 
			p.id = stoi(tmp);
            getline(ss, p.nombre, '|');
            getline(ss, tmp, '|'); 
			p.id_facultad = stoi(tmp);
            getline(ss, tmp, '|'); 
			p.activo = stringABool(tmp);
            g_programas.agregar(p);
            if (p.id >= nid_prog){
            	nid_prog = p.id + 1;	
			}
        }
        fila2.close();
    }
    ifstream fila3("cursos.txt");
    if (fila3.is_open()) {
        string line;
        while (getline(fila3, line)) {
            stringstream ss(line); 
			Curso c; 
			string tmp;
            getline(ss, tmp, '|'); 
			c.id = stoi(tmp);
            getline(ss, c.nombre, '|');
            getline(ss, tmp, '|'); 
			c.creditos = stoi(tmp);
            getline(ss, tmp, '|'); 
			c.id_programa = stoi(tmp);
            getline(ss, tmp, '|'); 
			c.activo = stringABool(tmp);
            g_cursos.agregar(c);
            if (c.id >= nid_cur){
            	nid_cur = c.id + 1;
			} 
        }
        fila3.close();
    }
    ifstream fila4("profesores.txt");
    if (fila4.is_open()) {
        string line;
        while (getline(fila4, line)) {
            stringstream ss(line); 
			Profesor p; 
			string tmp;
            getline(ss, tmp, '|'); 
			p.id = stoi(tmp);
            getline(ss, p.nombre, '|');
            getline(ss, p.apellido, '|');
            getline(ss, p.documento, '|');
            getline(ss, p.tipo_vinculacion, '|');
            getline(ss, p.categoria_escalafon, '|');
            getline(ss, tmp, '|'); 
			p.puntos_titulo = stoi(tmp);
            getline(ss, tmp, '|'); 
			p.puntos_experiencia = stoi(tmp);
            getline(ss, tmp, '|'); 
			p.puntos_productividad = stoi(tmp);
            getline(ss, tmp, '|'); 
			p.valor_punto = stod(tmp);
            getline(ss, tmp, '|'); 
			p.horas_catedra = stoi(tmp);
            getline(ss, tmp, '|'); 
			p.valor_hora = stod(tmp);
            getline(ss, tmp, '|'); 
			p.salario_pactado = stod(tmp);
            getline(ss, tmp, '|');
			p.activo = stringABool(tmp);
            g_profesores.agregar(p);
            if (p.id >= nid_prof){
            	nid_prof = p.id + 1;
			} 
        }
        fila4.close();
    }
    ifstream fila5("estudiantes.txt");
    if (fila5.is_open()) {
        string line;
        while (getline(fila5, line)) {
            stringstream ss(line); 
			Estudiante e; 
			string tmp;
            getline(ss, tmp, '|'); 
			e.id = stoi(tmp);
            getline(ss, e.nombre, '|');
            getline(ss, e.apellido, '|');
            getline(ss, e.documento, '|');
            getline(ss, tmp, '|'); 
			e.id_programa = stoi(tmp);
            getline(ss, tmp, '|'); 
			e.activo = stringABool(tmp);
            g_estudiantes.agregar(e);
            if (e.id >= nid_est){
            	nid_est = e.id + 1;
			} 
        }
        fila5.close();
    }
    ifstream fila6("administrativos.txt");
    if (fila6.is_open()) {
        string line;
        while (getline(fila6, line)) {
            stringstream ss(line); 
			Administrativo a; string tmp;
            getline(ss, tmp, '|'); 
			a.id = stoi(tmp);
            getline(ss, a.nombre, '|');
            getline(ss, a.apellido, '|');
            getline(ss, a.documento, '|');
            getline(ss, a.cargo, '|');
            getline(ss, a.tipo_contratacion, '|');
            getline(ss, tmp, '|'); 
			a.salario = stod(tmp);
            getline(ss, tmp, '|'); 
			a.activo = stringABool(tmp);
            g_administrativos.agregar(a);
            if (a.id >= nid_adm) nid_adm = a.id + 1;
        }
        fila6.close();
    }
    ifstream fila7("matriculas.txt");
    if (fila7.is_open()) {
        string line;
        while (getline(fila7, line)) {
            stringstream ss(line); 
			Matricula m; string tmp;
            getline(ss, tmp, '|'); 
			m.id_estudiante = stoi(tmp);
            getline(ss, tmp, '|'); 
			m.id_curso = stoi(tmp);
            getline(ss, tmp, '|'); 
			m.nota = stod(tmp);
            getline(ss, tmp, '|'); 
			m.activo = stringABool(tmp);
            g_matriculas.agregar(m);
        }
        fila7.close();
    }
    cout << "[OK] Datos cargados desde archivos .txt (si existian).\n";
}

// ==================== FACULTADES CRUD ====================
void crearFacultad() {
    Facultad f;
    f.id = nid_fac++;
    cout << "Nombre de la facultad: "; 
	getline(cin, f.nombre);
    cout << "Director: "; 
	getline(cin, f.director);
    f.activo = true;
    g_facultades.agregar(f);
    cout << "[OK] Facultad creada con ID: " << f.id << "\n";
}

void listarFacultades(bool todos = false) {
    cout << left << setw(5) << "ID" << setw(30) << "NOMBRE" << setw(30) << "DIRECTOR" << setw(10) << "ESTADO" << "\n";
    imprimirLinea(75);
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(30) << p->dato.nombre
                 << setw(30) << p->dato.director
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarFacultad() {
    int id; cout << "ID a consultar: "; cin >> id; 
	limpiarBuffer();
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p) {
        if (p->dato.id == id) {
            cout << "ID: " << p->dato.id << "\nNombre: " << p->dato.nombre
                 << "\nDirector: " << p->dato.director
                 << "\nEstado: " << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrada.\n";
}

void modificarFacultad() {
    int id; cout << "ID a modificar: "; cin >> id; 
	limpiarBuffer();
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            cout << "Nuevo nombre: "; 
			getline(cin, p->dato.nombre);
            cout << "Nuevo director: "; 
			getline(cin, p->dato.director);
            cout << "[OK] Modificada.\n"; return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrada o inactiva.\n";
}

void desactivarFacultad() {
    int id; cout << "ID a desactivar (logico): "; cin >> id; limpiarBuffer();
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p) {
        if (p->dato.id == id) {
			 p->dato.activo = false;
			 cout << "[OK] Desactivada.\n";
	    	 return; 
		 	}
        p = p->siguiente;
    }
    cout << "[X] No encontrada.\n";
}

void eliminarFacultadFisico() {
    int id; cout << "ID a eliminar (fisico): "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Facultad>* p = g_facultades.cabeza;
    while (p) {
        if (p->dato.id == id) {
            if (p->anterior){
            	p->anterior->siguiente = p->siguiente;
			} 
            else {
            	g_facultades.cabeza = p->siguiente;
			}
            if (p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_facultades.cola = p->anterior;
			} 
            delete p; g_facultades.tamano--;
            
            cout << "[OK] Eliminada fisicamente.\n"; return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrada.\n";
}

void menuFacultades() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LAS FACULTADES               |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Crear una Facultad                                |\n";
    	cout<<"| 2. Listar Facultades Activas                         |\n";
    	cout<<"| 3. Consultar Facultad                                |\n";
    	cout<<"| 4. Modificar Facultad                                |\n";
    	cout<<"| 5. Desactivar Facultad [LOGICO]                      |\n";
    	cout<<"| 6. Eliminar Facultad [FISICO]                        |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: 
			crearFacultad(); 
			break;
			
            case 2: 
			listarFacultades(); 
			break;
			
            case 3: 
			consultarFacultad(); 
			break;
			
            case 4: 
			modificarFacultad(); 
			break;
			
            case 5: 
			desactivarFacultad(); 
			break;
			
            case 6: 
			eliminarFacultadFisico(); 
			break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== PROGRAMAS CRUD ====================
void crearPrograma() {
    if (!g_facultades.cabeza) { 
	cout << "[X] No hay facultades.\n"; return; 
	}
    listarFacultades();
    Programa p; 
	p.id = nid_prog++;
    cout << "Nombre del programa: "; 
	getline(cin, p.nombre);
    cout << "ID de la facultad: ";
	cin >> p.id_facultad;
	limpiarBuffer();
	
    if (!existeFacultadActiva(p.id_facultad)) {
	 cout << "[X] Facultad invalida.\n"; 
	 nid_prog--; 
	 return;
	}
	
    p.activo = true;
    g_programas.agregar(p);
    cout << "[OK] Programa creado con ID: " << p.id << "\n";
}

void listarProgramas(bool todos = false) {
    cout << left << setw(5)<<"ID" << setw(30)<<"NOMBRE" << setw(30)<<"FACULTAD" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(75);
    Nodo<Programa>* p = g_programas.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(30) << p->dato.nombre
                 << setw(30) << nombreFacultad(p->dato.id_facultad)
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarPrograma() {
    int id; cout << "ID a consultar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Programa>* p = g_programas.cabeza;
    while (p) {
        if (p->dato.id == id) {
            cout << "ID: " << p->dato.id << "\nNombre: " << p->dato.nombre
                 << "\nFacultad: " << nombreFacultad(p->dato.id_facultad)
                 << "\nEstado: " << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void modificarPrograma() {
    int id; cout << "ID a modificar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Programa>* p = g_programas.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            cout << "Nuevo nombre: "; 
			getline(cin, p->dato.nombre);
            listarFacultades();
            cout << "Nuevo ID facultad: "; 
			cin >> p->dato.id_facultad; 
			limpiarBuffer();
            if (!existeFacultadActiva(p->dato.id_facultad)) {
			 cout << "[X] Facultad invalida.\n";
			 return; 
			 }
            cout << "[OK] Modificado.\n"; return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado o inactivo.\n";
}

void desactivarPrograma() {
    int id; cout << "ID a desactivar: "; 
	cin >> id; limpiarBuffer();
    Nodo<Programa>* p = g_programas.cabeza;
    
    while (p) { if (p->dato.id == id){
	 p->dato.activo = false;
	 cout << "[OK] Desactivado.\n"; 
	 return; 
	 } 
	 p = p->siguiente; }
    cout << "[X] No encontrado.\n";
}

void eliminarProgramaFisico() {
    int id; 
	cout << "ID a eliminar fisico: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Programa>* p = g_programas.cabeza;
    while (p) {
        if (p->dato.id == id) {
            if(p->anterior){
            	p->anterior->siguiente = p->siguiente;
			}  
            else{
            	g_programas.cabeza = p->siguiente;
			} 
            if (p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_programas.cola = p->anterior;
			} 
            delete p; 
			g_programas.tamano--;
            cout << "[OK] Eliminado fisicamente.\n"; return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void menuProgramas() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LOS PROGRAMAS                |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Crear un Programa                                 |\n";
    	cout<<"| 2. Listar Programas Activos                          |\n";
    	cout<<"| 3. Consultar Programa                                |\n";
    	cout<<"| 4. Modificar Programa                                |\n";
    	cout<<"| 5. Desactivar Programa [LOGICO]                      |\n";
    	cout<<"| 6. Eliminar Programa [FISICO]                        |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: 
			crearPrograma(); 
			break;
			
            case 2: 
			listarProgramas();
			break;
			
            case 3: 
			consultarPrograma();
			break;
			 
            case 4: 
			modificarPrograma();
			break;
			
            case 5: 
			desactivarPrograma(); 
			break;
			
            case 6: 
			eliminarProgramaFisico();
			break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== CURSOS CRUD ====================
void crearCurso() {
    if (!g_programas.cabeza) { 
	cout << "[X] No hay programas.\n";
	return;
	}
	
    listarProgramas();
    
    Curso c; 
	c.id = nid_cur++;
    cout << "Nombre del curso: "; 
	getline(cin, c.nombre);
    cout << "Creditos: "; 
	cin >> c.creditos; 
	limpiarBuffer();
    cout << "ID del programa: "; 
	cin >> c.id_programa; 
	limpiarBuffer();
    if (!existeProgramaActivo(c.id_programa)){
	 cout << "[X] Programa invalido.\n";
	 nid_cur--;
	 return;
	  }
    c.activo = true;
    g_cursos.agregar(c);
    cout << "[OK] Curso creado con ID: " << c.id << "\n";
}

void listarCursos(bool todos = false) {
    cout << left << setw(5)<<"ID" << setw(30)<<"NOMBRE" << setw(10)<<"CRED" << setw(30)<<"PROGRAMA" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(85);
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(30) << p->dato.nombre
                 << setw(10) << p->dato.creditos
                 << setw(30) << nombrePrograma(p->dato.id_programa)
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarCurso() {
    int id; cout << "ID a consultar: ";
	cin >> id; 
	limpiarBuffer();
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) {
        if (p->dato.id == id) {
            cout << "ID: " << p->dato.id
                             << "\nNombre: " << p->dato.nombre
                 << "\nCreditos: " << p->dato.creditos
                 << "\nPrograma: " << nombrePrograma(p->dato.id_programa)
                 << "\nEstado: " << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void modificarCurso() {
    int id; cout << "ID a modificar: ";
	cin >> id;
	limpiarBuffer();
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            cout << "Nuevo nombre: "; 
			getline(cin, p->dato.nombre);
            cout << "Nuevos creditos: "; 
			cin >> p->dato.creditos; 
			limpiarBuffer();
            listarProgramas();
            cout << "Nuevo ID programa: "; 
			cin >> p->dato.id_programa; 
			limpiarBuffer();
            if (!existeProgramaActivo(p->dato.id_programa)){
			 cout << "[X] Programa invalido.\n";
			 return; 
	    	}
            cout << "[OK] Modificado.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado o inactivo.\n";
}

void desactivarCurso() {
    int id; 
	cout << "ID a desactivar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) { 
	if (p->dato.id == id){ 
	p->dato.activo = false;
	cout << "[OK] Desactivado.\n"; 
	return; 
	} 
	p = p->siguiente; 
	}
    cout << "[X] No encontrado.\n";
}

void eliminarCursoFisico() {
    int id; 
	cout << "ID a eliminar fisico: ";
	cin >> id; 
	limpiarBuffer();
    Nodo<Curso>* p = g_cursos.cabeza;
    while (p) {
        if (p->dato.id == id) {
            if (p->anterior){
            	p->anterior->siguiente = p->siguiente;
			} 
            else{
            	g_cursos.cabeza = p->siguiente;
			} 
            if(p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_cursos.cola = p->anterior;
			}
            delete p; 
			g_cursos.tamano--;
            cout << "[OK] Eliminado fisicamente.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void menuCursos() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LOS CURSOS                   |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Crear un Curso                                    |\n";
    	cout<<"| 2. Listar Cursos Activos                             |\n";
    	cout<<"| 3. Consultar Curso                                   |\n";
    	cout<<"| 4. Modificar Curso                                   |\n";
    	cout<<"| 5. Desactivar Curso    [LOGICO]                      |\n";
    	cout<<"| 6. Eliminar Curso    [FISICO]                        |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: crearCurso(); break;
            case 2: listarCursos(); break;
            case 3: consultarCurso(); break;
            case 4: modificarCurso(); break;
            case 5: desactivarCurso(); break;
            case 6: eliminarCursoFisico(); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== PROFESORES CRUD ====================
void crearProfesor() {
    Profesor p; 
	p.id = nid_prof++;
    cout << "Nombre: "; 
	getline(cin, p.nombre);
    cout << "Apellido: "; 
	getline(cin, p.apellido);
    cout << "Documento: "; 
	getline(cin, p.documento);
    cout << "Tipo vinculacion (1.Planta/ 2.Ocasional/ 3.Catedratico): "; 
	getline(cin, p.tipo_vinculacion);
    if (p.tipo_vinculacion == "1" ){
        cout << "Categoria escalafon (Auxiliar/Asistente/Asociado/Titular): "; 
		getline(cin, p.categoria_escalafon);
        cout << "Puntos titulo: "; 
		cin >> p.puntos_titulo;
		limpiarBuffer();
        cout << "Puntos experiencia: "; 
		cin >> p.puntos_experiencia;
		limpiarBuffer();
        cout << "Puntos productividad: ";
	    cin >> p.puntos_productividad;
		limpiarBuffer();
        cout << "Valor del punto ($): ";
		cin >> p.valor_punto;
		limpiarBuffer();
        p.horas_catedra = 0;
		p.valor_hora = 0;
		p.salario_pactado = 0;
    } else if (p.tipo_vinculacion == "2") {
        cout << "Salario pactado ($): "; 
		cin >> p.salario_pactado; 
		limpiarBuffer();
        p.categoria_escalafon = "N/A"; 
		p.puntos_titulo = 0; 
		p.puntos_experiencia = 0;
        p.puntos_productividad = 0; 
		p.valor_punto = 0; 
		p.horas_catedra = 0; 
		p.valor_hora = 0;
    } else if (p.tipo_vinculacion == "3") {
        cout << "Horas catedra (semanales): "; 
		cin >> p.horas_catedra; 
		limpiarBuffer();
        cout << "Valor hora ($): "; 
		cin >> p.valor_hora; 
		limpiarBuffer();
        p.categoria_escalafon = "N/A"; 
		p.puntos_titulo = 0; 
		p.puntos_experiencia = 0;
        p.puntos_productividad = 0; 
		p.valor_punto = 0; 
		p.salario_pactado = 0;
    } else {
        cout << "[X] Tipo invalido.\n"; 
		nid_prof--; 
		return;
    }
    p.activo = true;
    g_profesores.agregar(p);
    cout << "[OK] Profesor creado con ID: " << p.id << "\n";
}

void listarProfesores(bool todos = false) {
    cout << left << setw(5)<<"ID" << setw(20)<<"NOMBRE" << setw(15)<<"VINCULACION"
         << setw(15)<<"CATEGORIA" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(65);
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(20) << (p->dato.nombre + " " + p->dato.apellido)
                 << setw(15) << p->dato.tipo_vinculacion
                 << setw(15) << p->dato.categoria_escalafon
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarProfesor() {
    int id; cout << "ID a consultar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p) {
        if (p->dato.id == id) {
            Profesor& pr = p->dato;
            cout << "ID: " << pr.id << "\nNombre: " << pr.nombre << " " << pr.apellido
                 << "\nDocumento: " << pr.documento
                 << "\nVinculacion: " << pr.tipo_vinculacion
                 << "\nCategoria: " << pr.categoria_escalafon
                 << "\nEstado: " << (pr.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void modificarProfesor() {
    int id; 
	cout << "ID a modificar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            Profesor& pr = p->dato;
            cout << "Nuevo nombre: "; 
			getline(cin, pr.nombre);
            cout << "Nuevo apellido: "; 
			getline(cin, pr.apellido);
            cout << "Nuevo documento: "; 
			getline(cin, pr.documento);
            //para evitar inconsistencias de campos  NO se permite cambiar tipo de vinculacion 
            cout << "[OK] Modificado.\n"; return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado o inactivo.\n";
}

void desactivarProfesor() {
    int id; cout << "ID a desactivar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p) {
	 if (p->dato.id == id){
	  p->dato.activo = false;
	   cout << "[OK] Desactivado.\n";
	   return; 
	   }
	    p = p->siguiente; 
	 }
    cout << "[X] No encontrado.\n";
}

void eliminarProfesorFisico() {
    int id;
	cout << "ID a eliminar fisico: "; 
	cin >> id;
	limpiarBuffer();
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p){
        if (p->dato.id == id) {
            if(p->anterior){
            	p->anterior->siguiente = p->siguiente;
			}  
            else{
            	g_profesores.cabeza = p->siguiente;
			} 
            if(p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_profesores.cola = p->anterior;
			} 
            delete p;
			g_profesores.tamano--;
            cout << "[OK] Eliminado fisicamente.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void menuProfesores() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LOS PROFESORES               |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Anadir un Profesor Nuevo                          |\n";
    	cout<<"| 2. Listar Profesores Activos                         |\n";
    	cout<<"| 3. Consultar Profesores                              |\n";
    	cout<<"| 4. Modificar Informacion de un Profesor              |\n";
    	cout<<"| 5. Desactivar Profesor    [LOGICO]                   |\n";
    	cout<<"| 6. Eliminar a un Profesor    [FISICO]                |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: 
			crearProfesor();
		    break;
		    
            case 2:
			listarProfesores();
		    break;
		    
            case 3: 
			consultarProfesor();
			break;
			
            case 4: 
			modificarProfesor(); 
			break;
			
            case 5: 
			desactivarProfesor(); 
			break;
			
            case 6: 
			eliminarProfesorFisico(); 
			break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== ESTUDIANTES CRUD ====================
void crearEstudiante() {
    if (!g_programas.cabeza){ 
	cout << "[X] No hay programas.\n";
	 return; 
	 }
	 
    listarProgramas();
    Estudiante e; 
	e.id = nid_est++;
    cout << "Nombre: "; 
	getline(cin, e.nombre);
    cout << "Apellido: "; 
	getline(cin, e.apellido);
    cout << "Documento: "; 
	getline(cin, e.documento);
    cout << "ID del programa: "; 
	cin >> e.id_programa; 
	limpiarBuffer();
    if (!existeProgramaActivo(e.id_programa)){ 
	cout << "[X] Programa invalido.\n"; 
	nid_est--; 
	return; 
	}
    e.activo = true;
    g_estudiantes.agregar(e);
    cout << "[OK] Estudiante creado con ID: " << e.id << "\n";
}

void listarEstudiantes(bool todos = false) {
    cout << left << setw(5)<<"ID" << setw(20)<<"NOMBRE" << setw(20)<<"APELLIDO"
         << setw(15)<<"DOCUMENTO" << setw(25)<<"PROGRAMA" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(95);
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(20) << p->dato.nombre
                 << setw(20) << p->dato.apellido
                 << setw(15) << p->dato.documento
                 << setw(25) << nombrePrograma(p->dato.id_programa)
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarEstudiante() {
    int id; cout << "ID a consultar: ";
	 cin >> id;
	 limpiarBuffer();
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) {
        if (p->dato.id == id) {
            cout << "ID: " << p->dato.id << "\nNombre: " << p->dato.nombre << " " << p->dato.apellido
                 << "\nDocumento: " << p->dato.documento
                 << "\nPrograma: " << nombrePrograma(p->dato.id_programa)
                 << "\nEstado: " << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void modificarEstudiante() {
    int id; 
	cout << "ID a modificar: ";
	cin >> id; 
	limpiarBuffer();
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            cout << "Nuevo nombre: ";
			getline(cin, p->dato.nombre);
            cout << "Nuevo apellido: ";
			getline(cin, p->dato.apellido);
            cout << "Nuevo documento: ";
			getline(cin, p->dato.documento);
            listarProgramas();
            cout << "Nuevo ID programa: ";
			cin >> p->dato.id_programa; 
			limpiarBuffer();
            if (!existeProgramaActivo(p->dato.id_programa)) {
			 cout << "[X] Programa invalido.\n";
			  return; 
			  }
            cout << "[OK] Modificado.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado o inactivo.\n";
}

void desactivarEstudiante() {
    int id; 
	cout << "ID a desactivar: ";
	cin >> id; 
	limpiarBuffer();
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p){
	 if (p->dato.id == id){
	  p->dato.activo = false;
	  cout << "[OK] Desactivado.\n"; 
	  return; 
	  }
	  p = p->siguiente; 
	  }
    cout << "[X] No encontrado.\n";
}

void eliminarEstudianteFisico() {
    int id; 
	cout << "ID a eliminar fisico: ";
	 cin >> id; 
	 limpiarBuffer();
    Nodo<Estudiante>* p = g_estudiantes.cabeza;
    while (p) {
        if (p->dato.id == id) {
            if (p->anterior){
            	p->anterior->siguiente = p->siguiente;
			} 
            else{
            	g_estudiantes.cabeza = p->siguiente;
			} 
            if (p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_estudiantes.cola = p->anterior;
			} 
            delete p; 
			g_estudiantes.tamano--;
            cout << "[OK] Eliminado fisicamente.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void menuEstudiantes() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LOS ESTUDIANTES               |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Anadir un Estudiante Nuevo                        |\n";
    	cout<<"| 2. Listar Estudiantes Activos                        |\n";
    	cout<<"| 3. Consultar Entudiante                              |\n";
    	cout<<"| 4. Modificar Informacion de un Estudiante            |\n";
    	cout<<"| 5. Desactivar Estudiante  [LOGICO]                   |\n";
    	cout<<"| 6. Eliminar a un Estudiante  [FISICO]                |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: 
			crearEstudiante();
			 break;
			 
            case 2: 
			listarEstudiantes(); 
			break;
			
            case 3: 
			consultarEstudiante(); 
			break;
			
            case 4: 
			modificarEstudiante(); 
			break;
			
            case 5: 
			desactivarEstudiante(); 
			break;
			
            case 6: 
			eliminarEstudianteFisico(); 
			break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== ADMINISTRATIVOS CRUD ====================
void crearAdministrativo() {
    Administrativo a;
	 a.id = nid_adm++;
    cout << "Nombre: ";
	getline(cin, a.nombre);
    cout << "Apellido: ";
	getline(cin, a.apellido);
    cout << "Documento: "; 
	getline(cin, a.documento);
    cout << "Cargo: ";
	getline(cin, a.cargo);
    cout << "Tipo de contratacion: "; 
	getline(cin, a.tipo_contratacion);
    cout << "Salario ($): "; 
	cin >> a.salario; 
	limpiarBuffer();
    a.activo = true;
    g_administrativos.agregar(a);
    cout << "[OK] Administrativo creado con ID: " << a.id << "\n";
}

void listarAdministrativos(bool todos = false) {
    cout << left << setw(5)<<"ID" << setw(20)<<"NOMBRE" << setw(20)<<"APELLIDO"
         << setw(15)<<"DOCUMENTO" << setw(20)<<"CARGO" << setw(15)<<"CONTRAT" << setw(12)<<"SALARIO" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(117);
    Nodo<Administrativo>* p = g_administrativos.cabeza;
    while (p) {
        if (p->dato.activo || todos) {
            cout << left << setw(5) << p->dato.id
                 << setw(20) << p->dato.nombre
                 << setw(20) << p->dato.apellido
                 << setw(15) << p->dato.documento
                 << setw(20) << p->dato.cargo
                 << setw(15) << p->dato.tipo_contratacion
                 << setw(12) << fixed << setprecision(0) << p->dato.salario
                 << setw(10) << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
        }
        p = p->siguiente;
    }
}

void consultarAdministrativo() {
    int id; 
	cout << "ID a consultar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Administrativo>* p = g_administrativos.cabeza;
    while (p) {
        if (p->dato.id == id) {
            cout << "ID: " << p->dato.id << "\nNombre: " << p->dato.nombre << " " << p->dato.apellido
                 << "\nDocumento: " << p->dato.documento
                 << "\nCargo: " << p->dato.cargo
                 << "\nContratacion: " << p->dato.tipo_contratacion
                 << "\nSalario: " << p->dato.salario
                 << "\nEstado: " << (p->dato.activo ? "Activo" : "Inactivo") << "\n";
            return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void modificarAdministrativo() {
    int id; 
	cout << "ID a modificar: ";
	cin >> id; limpiarBuffer();
    Nodo<Administrativo>* p = g_administrativos.cabeza;
    while (p) {
        if (p->dato.id == id && p->dato.activo) {
            cout << "Nuevo nombre: ";
			getline(cin, p->dato.nombre);
            cout << "Nuevo apellido: ";
			getline(cin, p->dato.apellido);
            cout << "Nuevo documento: "; 
			getline(cin, p->dato.documento);
            cout << "Nuevo cargo: "; 
			getline(cin, p->dato.cargo);
            cout << "Nueva contratacion: "; 
			getline(cin, p->dato.tipo_contratacion);
            cout << "Nuevo salario: "; 
			cin >> p->dato.salario; 
			limpiarBuffer();
            cout << "[OK] Modificado.\n"; 
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado o inactivo.\n";
}

void desactivarAdministrativo() {
    int id; 
	cout << "ID a desactivar: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Administrativo>* p = g_administrativos.cabeza;
    while (p) { if (p->dato.id == id) { 
	p->dato.activo = false; 
	cout << "[OK] Desactivado.\n";
     return;
	  } 
	  p = p->siguiente; 
	  }
    cout << "[X] No encontrado.\n";
}

void eliminarAdministrativoFisico() {
    int id; 
	cout << "ID a eliminar fisico: "; 
	cin >> id; 
	limpiarBuffer();
    Nodo<Administrativo>* p = g_administrativos.cabeza;
    while (p) {
        if (p->dato.id == id) {
            if (p->anterior){
            	p->anterior->siguiente = p->siguiente;
			}
            else{
            	g_administrativos.cabeza = p->siguiente;
			} 
            if (p->siguiente){
            	p->siguiente->anterior = p->anterior;
			} 
            else{
            	g_administrativos.cola = p->anterior;
			} 
            delete p;
			g_administrativos.tamano--;
            cout << "[OK] Eliminado fisicamente.\n";
			return;
        }
        p = p->siguiente;
    }
    cout << "[X] No encontrado.\n";
}

void menuAdministrativos() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE LOS ADMINISTRATIVOS          |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Anadir un Administrativo Nuevo                    |\n";
    	cout<<"| 2. Listar Administrativos Activos                    |\n";
    	cout<<"| 3. Consultar Administrativo                          |\n";
    	cout<<"| 4. Modificar Informacion de un Administrativo        |\n";
    	cout<<"| 5. Desactivar Administrativo  [LOGICO]               |\n";
    	cout<<"| 6. Eliminar a un Administrativo  [FISICO]            |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: 
			crearAdministrativo(); 
			break;
			
            case 2: 
			listarAdministrativos(); 
			break;
			
            case 3: 
			consultarAdministrativo(); 
			break;
			
            case 4: 
			modificarAdministrativo(); 
			break;
			
            case 5: 
			desactivarAdministrativo(); 
			break;
			
            case 6: 
			eliminarAdministrativoFisico();
			break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== GESTION ACADEMICA (MATRICULAS) ====================
void matricularCurso() {
    if (!g_estudiantes.cabeza || !g_cursos.cabeza) { 
	cout << "[X] Faltan estudiantes o cursos.\n";
	 return;
	  }
	  
    listarEstudiantes();
    
    int id_est; 
	cout << "ID estudiante: "; 
	cin >> id_est; 
	limpiarBuffer();
	
    if (!existeEstudianteActivo(id_est)) {
	 cout << "[X] Estudiante invalido.\n";
	  return;
	   }
	   
    listarCursos();
    
    int id_cur;
	 cout << "ID curso: ";
	  cin >> id_cur;
	  limpiarBuffer();
	  
    if (!existeCursoActivo(id_cur)) {
	 cout << "[X] Curso invalido.\n"; 
	 return;
	  }
	  
    // Verificar duplicado activo
    Nodo<Matricula>* m = g_matriculas.cabeza;
    while (m) {
        if (m->dato.id_estudiante == id_est && m->dato.id_curso == id_cur && m->dato.activo) {
            cout << "[X] El estudiante ya esta matriculado en este curso.\n";
			 return;
        }
        m = m->siguiente;
    }
    
    Matricula mat; 
	mat.id_estudiante = id_est;
	mat.id_curso = id_cur; 
	mat.nota = 0.0; 
	mat.activo = true;
    g_matriculas.agregar(mat);
    cout << "[OK] Matricula exitosa.\n";
}

void cancelarCurso() {
    int id_est, id_cur;
    
    cout << "ID estudiante: ";
	 cin >> id_est;
    cout << "ID curso a cancelar: ";
	cin >> id_cur;
	limpiarBuffer();
    Nodo<Matricula>* m = g_matriculas.cabeza;
    while (m) {
        if (m->dato.id_estudiante == id_est && m->dato.id_curso == id_cur && m->dato.activo) {
            m->dato.activo = false;
            cout << "[OK] Curso cancelado (eliminacion logica de matricula).\n";
            return;
        }
        m = m->siguiente;
    }
    cout << "[X] Matricula activa no encontrada.\n";
}

void registrarNota() {
    int id_est, id_cur;
	double nota;
    cout << "ID estudiante: "; 
	cin >> id_est;
    cout << "ID curso: "; 
	cin >> id_cur;
    cout << "Nota (0.0 - 5.0): "; 
	cin >> nota; 
	limpiarBuffer();
	
    if (nota < 0 || nota > 5) { 
	cout << "[X] Nota invalida.\n"; 
	return;
    }
	 
    Nodo<Matricula>* m = g_matriculas.cabeza;
    while (m) {
        if (m->dato.id_estudiante == id_est && m->dato.id_curso == id_cur && m->dato.activo) {
            m->dato.nota = nota;
            cout << "[OK] Nota registrada.\n";
            return;
        }
        m = m->siguiente;
    }
    cout << "[X] Matricula activa no encontrada.\n";
}

void calcularPromedioEstudiante() {
    int id_est;
	cout << "ID estudiante: "; 
	cin >> id_est; 
	limpiarBuffer();
    double suma = 0; 
	int count = 0;
    Nodo<Matricula>* m = g_matriculas.cabeza;
    while (m) {
        if (m->dato.id_estudiante == id_est && m->dato.activo && m->dato.nota >= 0) {
            suma += m->dato.nota; count++;
        }
        m = m->siguiente;
    }
    
    if (count == 0){ 
	cout << "[i] No tiene notas registradas en cursos activos.\n"; 
	return;
	 }
	 
    double prom = suma / count;
    
    cout << "Promedio del estudiante: " << fixed << setprecision(2) << prom << "\n";
    if (prom < 3.0) {
        cout << "!!! ALERTA: El estudiante se encuentra en EBRA (Bajo Rendimiento Academico) !!!\n";
    } else {
        cout << "[OK] Promedio aprobatorio.\n";
    }
}

void listarMatriculasEstudiante() {
    int id_est; 
	cout << "ID estudiante: "; 
	cin >> id_est; 
	limpiarBuffer();
	
    cout << left << setw(5)<<"CURSO_ID" << setw(30)<<"CURSO" << setw(10)<<"NOTA" << setw(10)<<"ESTADO" << "\n";
    imprimirLinea(55);
    Nodo<Matricula>* m = g_matriculas.cabeza;
    while (m) {
        if (m->dato.id_estudiante == id_est && m->dato.activo) {
            cout << left << setw(5) << m->dato.id_curso
                 << setw(30) << nombreCurso(m->dato.id_curso)
                 << setw(10) << fixed << setprecision(1) << m->dato.nota
                 << setw(10) << "Activa" << "\n";
        }
        m = m->siguiente;
    }
}

void menuGestionAcademica() {
    int op;
    do {
    	cout<<"\n|======================================================|\n";
    	cout<<"|              GESTION DE ACADEMICA                    |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Matricular un Curso                               |\n";
    	cout<<"| 2. Cancelar Curso                                    |\n";
    	cout<<"| 3. Registrar Nota                                    |\n";
    	cout<<"| 4. Calcular Promedio  - Alerta EBRA                  |\n";
    	cout<<"| 5. Ver cursos Matriculados                           |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: matricularCurso(); break;
            case 2: cancelarCurso(); break;
            case 3: registrarNota(); break;
            case 4: calcularPromedioEstudiante(); break;
            case 5: listarMatriculasEstudiante(); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== NOMINA DOCENTE ====================
int puntosCategoriaEscalafon(const string& cat) {
    // Valores representativos basados en el Decreto 1279 de 2002
    if (cat == "Titular"){
    	return 75;
	} 
    if (cat == "Asociado"){
    	return 45;
	} 
    if (cat == "Asistente"){
    	return 20;
	} 
    return 0; // Auxiliar
}

void calcularNominaDocente() {
	
    if (!g_profesores.cabeza) { 
	cout << "[X] No hay profesores registrados.\n";
	 return;
    }
    
    cout << "\n";
    cout << left << setw(5)<<"ID" << setw(22)<<"NOMBRE" << setw(14)<<"VINCULACION"
         << setw(14)<<"SALARIO_BASE" << setw(10)<<"SALUD(4%)" << setw(10)<<"PENS(4%)"
         << setw(14)<<"PRESTACIONES" << setw(14)<<"SALARIO_NETO" << "\n";
    imprimirLinea(103);
    Nodo<Profesor>* p = g_profesores.cabeza;
    while (p) {
    	
        if (!p->dato.activo) { p = p->siguiente;
		 continue; 
		 }
		 
		 
        Profesor& pr = p->dato;
        double salario_base = 0.0;
        
        if (pr.tipo_vinculacion == "Planta") {
            int pts_cat = puntosCategoriaEscalafon(pr.categoria_escalafon);
            int pts_total = pr.puntos_titulo + pr.puntos_experiencia + pr.puntos_productividad + pts_cat;
            salario_base = pts_total * pr.valor_punto;
        }else if (pr.tipo_vinculacion == "Ocasional") {
            salario_base = pr.salario_pactado;
        }else if (pr.tipo_vinculacion == "Catedratico") {
            salario_base = pr.horas_catedra * pr.valor_hora;
        }
        double descuento_salud = salario_base * 0.04;
        double descuento_pension = salario_base * 0.04;
        double prestaciones = salario_base * 0.2083; // Cesantias 8.33% + Prima 8.33% + Vacaciones 4.17%
        double neto = salario_base - descuento_salud - descuento_pension;
        cout << left << setw(5) << pr.id
             << setw(22) << (pr.nombre + " " + pr.apellido)
             << setw(14) << pr.tipo_vinculacion
             << setw(14) << fixed << setprecision(0) << salario_base
             << setw(10) << setprecision(0) << descuento_salud
             << setw(10) << setprecision(0) << descuento_pension
             << setw(14) << setprecision(0) << prestaciones
             << setw(14) << setprecision(0) << neto << "\n";
        p = p->siguiente;
    }
    cout << "\n[i] Descuentos de ley: Salud 4%, Pension 4% (empleado).\n";
    cout << "[i] Prestaciones sociales a cargo del empleador (aprox 20.83%).\n";
    cout << "[i] Valores de puntos por categoria representativos del Decreto 1279/2002.\n";
}

void simularCasosNomina() {
    // Inserta datos de prueba automaticamente
    Profesor p1; 
	p1.id = nid_prof++; 
	p1.nombre="Carlos"; 
	p1.apellido="Perez"; 
	p1.documento="123";
    p1.tipo_vinculacion="Planta"; 
	p1.categoria_escalafon="Asociado"; 
	p1.puntos_titulo=120;
    p1.puntos_experiencia=40; 
	p1.puntos_productividad=30; 
	p1.valor_punto=25000;
    p1.horas_catedra=0; 
	p1.valor_hora=0; 
	p1.salario_pactado=0; 
	p1.activo=true;
    g_profesores.agregar(p1);



    Profesor p2; 
	p2.id = nid_prof++; 
	p2.nombre="Ana"; 
	p2.apellido="Lopez"; 
	p2.documento="456";
    p2.tipo_vinculacion="Ocasional"; 
	p2.categoria_escalafon="N/A"; 
	p2.puntos_titulo=0;
    p2.puntos_experiencia=0; 
	p2.puntos_productividad=0; 
	p2.valor_punto=0;
    p2.horas_catedra=0; 
	p2.valor_hora=0; 
	p2.salario_pactado=3500000; 
	p2.activo=true;
    g_profesores.agregar(p2);



    Profesor p3; 
	p3.id = nid_prof++; 
	p3.nombre="Luis"; 
	p3.apellido="Rios"; 
	p3.documento="789";
    p3.tipo_vinculacion="Catedratico"; 
	p3.categoria_escalafon="N/A"; 
	p3.puntos_titulo=0;
    p3.puntos_experiencia=0; 
	p3.puntos_productividad=0; 
	p3.valor_punto=0;
    p3.horas_catedra=12;
	p3.valor_hora=45000; 
	p3.salario_pactado=0; 
	p3.activo=true;
    g_profesores.agregar(p3);

    cout << "[OK] Casos de prueba creados (Planta, Ocasional, Catedratico).\n";
}

void menuNomina() {
    int op;
    do {
        cout<<"\n|======================================================|\n";
    	cout<<"|                    Nomina Docente                    |\n";
    	cout<<"|------------------------------------------------------|\n";
    	cout<<"| 1. Calcular Nominas                                  |\n";
    	cout<<"| 2. Simular casos de Prueba                           |\n";
    	cout<<"| 0. Volver                                            |\n";
    	cout<<"|------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: calcularNominaDocente(); break;
            case 2: simularCasosNomina(); break;
        }
        if (op != 0) pausar();
    } while (op != 0);
}

// ==================== MAIN / MENU PRINCIPAL ====================
int main() {
    cout << "============================================================\n";
    cout << "   PITA - Universidad Popular del Cesar (UPC)\n";
    cout << "   Programa Integrado de Transacciones Academicas\n";
    cout << "   Estructura de Datos - Taller 1\n";
    cout << "============================================================\n\n";

    cout << "Desea cargar datos existentes desde archivos .txt? (s/n): ";
    char opc;
	cin >> opc;
	limpiarBuffer();
    if (opc == 's' || opc == 'S'){
    	cargarTodo();
	} 
    else {
    	cout << "[i] Iniciando sin datos pre-cargados.\n";
	}

    int op;
    do {
       	cout<<"\n|===========================================================|\n";
    	cout<<"|                       MENU PRINCIPAL                      |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 1. Gestion de Facultades                                  |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 2. Gestion de Programas Academicos                        |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 3. Gestion de Cursos                                      |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 4. Gestion de Profesores                                  |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 5. Gestion de Estudiantes                                 |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 6. Gestion de Administrativos                             |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 7. Gestion Academica (Matriculas/ Notas / Promedio / EBRA)|\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 8. Nomina Docente                                         |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 9.  Guardar todos los datos (.txt)                        |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 10. Cargar datos (.txt)                                   |\n";
    	cout<<"|-----------------------------------------------------------|\n";
    	cout<<"| 0. Salir                                                  |\n";
    	cout<<"|-----------------------------------------------------------|\n";
  		cout<<"|Digite una opcion: ";
        cin >> op; limpiarBuffer();
        switch (op) {
            case 1: menuFacultades(); break;
            case 2: menuProgramas(); break;
            case 3: menuCursos(); break;
            case 4: menuProfesores(); break;
            case 5: menuEstudiantes(); break;
            case 6: menuAdministrativos(); break;
            case 7: menuGestionAcademica(); break;
            case 8: menuNomina(); break;
            case 9: guardarTodo(); pausar(); break;
            case 10: cargarTodo(); pausar(); break;
            case 0: cout << "[i] Saliendo... No olvide guardar!\n"; break;
            default: cout << "[X] Opcion invalida.\n"; pausar(); break;
        }
    } while (op != 0);

    return 0;
}