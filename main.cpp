#include <iostream>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;

string nazwaPlikuAdresata = "KsiazkaAdresowa.txt";
string nazwaPlikuAdresataTymczasowa = "KsiazkaAdresowa_tymczasowa.txt";
string nazwaPlikuUzytkownikow = "Uzytkownicy.txt";

struct Uzytkownik {
    int id = 0;
    string nazwa = "", haslo = "";
};

struct Adresat {
    int idAdresata = 0, idUzytkownika = 0;
    string imie = "", nazwisko = "", numerTelefonu = "", email = "", adres = "";
};

string konwerjsaIntNaString(int liczba);
string zamienPierwszaLitereNaDuzaAPozostaleNaMale(string tekst);
// Funkcje U¿ytkownika
void zapisUzytkownikaDoPliku(Uzytkownik uzytkownicy);
void zapiszWszystkichUzytkownikowDoPlikuTekstowego(vector <Uzytkownik> &uzytkownicy);
Uzytkownik pobierzDaneUzytkownika(string liniaTekstu);
void wczytywanieUzytkownikaZpliku(vector<Uzytkownik> &uzytkownicy);
void rejestracja(vector<Uzytkownik> &uzytkownicy);
int logowanie(vector<Uzytkownik> uzytkownicy);
void zmianaHasla(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika);
// Funkcje Adresata
int pobieranieIdAdresataZpliku();
void dopiszAdresataDoPliku(Adresat adresat);
void dodajOsobe(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika);
Adresat pobierzDaneAdresata(string liniaTekstu);
void odczytAdresatowZpliku(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika);
void wyszukajAdresatowPoImieniu(vector<Adresat> &adresaci);
void wyszukajAdresatowPoNazwisku(vector<Adresat> &adresaci);
void usuwanieWybranejOsoby(vector<Adresat> &adresaci);
void edycjaWybranejOsoby(vector<Adresat> &adresaci);
void wyswietlanieOsob(vector<Adresat> &adresaci);
void zapisEdytwanychAdresatowDoPliku(vector <Adresat> &adresaci, int idWybranegoAdresata);
void zapisDanychDoPlikuPoUsunieciuAdresata(vector <Adresat> &adresaci, int idWybranegoAdresata);

int main() {
    vector<Uzytkownik> uzytkownicy;
    vector<Adresat> adresaci;
    int idZalogowanegoUzytkownika = 0;
    int iloscUzytkownikow = 0;
    char wybor;

    wczytywanieUzytkownikaZpliku(uzytkownicy);

    while(true) {
        if(idZalogowanegoUzytkownika == 0) {
            system("cls");
            cout << " >>> KSIAZKA ADRESOWA <<<" << endl << endl;
            cout << "ID ZALOGODANEGO UZYTKOWNIKA: " << idZalogowanegoUzytkownika << endl << endl;
            cout << "1. Rejestracja" << endl;
            cout << "2. Logowanie" << endl;
            cout << "9. Zakoncz program" << endl;
            cout << "Wybor: ";
            cin >> wybor;

            switch (wybor) {
            case '1':
                rejestracja(uzytkownicy);
                break;
            case '2':
                idZalogowanegoUzytkownika = logowanie(uzytkownicy);
                break;
            case '9':
                exit(0);
                break;
            }
        } else {
            if(adresaci.size() == 0) {
                odczytAdresatowZpliku(adresaci, idZalogowanegoUzytkownika);
            }
            system("cls");
            cout << " >>> KSIAZKA ADRESOWA <<<" << endl << endl;
            cout << "ID ZALOGOWANEGO UZYTKOWNIKA: " << idZalogowanegoUzytkownika << endl << endl;
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukaj adresata" << endl;
            cout << "3. Wyswietl wszystkich adresatow" << endl;
            cout << "4. Usuwanie wybranego adresata" << endl;
            cout << "5. Edycja danych wybranego adresata" << endl;
            cout << "6. Zmiana hasla" << endl;
            cout << "7. Wylogowanie" << endl;
            cout << "9. Zakoncz program" << endl;
            cout << "Wybor: ";
            cin >> wybor;


            switch (wybor) {
            case '1':
                dodajOsobe(adresaci, idZalogowanegoUzytkownika);
                break;
            case '2':
                cout << "1. Wyszukaj po imieniu" << endl;
                cout << "2. Wyszukaj po nazwisku" << endl;
                cin >> wybor;
                if (wybor == '1') {
                    wyszukajAdresatowPoImieniu(adresaci);
                } else if (wybor == '2') {
                    wyszukajAdresatowPoNazwisku(adresaci);
                }
                break;
            case '3':
                wyswietlanieOsob(adresaci);
                break;
            case '4':
                wyswietlanieOsob(adresaci);
                usuwanieWybranejOsoby(adresaci);
                break;
            case '5':
                wyswietlanieOsob(adresaci);
                edycjaWybranejOsoby(adresaci);
                break;
            case '6':
                zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
                break;
            case '7':
                idZalogowanegoUzytkownika = 0;
                adresaci.clear();
                break;
            case '9':
                exit(0);
                break;
            }
        }
    }
    return 0;
}

