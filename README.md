Goals and capabilities of the p# Gravity
Simplistic, yet fully featured, simulator of gravity between celestial bodies, including stars, planets and asteroids.
SFML, C++11.

## Purpose of the project
* Gaining experience in practial usage of object-oriented programming in C++, including virtual methods and inheritance.
* Excercise in using C++'s `std::` containers.
* Fluency in working with smart pointers (`std::unique_ptr<>`).
* Learning how to create simple & interactive GUI applications in SFML.
* Creating a simple toy which helps to understand how orbital mechanics works.
* Gaining experience in drawing graphics and managing windows in the SFML library.
* Getting familiar with Git and GitHub; excercise cooperation.
* Learning how to optimize
* Excercise C++ generic programming
* Optimization via efficient multithreaded programming
## Goals and capabilities of the program
* Symulacja grawitacji w układzie wielu ciał w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych ciał niebieskich w czasie działania programu posiadających prędkość, położenie i zwrot nadawane w ramach zdarzeń wciśnięcia i opuszczenia przycisku myszy.
* Scalanie ciał niebieskich w razie kolizji.
* Usuwanie istniejących ciał niebieskich.
* Rozróżnianie miedzy róznymi typami spektralnymi gwiazd oraz różnymi typami planet
* Kontrola tempa i dokładności symulacji
* Rozrywkowy tryb polegający na kontroli statku kosmicznego
## Implementation notes
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura główna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę `draw` (sam obiekt dziedziczy z `sf::Drawable`). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (`std::list<std::unique_ptr<Celestial_body>>`) które wszystkie dziedziczą z `Celestial_body`, która to klasa również jest pochodną klasy `sf::Drawable`). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda `tick`, w ramach której prowadzona jest obsługa całej symulacji.
Program będzie się obsługiwać za pomocą zarówno myszy jak i klawiatury. Okno programu będzie posiadało specjalny obiekt przechowujący i sterujący stanem interfejsu graficznego (`UI_state`). Występujące w oknie zdarzenia sterujące programem są przekierowywane do specjalnych funkcji obiektu sterującego. Pewnym klawiszom zostaną przypisane funkcje przełączajace tzw. narzędzie interfejsu graficznego (`UI_tool`). Planowanych jest kilka narzedzi UI, w tym m.in. generator ciał niebieskich, selektor i usuwacz.
## Current completion status
* Kompletna mechanika klasy `Celestial_body`
* Zarys klas `Planet` oraz `Star`
* Klasa `Asteroid`
* Stabilny mechanizm interfejsów graficznych
	* Wyświetlanie zielonych wiadomości tekstowych
	* Przełączanie narzędzi GUI
	* `CB_selector` zaznacza i usuwa ciała niebieskie
	* `CB_gen` dodaje ciała niebieskie i przełącza między różnymi ich typami
* Mechanika pokazywania i odświeżania okna, a także tworzenia i rysowania obiektu symulatora.
	* Każde ciało niebieskie jest rysowane osobno
	* Każde cialo niebieskie ma uprzednio rysowany jego ślad orbity, którego czas zanikania da się przestawiać
	* Rysowanie sladów ciał niebieskich da się wyłączyć
	* Ślady ciał niebieskich da się usunąć
* Pierwotna implementacja metody `tick` w Symulatorze
	* Wykonywanie funkcji obliczającej siłę grawitacji dla każdej pary obiektów oraz stosowanie jej efektów na prędkościach obiektów
	* Poruszanie każdym ciałem zgodnie z jego prędkością
	* Przeprowadzanie detekcji i obsługi kolizji
* Obsługa kolizji w funkcji statycznej w klasie `Celestial_body`
	* Zachowanie momentu pędu
	* Tworzenie nowego obiektu w zależności od mas obiektów kolidujących ze sobą (RTTI na `Celestial_body*`)
	* Scalanie sladów zderzających się ciał w nowym obiekcie `Celestial_body` i zapisanie ich do specjalnej listy śladów odziedziczonych
	* Spośród zderzających się obiektów: usunięcie pierwszego (w funkcji `Simulator::tick()`) i nadpisanie drugiego nowopowstałym.
* Obsługa poprawnego skalowania okna
* Pauzowanie symulacji
* Obsługa gwiazd stałych
* Zmiana widoku w oknie poprzez scrollowanie i klawisze strzałek
* Generator tekstur przycisków
* Zmiana tempa i jakości symulacji
	* Manipulowanie intensywnością obliczeniową symulacji (liczba ticków symulatora / odświeżenie okna)
	* Manipulowanie dokładnością ticków symulacji
## To do
* Odpowiednie teksturowanie ciał niebieskich
* Przewidywanie sladów ciał niebieskich
* Dalszy rozwój `CB_selector`, `CB_gen` i `UI_masterpanel`
	* Zwiększenie ilości funkcji i udogodnień podpiętych pod wciśnięcie danego klawisza
	* Stworzenie `Textbox` do wybierania masy tworzonego ciała
	* Okodowanie serii przycisków do wyboru typu tworzonego ciała
	* Modyfikacja parametrów ciał niebieskich
	* Więcej danych w trybie debug
* Ładowanie tekstur z plików w funkcji `LoadResources()`
* Rozplanować i dodać tryb sterowalnego statku kosmicznego 
* Śledzenie widokiem danego ciała niebieskiego
* Zapis i odczyt z pliku przez moduł XML
* System szybkich zapisów i odtworzeń obecnego stanu symulatora w pamięci
* Zmienić algorytm ewaluacji indywidualnych par ciał niebieskich na wielowątkowy TIANCHE (przykład działania w `pairalgo/tianche. cpp` i `wuxing.cpp`)
* Optymalizacja renderowania
## Wuxing
In the `pairalgo` folder, there is a little animation engine called Wuxing. It visualizes how individual-unique-pair iteration algorithms work. It was written to serve as a helper in designing efficient algorithms for calculating the force of gravity between every unique pair of celestial bodies.

It helped in developing the Tianche algorithm.

## Building instructions
Aby skompilować projekt, należy skompilować wszystkie dostępne w folderze głównym repozytorium pliki .cpp - wszystko ma zostać składową pliku wykonywalnego "Grawitacja.exe", do którego należy dolinkować bibliotekę SFML (moduł graficzny tej biblioteki)
Wymagany standard C++17 wraz z RTTI.
## To consider

