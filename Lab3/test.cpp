#include <iostream>
#include <cstring>
using namespace std;

int main() {
    char *a = nullptr; // Inițializăm pointerii cu nullptr
    char *b = nullptr;
    
    char temp[1000]; // Declarăm șirul temporar
    
    // Citirea șirurilor de la utilizator
    cout << "Enter string a: ";
    cin.getline(temp, 1000); // Citim în șirul temporar direct
    a = new char[strlen(temp) + 1]; // Alocăm memorie pentru a, cu dimensiunea necesară
    strcpy(a, temp); // Copiem conținutul din șirul temporar în șirul a
    
    cout << "Enter string b: ";
    cin.getline(temp, 1000);
    b = new char[strlen(temp) + 1]; // Alocăm memorie pentru b, cu dimensiunea necesară
    strcpy(b, temp); // Copiem conținutul din șirul temporar în șirul b
    
    // Compararea șirurilor
    if (strcmp(a, b) != 0) {
        cout << -1;
    }
    else {
        cout << 1;
    }
    
    // Dealocarea memoriei
    delete[] a;
    delete[] b;

    return 0;
}