string konwerjsaIntNaString(int liczba) {
    ostringstream ss;
    ss << liczba;
    string str = ss.str();

    return str;
}

string zamienPierwszaLitereNaDuzaAPozostaleNaMale(string tekst) {
    if (!tekst.empty()) {
        transform(tekst.begin(), tekst.end(), tekst.begin(), ::tolower);
        tekst[0] = toupper(tekst[0]);
    }
    return tekst;
}

void zapisUzytkownikaDoPliku(Uzytkownik uzytkownicy) {
    fstream plikUzytkownika;
    plikUzytkownika.open(nazwaPlikuUzytkownikow.c_str(), ios::out | ios::app);

    if(plikUzytkownika.good() == true) {
        plikUzytkownika << uzytkownicy.id << "|";
        plikUzytkownika << uzytkownicy.nazwa << "|";
        plikUzytkownika << uzytkownicy.haslo << "|" << endl;
        plikUzytkownika.close();

        cout << endl << "Uzytkownik zostal dodany" << endl;
        system("pause");
    } else {
        cout << "Nie mozna otworzyc pliku i zapisac w nim danych" << endl;
        system("pause");
    }
}

void zapiszWszystkichUzytkownikowDoPlikuTekstowego(vector <Uzytkownik> &uzytkownicy) {
    fstream plikUzytkownikow;
    string liniaTekstu = "";

    plikUzytkownikow.open(nazwaPlikuUzytkownikow.c_str(), ios::out);
    if (plikUzytkownikow.good()) {
        for (vector <Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
            liniaTekstu += konwerjsaIntNaString(itr -> id) + '|';
            liniaTekstu += itr -> nazwa + '|';
            liniaTekstu += itr -> haslo + '|';

            plikUzytkownikow << liniaTekstu << endl;
            liniaTekstu = "";
        }
        plikUzytkownikow.close();
    } else {
        cout << "Nie mozna otworzyc pliku." << endl;
    }
}

Uzytkownik pobierzDaneUzytkownika(string liniaTekstu) {
    Uzytkownik uzytkownik;
    string wyrazPomocniczy = "";
    int numerPojedynczejDanejAdresata = 1;

    for(int i = 0; i < liniaTekstu.size(); i++) {
        if(liniaTekstu[i] != '|') {
            wyrazPomocniczy += liniaTekstu[i];
        } else {
            switch(numerPojedynczejDanejAdresata) {
            case 1:
                uzytkownik.id = atoi(wyrazPomocniczy.c_str());
                break;
            case 2:
                uzytkownik.nazwa = wyrazPomocniczy;
                break;
            case 3:
                uzytkownik.haslo = wyrazPomocniczy;
                break;
            }
            wyrazPomocniczy = "";
            numerPojedynczejDanejAdresata++;
        }
    }
    return uzytkownik;
}

