#include <iostream>
#include <cstring>
#include <fstream>
sadsad
const int MAX_PROCESOS = 100;
const int MAX_NOMBRE   = 40;
const int MAX_MEMORIA  = 100; // bloques de memoria simulada

struct Proceso {
    int id;
    char nombre[MAX_NOMBRE];
    int prioridad;
    int memoria;
    bool activo;
};

struct GestorProcesos {
    Proceso procesos[MAX_PROCESOS];
    int numProcesos = 0;
    int siguienteID = 1;
    int memoriaLibre = MAX_MEMORIA;

    // Función auxiliar para encontrar el índice de un proceso por ID
    int buscarIndice(int id) {
        for (int i = 0; i < numProcesos; ++i)
            if (procesos[i].activo && procesos[i].id == id) return i;
        return -1;
    }

    void agregarProceso() {
        if (numProcesos >= MAX_PROCESOS) {
            std::cout << "Límite de procesos alcanzado.\n";
            return;
        }
        Proceso p;
        std::cout << "Nombre del proceso: ";
        std::cin.ignore();
        std::cin.getline(p.nombre, MAX_NOMBRE);

        std::cout << "Prioridad (mayor numero = mayor prioridad): ";
        std::cin >> p.prioridad;

        std::cout << "Memoria necesaria (en bloques): ";
        std::cin >> p.memoria;

        if (p.memoria > memoriaLibre) {
            std::cout << "No hay suficiente memoria disponible.\n";
            return;
        }

        p.id = siguienteID++;
        p.activo = true;
        procesos[numProcesos++] = p;
        memoriaLibre -= p.memoria;
        std::cout << "Proceso agregado exitosamente con ID " << p.id << ".\n";
    }

    void eliminarProceso() {
        int id;
        std::cout << "ID del proceso a eliminar: ";
        std::cin >> id;
        int idx = buscarIndice(id);
        if (idx == -1) {
            std::cout << "No se encontro el proceso.\n";
            return;
        }
        memoriaLibre += procesos[idx].memoria;
        procesos[idx].activo = false;
        std::cout << "Proceso eliminado y memoria liberada.\n";
    }

    void buscarProceso() {
        int id;
        std::cout << "ID del proceso a buscar: ";
        std::cin >> id;
        int idx = buscarIndice(id);
        if (idx == -1) {
            std::cout << "No se encontro el proceso.\n";
            return;
        }
        Proceso& p = procesos[idx];
        std::cout << "ID: " << p.id << " | Nombre: " << p.nombre
                  << " | Prioridad: " << p.prioridad << " | Memoria: " << p.memoria << "\n";
    }

    void modificarProceso() {
        int id;
        std::cout << "ID del proceso a modificar: ";
        std::cin >> id;
        int idx = buscarIndice(id);
        if (idx == -1) {
            std::cout << "No se encontro el proceso.\n";
            return;
        }
        Proceso & p = procesos[idx];
        std::cout << "Nuevo nombre (actual: " << p.nombre << "): ";
        std::cin.ignore();
        std::cin.getline(p.nombre, MAX_NOMBRE);

        std::cout << "Nueva prioridad (actual: " << p.prioridad << "): ";
        std::cin >> p.prioridad;

        std::cout << "Nueva memoria (actual: " << p.memoria << "): ";
        int nuevaMemoria;
        std::cin >> nuevaMemoria;

        int diferencia = nuevaMemoria - p.memoria;
        if (diferencia > memoriaLibre) {
            std::cout << "No hay suficiente memoria disponible para aumentar.\n";
            return;
        }
        memoriaLibre -= diferencia;
        p.memoria = nuevaMemoria;

        std::cout << "Proceso modificado exitosamente.\n";
    }

    // Muestra todos los procesos ordenados por prioridad (mayor primero)
    void listarProcesos() {

        int indices[MAX_PROCESOS];
        int n = 0;
        for (int i = 0; i < numProcesos; ++i)
            if (procesos[i].activo) indices[n++] = i;

        // Ordenar por prioridad (burbuja porque hay pocos elementos)
        for (int i = 0; i < n - 1; ++i)
            for (int j = 0; j < n - i - 1; ++j)
                if (procesos[indices[j]].prioridad < procesos[indices[j + 1]].prioridad) {
                    int temp = indices[j];
                    indices[j] = indices[j + 1];
                    indices[j + 1] = temp;
                }

        std::cout << "\nLista de procesos (por prioridad):\n";
        for (int k = 0; k < n; ++k) {
            Proceso& p = procesos[indices[k]];
            std::cout << "ID: " << p.id << " | Nombre: " << p.nombre
                      << " | Prioridad: " << p.prioridad << " | Memoria: " << p.memoria << "\n";
        }
        std::cout << "Bloques de memoria disponibles: " << memoriaLibre << "\n";
    }

    // Guardar en archivo binario
    void guardarEnArchivo(const char* filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cout << "Error al abrir el archivo para guardar.\n";
            return;
        }
        file.write((char*)&numProcesos, sizeof(numProcesos));
        file.write((char*)&siguienteID, sizeof(siguienteID));
        file.write((char*)&memoriaLibre, sizeof(memoriaLibre));
        for (int i = 0; i < numProcesos; ++i)
            file.write((char*)&procesos[i], sizeof(Proceso));
        std::cout << "Datos guardados exitosamente.\n";
        file.close();
    }

    // Cargar del archivo binario
    void cargarDeArchivo(const char* filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cout << "No hay datos guardados para cargar.\n";
            return;
        }
        file.read((char*)&numProcesos, sizeof(numProcesos));
        file.read((char*)&siguienteID, sizeof(siguienteID));
        file.read((char*)&memoriaLibre, sizeof(memoriaLibre));
        for (int i = 0; i < numProcesos; ++i)
            file.read((char*)&procesos[i], sizeof(Proceso));
        std::cout << "Datos cargados exitosamente.\n";
        file.close();
    }
};

int main() {
    GestorProcesos gestor;
    gestor.cargarDeArchivo("procesos.dat");

    int opcion;
    do {
        std::cout << "\n--- GestorPro 1.0 ---\n";
        std::cout << "1. Agregar Proceso\n2. Eliminar Proceso\n3. Buscar Proceso\n";
        std::cout << "4. Modificar Proceso\n5. Listar Procesos\n6. Guardar y Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1: gestor.agregarProceso(); break;
            case 2: gestor.eliminarProceso(); break;
            case 3: gestor.buscarProceso(); break;
            case 4: gestor.modificarProceso(); break;
            case 5: gestor.listarProcesos(); break;
            case 6: 
                gestor.guardarEnArchivo("procesos.dat");
                std::cout << "Saliendo...\n";
                break;
            default: std::cout << "Opción inválida.\n"; break;
        }
    } while (opcion != 6);

    return 0;
}
