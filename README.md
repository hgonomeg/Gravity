# Grawitacja
Prosty symulator grawitacji miedzy cialami niebieskimi, w tym gwiazdami, planetami i asteroidami.
SFML, C++11.
## Cel projektu
* Cwiczenia programistyczne w zakresie praktycznego wykorzystania programowania obiektowego w jezyku C++, w tym metod wirtualnych, dziedziczenia.
* Cwiczenia korzystania z kontener¢w biblioteki standardowej jezyka C++.
* Uplynnienie poslugiwania sie inteligentnymi wskaznikami (`std::unique_ptr<>`).
* Cwiczenia w zakresie tworzenia prostych interaktywnych aplikacji graficznych w bibliotece SFML.
* Stworzenie prostej zabawki pomagajacej zrozumiec dzialanie mechaniki orbitalnej.
* Zdobycie doswiadczenia w zakresie rysowania tresci graficznych i kontroli okna  w bibliotece SFML
* Blizsze zapoznanie sie z mechanika Git oraz GitHub; cwiczenia w kooperacji wieloosobowej.
## Zadania i mozliwosci programu
* Symulacja grawitacji w ukladzie wielu cial w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych cial niebieskich w czasie dzialania programu posiadajacych predkosc, polozenie i zwrot nadawane w ramach zdarzen wcisniecia i opuszczenia przycisku myszy.
* Scalanie cial niebieskich w razie kolizji.
* Usuwanie istniejacych cial niebieskich.
* Rozr¢znianie miedzy r¢znymi typami spektralnymi gwiazd oraz r¢znymi typami planet
* Kontrola tempa symulacji
* Rozrywkowy tryb polegajacy na kontroli statku kosmicznego
## Opis techniczny
Caly program gleboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura gl¢wna (main) otwiera okno na kt¢rym w kazdym cyklu odswiezenia okna (60FPS) rysuje obiekt Symulatora, wywolujac metode `draw` (sam obiekt dziedzicyz `sf::Drawable`). Rysowanie obiektu sprowadza sie do narysowania calej jego zawartosci (jest to lista obiekt¢w (`std::list<std::unique_ptr<Celestial_body>>`) kt¢re wszystkie dziedzicza z `Celestial_body`, kt¢ra to klasa r¢wniez jest pochodna klasy `sf::Drawable`). Ponadto opr¢cz rysowania, na obiekcie symulatora wywolywana jest metoda `tick`, w ramach kt¢rej prowadzona jest obsluga calej symulacji.
Program bedzie sie obslugiwac za pomoca zar¢wno myszy jak i klawiatury. Okno programu bedzie posiadalo specjalny obiekt przechowujacy i sterujacy stanem interfejsu graficznego (`UI_state`). Wystepujace w oknie zdarzenia sterujace programem sa przekierowywane do specjalnych funkcji obiektu sterujacego. Pewnym klawiszom zostana przypisane funkcje przelaczajace tzw. narzedzie interfejsu graficznego (`UI_tool`). Planowanych jest kilka narzedzi UI, w tym m.in. generator cial niebieskich, selektor i usuwacz.
## Obecny stan implementacji
* Kompletna mechanika klasy `Celestial_body`
* Zarys klas `Planet` oraz `Star`
* Klasa `Asteroid`
* Podstawa mechanizmu interfejs¢w graficznych
	* Wyswietlanie zielonych wiadomosci tekstowych
	* Przelaczanie narzedzi GUI
	* `CB_selector` usuwa ciala niebieskie
* Mechanika pokazywania i odswiezania okna, a takze tworzenia i rysowania obiektu symulatora.
	* Kazde cialo niebieskie jest rysowane osobno
	* Kazde cialo niebieskie ma uprzednio rysowany jego slad orbity,
	  kt¢rego czas zanikania da sie przestawiac
	* Rysowanie slad¢w cial niebieskich da sie wylaczyc
* Pierwotna implementacja metody `tick` w Symulatorze
	* Wykonywanie fukncji obliczajacej sile grawitacji dla kazdej pary
	  obiekt¢w oraz stosowanie jej efekt¢w na predkosciach obiekt¢w
	* Poruszenie kazdym cialem zgodnie z jego predkoscia
	* Przeprowadzanie detekcji kolizji i obslugi kolizji
* Obsluga kolizji w funkcji statycznej w klasie `Celestial_body`
	* Zachowanie momentu pedu
	* Tworzenie nowego obiektu w zaleznosci od mas obiekt¢w kolidujacych ze soba (RTTI na `Celestial_body*`)
	* Scalenie slad¢w zderzajacych sie cial w nowym obiekcie `Celestial_body` i zapisanie ich do specjalnej listy slad¢w odziedziczonych
	* Sposr¢d zderzajacych sie obiekt¢w: usuniecie pierwszego (w funkcji `Simulator::tick()`) i nadpisanie drugiego nowopowstalym.
* Obsluga poprawnego skalowania okna
* Pauzowanie symulacji
* Zmiana widoku w oknie zgodnie poprzez scrollowanie i klawisze strzalek
* Generator tekstur przycisk¢w
## Do zrobienia
* Odpowiednie teksturowanie cial niebieskich
* Pelne wyswietlanie interfejs¢w graficznych
	* Pelna implementacja zaplanowanych przycisk¢w
	* Pelna implementacja zaplanowanych komunikat¢w tekstowych
* Konstruktor kopiujacy `Simulator`
* Przewidywanie slad¢w cial niebieskich
* Stos diagram¢w alokacji dla `Celestial_body`, potrzebny do zapanowania nad alokacja obiekt¢w w trybie przewidywania slad¢w
* Przerobic statyczny diagram alokacji w `Celestial_body` z `std::vector<bool>` na `std::map<unsigned int, unsigned int>`
* Okodowanie `CB_selector`, `CB_gen` i `UI_masterpanel`
	* Zwiekszenie ilosci funkcji i udogodnien podpietych pod wcisniecie danego klawisza
	* Generacja cial niebieskich r¢znych typ¢w
		* Stworzenie `Textbox` do wybierania masy tworzonego ciala
		* Okodowanie serii przycisk¢w do wyboru typu tworzonego ciala
	* Modyfikacja parametr¢w cial niebieskich
	* Stworzenie narzedzia do manipulacji czasem
		* Mechanizm przelaczania dokladnosci symulacji za pomoca zmiennej `Simulator::STEPPINGRATE`
		* Mechanizm sterowania szybkoscia symulacji, poprzez zmiane liczby tick¢w symulatora przypadajacych na jedno odswiezenie okna
* Ladowanie tekstur z plik¢w w funkcji `LoadResources()`
* Rozplanowac i dodac tryb sterowalnego statku kosmicznego (dziedziczacego z klasy `Asteroid`, reprezentujacej cialo o minimalnej masie)
* Funkcje usuwajace istniejace slady cial niebieskich
* System szybkich zapis¢w i odtworzen obecnego stanu symulatora w pamieci
## Jak skompilowac?
Aby skompilowac projekt, nalezy skompilowac wszystkie dostepne w repozytorium pliki .cpp - wszystko ma zostac skladowa pliku wykonywalnego "Grawitacja.exe", do kt¢rego nalezey dolinkowac biblioteke SFML (modul graficzny tej biblioteki)
Wymagany standard C++11 wraz z RTTI.
## Do rozwazenia
* Sledzenie widokiem danego ciala niebieskiego
* Zapis i odczyt z pliku przez modul XML