void wczytywanieUzytkownikaZpliku(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik uzytkownik;
    string liniaTekstu;

    fstream plikUzytkownika;
    plikUzytkownika.open(nazwaPlikuUzytkownikow.c_str(), ios::in);

    if(plikUzytkownika.good() == true) {
        while(getline(plikUzytkownika, liniaTekstu)) {
            uzytkownik = pobierzDaneUzytkownika(liniaTekstu);
            uzytkownicy.push_back(uzytkownik);
        }
        plikUzytkownika.close();
    }
}

void rejestracja(vector<Uzytkownik> &uzytkownicy) {
    Uzytkownik uzytkownik;

    system("cls");
    cout << ">>> REJESTROWANIE NOWEGO UZYTKOWNIKA <<<" << endl <<endl;

    if(uzytkownicy.empty() == true) {
        uzytkownik.id = 1;
    } else {
        uzytkownik.id = uzytkownicy.back().id + 1;
    }

    cout << "Podaj nazwe uzytkownika: " << endl;
    cin >> uzytkownik.nazwa;
    for(vector<Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if(itr -> nazwa == uzytkownik.nazwa) {
            cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: ";
            cin >> uzytkownik.nazwa;
            itr = uzytkownicy.begin() - 1;
        }
    }
    cout << "Podaj haslo: ";
    cin >> uzytkownik.haslo;
    uzytkownicy.push_back(uzytkownik);
    zapisUzytkownikaDoPliku(uzytkownik);
    Sleep(1000);
}

int logowanie(vector<Uzytkownik> uzytkownicy) {
    string login, haslo;

    system("cls");
    cout << ">>> LOGOWANIE UZYTKOWNIKA <<<" << endl <<endl;
    cout << "Podaj login: ";
    cin >> login;
    while(1) {
        for(vector<Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
            if(itr -> nazwa == login) {
                for(int proby = 0; proby < 3; proby++) {
                    cout << "Podaj haslo. Pozostalo Ci: " << 3 - proby << " prob ";
                    cin >> haslo;
                    if(itr -> haslo == haslo) {
                        cout << "Zalogowales sie." << endl;
                        Sleep(1500);
                        return itr -> id;
                    }
                }
                cout << endl;
                cout << "Podales 3 razy bledne haslo. Poczekaj 3 sekundy przed kolejna proba" << endl;
                Sleep(3000);
                itr = uzytkownicy.begin() - 1;
            }
        }
        cout << endl;
        cout << "Nie ma uzytkownika z takim loginem. Sproboj jeszcze raz" << endl;
        Sleep(1500);
        return 0;
    }
}

void zmianaHasla(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika) {
    Uzytkownik uzytkownik;
    string haslo;

    system("cls");
    cout << ">>> EDYCJA HASLA UZYTKOWNIKA <<<" << endl << endl;
    cout << "Podaj nowe haslo: ";
    cin >> haslo;

    for(vector<Uzytkownik>::iterator itr = uzytkownicy.begin(); itr != uzytkownicy.end(); itr++) {
        if(itr -> id == idZalogowanegoUzytkownika) {
            itr -> haslo = haslo;
        }
    }
    cout << endl;
    cout << "Haslo zostalo zmienione" << endl;
    Sleep(1500);
    zapiszWszystkichUzytkownikowDoPlikuTekstowego(uzytkownicy);
}

int pobieranieIdAdresataZpliku() {
    fstream plik;
    string liniaZDanymiAdresata, numerId;
    int idAdresata;

    plik.open(nazwaPlikuAdresata.c_str(), ios::in);
    if(plik.good() == true) {
        while(getline(plik, liniaZDanymiAdresata)) {
            int i = 0;
            while(liniaZDanymiAdresata[i] != '|') {
                numerId += liniaZDanymiAdresata[i];
                i++;
            }
            idAdresata = atoi(numerId.c_str());
            numerId = "";
        }
        plik.close();
        return idAdresata + 1;
    }
    return 0;
}

