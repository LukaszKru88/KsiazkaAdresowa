#include <iostream>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

string zamianaIntNaString(int liczba);
void dodajOsobe(vector<string> &osoby);
void odczytZpliku(vector<string> &osoby);
void zapisDoPliku(vector<string> osoby);
void wyszukiwaniePoImieniu(vector<string> osoby);
void wyszukiwaniePoNazwisku(vector<string> osoby);
void usuwanieWybranejOsoby(vector<string> &osoby);
void edycjaWybranejOsoby(vector<string> &osoby);
void wyswietlanieOsob(vector<string> osoby);

int main() {
    vector<string> osoby;
    char wybor;

    odczytZpliku(osoby);

    while(true) {
        system("cls");
        cout << "1. Dodaj osobe" << endl;
        cout << "2. Wyszukaj osoby" << endl;
        cout << "3. Wyswietl wszystkie osoby" << endl;
        cout << "4. Usuwanie wybranej osoby" << endl;
        cout << "5. Edycja danych wybranej osoby" << endl;
        cout << "9. Zakoncz program" << endl;
        cin >> wybor;

        switch (wybor) {
        case '1':
            dodajOsobe(osoby);
            break;
        case '2':
            cout << "1. Wyszukaj po imieniu" << endl;
            cout << "2. Wyszukaj po nazwisku" << endl;
            cin >> wybor;
            if (wybor == '1') {
                wyszukiwaniePoImieniu(osoby);
            }
            else if (wybor == '2') {
                wyszukiwaniePoNazwisku(osoby);
            }
            break;
        case '3':
            wyswietlanieOsob(osoby);
            system("pause");
            break;
        case '4':
            wyswietlanieOsob(osoby);
            usuwanieWybranejOsoby(osoby);
            break;
        case '5':
            wyswietlanieOsob(osoby);
            edycjaWybranejOsoby(osoby);
            break;
        case '9':
            exit(0);
            break;
        }
    }
    return 0;
}

string zamianaIntNaString(int liczba) {
    ostringstream ss;
    ss << liczba;
    return ss.str();
}

void dodajOsobe(vector<string> &osoby) {
    int osobaId;
    string numerId, imie, nazwisko, numerTelefonu, email, adres;

    //przypisanie ID osobie
    vector<string>::iterator it = osoby.begin();
    for(it; it != osoby.end(); it +=6) {
        numerId = *it;
    }
    osobaId = atoi(numerId.c_str()) + 1;
    numerId = zamianaIntNaString(osobaId);
    osoby.push_back(numerId);

    //pobieranie danych nowej osoby
    system("cls");
    cout << "Podaj dane nowej osoby: " << endl;
    cout << "Podaj imie: " << endl;
    cin >> imie;
    transform(imie.begin(),imie.end(),imie.begin(),::toupper);
    osoby.push_back(imie);
    cout << "Podaj nazwisko: " << endl;
    cin >> nazwisko;
    transform(nazwisko.begin(),nazwisko.end(),nazwisko.begin(),::toupper);
    osoby.push_back(nazwisko);
    cout << "Podaj numer telefonu: " << endl;
    cin.sync();
    getline(cin, numerTelefonu);
    osoby.push_back(numerTelefonu);
    cout << "Podaj adres emial: " << endl;
    cin >> email;
    osoby.push_back(email);
    cout << "Podaj adres (podaj w jednej linijce): " << endl;
    cin.sync();
    getline(cin, adres);
    transform(adres.begin(),adres.end(),adres.begin(),::toupper);
    osoby.push_back(adres);

    zapisDoPliku(osoby);
}

void odczytZpliku(vector<string> &osoby) {
    fstream plik;
    string wyrazPomocniczy, linia;

    plik.open("KsiazkaAdresowa.txt", ios::in);
    if(plik.good()) {
        while(getline(plik, linia)) {
            int j = 0;
            for(int i = 0; i <= linia.size(); i++) {
                if(linia[i] == '|') {
                    for(j; j < i; j++) {
                        wyrazPomocniczy += linia[j];
                    }
                    j++;
                    osoby.push_back(wyrazPomocniczy);
                    wyrazPomocniczy = "";
                }
            }
        }
        plik.close();
        cout << "Baza osob zostala wczytana." << endl;
        Sleep(1000);
    } else {
        cout << "Nie mozna otworzyc pliku: KsiazkaAdresowa.txt lub plik nie istnieje" << endl;
        Sleep(1000);
    }
}
void zapisDoPliku(vector<string> osoby) {
    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::out);
    if(plik.good()) {
        vector<string>::iterator it = osoby.begin();
        for(it; it != osoby.end(); it += 6) {
            plik << *it << "|" << *(it + 1) << "|" << *(it + 2) << "|" << *(it + 3) << "|" << *(it + 4) << "|" << *(it + 5) << "|" << endl;
        }
        plik.close();
    } else {
        cout << "Nie mozna otworzyc pliku: KsiazkaAdresowa.txt" << endl;
    }
}