void dopiszAdresataDoPliku(Adresat adresat) {
    fstream plikTekstowy;
    plikTekstowy.open(nazwaPlikuAdresata.c_str(), ios::out | ios::app);

    if (plikTekstowy.good() == true) {
        plikTekstowy << adresat.idAdresata << '|';
        plikTekstowy << adresat.idUzytkownika << '|';
        plikTekstowy << adresat.imie << '|';
        plikTekstowy << adresat.nazwisko << '|';
        plikTekstowy << adresat.numerTelefonu << '|';
        plikTekstowy << adresat.email << '|';
        plikTekstowy << adresat.adres << '|' << endl;
        plikTekstowy.close();

        cout << endl << "Adresat zostal dodany" << endl;
        system("pause");
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych." << endl;
        system("pause");
    }
}

void dodajOsobe(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    Adresat adresat;

    system("cls");
    cout << ">>> DODAWANIE NOWEGO ADRESATA <<<" << endl << endl;

    if(adresaci.empty() == true) {
        adresat.idAdresata = 1;
    } else {
        adresat.idAdresata = pobieranieIdAdresataZpliku();
    }

    adresat.idUzytkownika = idZalogowanegoUzytkownika;

    cout << "Podaj dane nowej osoby: " << endl;
    cout << "Podaj imie: " << endl;
    cin >> adresat.imie;
    adresat.imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(adresat.imie);

    cout << "Podaj nazwisko: " << endl;
    cin >> adresat.nazwisko;
    adresat.nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(adresat.nazwisko);

    cout << "Podaj numer telefonu: " << endl;
    cin.sync();
    getline(cin, adresat.numerTelefonu);

    cout << "Podaj adres emial: " << endl;
    cin >> adresat.email;

    cout << "Podaj adres (podaj w jednej linijce): " << endl;
    cin.sync();
    getline(cin, adresat.adres);

    adresaci.push_back(adresat);
    dopiszAdresataDoPliku(adresat);
}

Adresat pobierzDaneAdresata(string liniaTekstu) {
    Adresat adresat;
    string wyrazPomocniczy = "";
    int numerPojedynczejDanejAdresata = 1;

    for (int i = 0; i < liniaTekstu.size(); i++) {
        if (liniaTekstu[i] != '|') {
            wyrazPomocniczy += liniaTekstu[i];
        } else {
            switch(numerPojedynczejDanejAdresata) {
            case 1:
                adresat.idAdresata = atoi(wyrazPomocniczy.c_str());
                break;
            case 2:
                adresat.idUzytkownika = atoi(wyrazPomocniczy.c_str());
            case 3:
                adresat.imie = wyrazPomocniczy;
                break;
            case 4:
                adresat.nazwisko = wyrazPomocniczy;
                break;
            case 5:
                adresat.numerTelefonu = wyrazPomocniczy;
                break;
            case 6:
                adresat.email = wyrazPomocniczy;
                break;
            case 7:
                adresat.adres = wyrazPomocniczy;
                break;
            }
            wyrazPomocniczy = "";
            numerPojedynczejDanejAdresata++;
        }
    }
    return adresat;
}

void odczytAdresatowZpliku(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika) {
    Adresat adresat;
    string liniaTekstu = "";

    fstream plikTekstowyAdresatow;
    plikTekstowyAdresatow.open(nazwaPlikuAdresata.c_str(), ios::in);

    if (plikTekstowyAdresatow.good() == true) {
        while (getline(plikTekstowyAdresatow, liniaTekstu)) {
            adresat = pobierzDaneAdresata(liniaTekstu);
            if(adresat.idUzytkownika == idZalogowanegoUzytkownika) {
                adresaci.push_back(adresat);
            }
        }
        plikTekstowyAdresatow.close();
    }
}


void wyszukajAdresatowPoImieniu(vector <Adresat> &adresaci) {
    string imiePoszukiwanegoAdresata = "";
    int iloscAdresatow = 0;

    system("cls");
    if (!adresaci.empty()) {
        cout << ">>> WYSZUKIWANIE ADRESATOW O IMIENIU <<<" << endl << endl;

        cout << "Wyszukaj adresatow o imieniu: ";
        cin >> imiePoszukiwanegoAdresata;
        imiePoszukiwanegoAdresata = zamienPierwszaLitereNaDuzaAPozostaleNaMale(imiePoszukiwanegoAdresata);

        for (vector <Adresat>::iterator  itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> imie == imiePoszukiwanegoAdresata) {
                cout << endl;
                cout << "Id:                 " << itr -> idAdresata << endl;
                cout << "Imie:               " << itr -> imie << endl;
                cout << "Nazwisko:           " << itr -> nazwisko << endl;
                cout << "Numer Telefonu:     " << itr -> numerTelefonu << endl;
                cout << "Nr Email:           " << itr -> email << endl;
                cout << "Adres:              " << itr -> adres << endl;
                iloscAdresatow++;
            }
        }
        if (iloscAdresatow == 0) {
            cout << endl << "Nie ma adresatow z tym imieniem w ksiazce adresowej" << endl;
        } else {
            cout << endl << "Ilosc adresatow z imieniem: >>> " << imiePoszukiwanegoAdresata << " <<<";
            cout << " w ksiazce adresowej wynosi: " << iloscAdresatow << endl << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl << endl;
    }
    cout << endl;
    system("pause");
}