void wyszukiwaniePoImieniu(vector<string> osoby) {
    string wyszukiwanyWyraz;
    cout << "Podaj wyszukiwane imie" << endl;
    cin >> wyszukiwanyWyraz;
    transform(wyszukiwanyWyraz.begin(),wyszukiwanyWyraz.end(),wyszukiwanyWyraz.begin(),::toupper);
    for (int i = 1; i < osoby.size(); i += 6) {
        if (osoby[i].find(wyszukiwanyWyraz) == false) {
            cout << osoby[i - 1] << endl;
            cout << osoby[i] << endl;
            cout << osoby[i + 1] << endl;
            cout << osoby[i + 2] << endl;
            cout << osoby[i + 3] << endl;
            cout << osoby[i + 4] << endl;
            cout << endl;
        }
    }
    system("pause");
}

void wyszukiwaniePoNazwisku(vector<string> osoby) {
    string wyszukiwanyWyraz;
    cout << "Podaj wyszukiwane nazwisko" << endl;
    cin >> wyszukiwanyWyraz;
    transform(wyszukiwanyWyraz.begin(),wyszukiwanyWyraz.end(),wyszukiwanyWyraz.begin(),::toupper);
    for (int i = 2; i < osoby.size(); i += 6) {
        if (osoby[i].find(wyszukiwanyWyraz) == false) {
            cout << osoby[i - 2] << endl;
            cout << osoby[i - 1] << endl;
            cout << osoby[i] << endl;
            cout << osoby[i + 1] << endl;
            cout << osoby[i + 2] << endl;
            cout << osoby[i + 3] << endl;
            cout << endl;
        }
    }
    system("pause");
}

void usuwanieWybranejOsoby(vector<string> &osoby) {
    string znak;
    cout << "Podaj ID osoby do usuniecia: " << endl;
    cin >> znak;

    for(int k = 0; k < osoby.size(); k += 6) {
        if(znak == osoby[k]) {
            osoby.erase(osoby.begin() + k, osoby.begin() + (k + 6));
        }
    }
    cout << "Osoba z ID: " << znak << " zostala usunieta" << endl;
    zapisDoPliku(osoby);
    Sleep(2000);
}

void edycjaWybranejOsoby(vector<string> &osoby) {
    int wybor;
    string znak;
    string nowaDana;
    cout << "Podaj ID osoby do edycji: " << endl;
    cin >> znak;

    for(int k = 0; k < osoby.size(); k += 6) {
        if(znak == osoby[k]) {
            cout << "Podaj co chcesz edytowac: " << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Numer telefonu" << endl;
            cout << "4. Email" << endl;
            cout << "5. Adres" << endl;

            cin >> wybor;
            cout << "Podaj nowa tresc: ";
            switch (wybor) {
            case 1:
                cin >> nowaDana;
                transform(nowaDana.begin(),nowaDana.end(),nowaDana.begin(),::toupper);
                osoby.erase(osoby.begin() + (k + wybor));
                osoby.insert(osoby.begin() + (k + wybor), nowaDana);
                break;
            case 2:
                cin >> nowaDana;
                transform(nowaDana.begin(),nowaDana.end(),nowaDana.begin(),::toupper);
                osoby.erase(osoby.begin() + (k + wybor));
                osoby.insert(osoby.begin() + (k + wybor), nowaDana);
                break;
            case 3:
                cin.sync();
                getline(cin, nowaDana);
                osoby.erase(osoby.begin() + (k + wybor));
                osoby.insert(osoby.begin() + (k + wybor), nowaDana);
                break;
            case 4:
                cin >> nowaDana;
                osoby.erase(osoby.begin() + (k + wybor));
                osoby.insert(osoby.begin() + (k + wybor), nowaDana);
                break;
            case 5:
                cin.sync();
                getline(cin, nowaDana);
                transform(nowaDana.begin(),nowaDana.end(),nowaDana.begin(),::toupper);
                osoby.erase(osoby.begin() + (k + wybor));
                osoby.insert(osoby.begin() + (k + wybor), nowaDana);
                break;
            }
        }
    }
    cout << "Osoba z ID: " << znak << " zostala edytowana" << endl;
    zapisDoPliku(osoby);
    Sleep(2000);
}

void wyswietlanieOsob(vector<string> osoby) {
    for(int i = 0; i < osoby.size(); i += 6) {
        cout << "Numer ID: " << osoby[i] << endl;
        cout << "Imie: " << osoby[i + 1] << endl;
        cout << "Nazwisko: " << osoby[i + 2] << endl;
        cout << "Numer telefonu: " << osoby[i + 3] << endl;
        cout << "Email: " << osoby[i + 4] << endl;
        cout << "Adres: " << osoby[i + 5] << endl;
        cout << endl;
    }
}