void wyszukajAdresatowPoNazwisku(vector <Adresat> &adresaci) {
    string nazwiskoPoszukiwanegoAdresata;
    int iloscAdresatow = 0;

    system("cls");
    if (!adresaci.empty()) {
        cout << ">>> WYSZUKIWANIE ADRESATOW O NAZWISKU <<<" << endl << endl;

        cout << "Wyszukaj adresatow o nazwisku: ";
        cin >> nazwiskoPoszukiwanegoAdresata;
        nazwiskoPoszukiwanegoAdresata = zamienPierwszaLitereNaDuzaAPozostaleNaMale(nazwiskoPoszukiwanegoAdresata);

        for (vector <Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            if (itr -> nazwisko == nazwiskoPoszukiwanegoAdresata) {
                cout << endl;
                cout << "Id:                 " << itr -> idAdresata << endl;
                cout << "Imie:               " << itr -> imie << endl;
                cout << "Nazwisko:           " << itr -> nazwisko << endl;
                cout << "Numer Telefonu:     " << itr -> numerTelefonu << endl;
                cout << "Nr Email:           " << itr -> email << endl;
                cout << "Adres:              " << itr -> adres << endl;
                iloscAdresatow++;
            }
        }
        if (iloscAdresatow == 0) {
            cout << endl << "Nie ma adresatow z tym imieniem w ksiazce adresowej" << endl;
        } else {
            cout << endl << "Ilosc adresatow z nazwiskiem: >>> " << nazwiskoPoszukiwanegoAdresata << " <<<";
            cout << " w ksiazce adresowej wynosi: " << iloscAdresatow << endl << endl;
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl << endl;
    }
    cout << endl;
    system("pause");
}

void usuwanieWybranejOsoby(vector<Adresat> &adresaci) {
    int idUsuwanegoAdresata = 0;
    char znak;
    bool czyIstniejeAdresat = false;

    system("cls");
    if (!adresaci.empty()) {
        cout << ">>> USUWANIE WYBRANEJ OSOBY <<<" << endl << endl;
        cout << "Podaj numer ID adresata ktorego chcesz USUNAC: ";
        cin >> idUsuwanegoAdresata;

        if (cin.fail()) {
            cout << endl << "Wprowadzono niepoprawne dane. Powrot do menu glownego" << endl;
            cin.clear();
            cin.sync();
        } else {
            for (vector <Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
                if (itr -> idAdresata == idUsuwanegoAdresata) {
                    czyIstniejeAdresat = true;
                    cout << endl << endl << "Potwierdz naciskajac klawisz 't': ";
                    cin >> znak;
                    if (znak == 't') {
                        itr = adresaci.erase(itr);
                        cout << endl << endl << "Szukany adresat zostal USUNIETY" << endl << endl;
                        zapisDanychDoPlikuPoUsunieciuAdresata(adresaci, idUsuwanegoAdresata);
                        break;
                    } else {
                        cout << endl << endl << "Wybrany adresat NIE zostal usuniety" << endl << endl;
                        break;
                    }
                }
            }
            if (czyIstniejeAdresat == false) {
                cout << endl << "Nie ma takiego adresata w ksiazce adresowej" << endl << endl;
            }
        }
    } else {
        cout << "Ksiazka adresowa jest pusta" << endl << endl;
    }
    system("pause");
}

void edycjaWybranejOsoby(vector<Adresat> &adresaci) {
    int idWybranegoAdresata = 0;
    char wybor;
    bool czyIstniejeAdresat = false;

    system("cls");
    if (!adresaci.empty()) {
        cout << ">>> EDYCJA WYBRANEGO ADRESATA <<<" << endl << endl;
        cout << "Podaj numer ID adresata u ktorego chcesz zmienic dane: ";
        cin >> idWybranegoAdresata;

        if (cin.fail()) {
            cout << endl << "Wprowadzono niepoprawne dane. Powrot do menu glownego" << endl << endl;
            cin.clear();
            cin.sync();
        } else {
            for (vector <Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
                if (itr -> idAdresata == idWybranegoAdresata) {
                    czyIstniejeAdresat = true;

                    cout << endl << "Ktore dane zaktualizowac: " << endl;
                    cout << "1 - Imie" << endl;
                    cout << "2 - Nazwisko" << endl;
                    cout << "3 - Numer telefonu" << endl;
                    cout << "4 - Email" << endl;
                    cout << "5 - Adres" << endl;
                    cout << "6 - Powrot " << endl;
                    cout << endl << "Wybierz 1-6: ";
                    cin >> wybor;

                    switch (wybor) {
                    case '1':
                        cout << "Podaj nowe imie: ";
                        cin >> itr -> imie;
                        itr -> imie = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> imie);
                        cout << endl << "Imie zostalo zmienione" << endl << endl;
                        zapisEdytwanychAdresatowDoPliku(adresaci, idWybranegoAdresata);
                        break;
                    case '2':
                        cout << "Podaj nowe nazwisko: ";
                        cin >> itr -> nazwisko;
                        itr -> nazwisko = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> nazwisko);
                        cout << endl << "Nazwisko zostalo zmienione" << endl << endl;
                        zapisEdytwanychAdresatowDoPliku(adresaci, idWybranegoAdresata);
                        break;
                    case '3':
                        cout << "Podaj nowy numer telefonu: ";
                        cin.sync();
                        getline(cin, itr -> numerTelefonu);
                        cout << endl << "Numer telefonu zostal zmieniony" << endl << endl;
                        zapisEdytwanychAdresatowDoPliku(adresaci, idWybranegoAdresata);
                        break;
                    case '4':
                        cout << "Podaj nowy email: ";
                        cin.sync();
                        getline(cin, itr -> email);
                        cout << endl << "Email zostal zmieniony" << endl << endl;
                        zapisEdytwanychAdresatowDoPliku(adresaci, idWybranegoAdresata);
                        break;
                    case '5':
                        cout << "Podaj nowy adres zamieszkania: ";
                        cin.sync();
                        getline(cin, itr -> adres);
                        itr -> adres = zamienPierwszaLitereNaDuzaAPozostaleNaMale(itr -> adres);
                        cout << endl << "Adres zostal zmieniony" << endl << endl;
                        zapisEdytwanychAdresatowDoPliku(adresaci, idWybranegoAdresata);
                        break;
                    case '6':
                        cout << endl << "Powrot do menu glownego" << endl << endl;
                        break;
                    default:
                        cout << endl << "Nie ma takiej opcji w menu! Powrot do menu glownego." << endl << endl;
                        break;
                    }
                }
            }
            if (czyIstniejeAdresat == false) {
                cout << endl << "Nie ma takiego adresata w ksiazce adresowej." << endl << endl;
            }
        }
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl << endl;
    }
    system("pause");
}

void wyswietlanieOsob(vector<Adresat> &adresaci) {
    system("cls");
    if (!adresaci.empty()) {
        for (vector <Adresat> :: iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
            cout << "Id:                 " << itr -> idAdresata << endl;
            cout << "Imie:               " << itr -> imie << endl;
            cout << "Nazwisko:           " << itr -> nazwisko << endl;
            cout << "Numer telefonu:     " << itr -> numerTelefonu << endl;
            cout << "Email:              " << itr -> email << endl;
            cout << "Adres:              " << itr -> adres << endl << endl;
        }
        cout << endl;
    } else {
        cout << "Ksiazka adresowa jest pusta." << endl << endl;
    }
    system("pause");
}

void zapisEdytwanychAdresatowDoPliku(vector <Adresat> &adresaci, int idWybranegoAdresata) {
    fstream plik, plikTymczasowy;
    string liniaZDanymiAdresata, numerId;
    int idAdresata;

    plik.open(nazwaPlikuAdresata.c_str(), ios::in);
    plikTymczasowy.open(nazwaPlikuAdresataTymczasowa.c_str(), ios::out);
    if(plik.good() == true && plikTymczasowy.good() == true) {
        while(getline(plik, liniaZDanymiAdresata)) {
            int i = 0;
            while(liniaZDanymiAdresata[i] != '|') {
                numerId += liniaZDanymiAdresata[i];
                i++;
            }
            idAdresata = atoi(numerId.c_str());
            numerId = "";

            if(idAdresata != idWybranegoAdresata) {
                plikTymczasowy << liniaZDanymiAdresata << endl;
            } else {
                for (vector <Adresat>::iterator itr = adresaci.begin(); itr != adresaci.end(); itr++) {
                    if(itr -> idAdresata == idWybranegoAdresata) {
                        liniaZDanymiAdresata = "";
                        liniaZDanymiAdresata += konwerjsaIntNaString(itr -> idAdresata) + '|';
                        liniaZDanymiAdresata += konwerjsaIntNaString(itr -> idUzytkownika) + '|';
                        liniaZDanymiAdresata += itr -> imie + '|';
                        liniaZDanymiAdresata += itr -> nazwisko + '|';
                        liniaZDanymiAdresata += itr -> numerTelefonu + '|';
                        liniaZDanymiAdresata += itr -> email + '|';
                        liniaZDanymiAdresata += itr -> adres + '|';

                        plikTymczasowy << liniaZDanymiAdresata << endl;
                        liniaZDanymiAdresata = "";
                    }
                }
            }
        }
    }
    plik.close();
    plikTymczasowy.close();
    remove(nazwaPlikuAdresata.c_str());
    rename(nazwaPlikuAdresataTymczasowa.c_str(), nazwaPlikuAdresata.c_str());
}

void zapisDanychDoPlikuPoUsunieciuAdresata(vector <Adresat> &adresaci, int idWybranegoAdresata) {
    fstream plik, plikTymczasowy;
    string liniaZDanymiAdresata, numerId;
    int idAdresata;

    plik.open(nazwaPlikuAdresata.c_str(), ios::in);
    plikTymczasowy.open(nazwaPlikuAdresataTymczasowa.c_str(), ios::out);
    if(plik.good() == true && plikTymczasowy.good() == true) {
        while(getline(plik, liniaZDanymiAdresata)) {
            int i = 0;
            while(liniaZDanymiAdresata[i] != '|') {
                numerId += liniaZDanymiAdresata[i];
                i++;
            }
            idAdresata = atoi(numerId.c_str());
            numerId = "";

            if(idAdresata != idWybranegoAdresata) {
                plikTymczasowy << liniaZDanymiAdresata << endl;
            }
        }
    }
    plik.close();
    plikTymczasowy.close();
    remove(nazwaPlikuAdresata.c_str());
    rename(nazwaPlikuAdresataTymczasowa.c_str(), nazwaPlikuAdresata.c_str());
}

