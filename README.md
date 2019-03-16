# Grawitacja
Prosty symulator grawitacji między ciałami niebieskimi, w tym gwiazdami, planetami i asteroidami.
SFML, C++11.
## Cel projektu
* Ćwiczenia programistyczne w zakresie praktycznego wykorzystania programowania obiektowego w języku C++, w tym metod wirtualnych, dziedziczenia.
* Ćwiczenia korzystania z kontenerów biblioteki standardowej jezyka C++.
* Upłynnienie posługiwania się inteligentnymi wskaźnikami (`std::unique_ptr<>`).
* Ćwiczenia w zakresie tworzenia prostych interaktywnych aplikacji graficznych w bibliotece SFML.
* Stworzenie prostej zabawki pomagającej zrozumieć działanie mechaniki orbitalnej.
* Zdobycie doświadczenia w zakresie rysowania treści graficznych i kontroli okna w bibliotece SFML
* Bliższe zapoznanie się z mechaniką Git oraz GitHub; ćwiczenia w kooperacji wieloosobowej.
## Zadania i możliwosci programu
* Symulacja grawitacji w układzie wielu ciał w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych ciał niebieskich w czasie działania programu posiadających prędkość, położenie i zwrot nadawane w ramach zdarzeń wciśnięcia i opuszczenia przycisku myszy.
* Scalanie ciał niebieskich w razie kolizji.
* Usuwanie istniejących ciał niebieskich.
* Rozróżnianie miedzy róznymi typami spektralnymi gwiazd oraz róznymi typami planet
* Kontrola tempa symulacji
* Rozrywkowy tryb polegający na kontroli statku kosmicznego
## Opis techniczny
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura glówna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę `draw` (sam obiekt dziedziczy z `sf::Drawable`). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (`std::list<std::unique_ptr<Celestial_body>>`) które wszystkie dziedziczą z `Celestial_body`, która to klasa również jest pochodną klasy `sf::Drawable`). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda `tick`, w ramach której prowadzona jest obsługa całej symulacji.
Program będzie się obsługiwać za pomocą zarówno myszy jak i klawiatury. Okno programu będzie posiadało specjalny obiekt przechowujący i sterujący stanem interfejsu graficznego (`UI_state`). Występujace w oknie zdarzenia sterujące programem są przekierowywane do specjalnych funkcji obiektu sterującego. Pewnym klawiszom zostaną przypisane funkcje przełączajace tzw. narzędzie interfejsu graficznego (`UI_tool`). Planowanych jest kilka narzedzi UI, w tym m.in. generator ciał niebieskich, selektor i usuwacz.
## Obecny stan implementacji
* Kompletna mechanika klasy `Celestial_body`
* Zarys klas `Planet` oraz `Star`
* Klasa `Asteroid`
* Podstawa mechanizmu interfejsów graficznych
	* Wyświetlanie zielonych wiadomości tekstowych
	* Przełączanie narzedzi GUI
	* `CB_selector` usuwa ciała niebieskie
* Mechanika pokazywania i odświeżania okna, a także tworzenia i rysowania obiektu symulatora.
	* Każde ciało niebieskie jest rysowane osobno
	* Każde cialo niebieskie ma uprzednio rysowany jego ślad orbity, którego czas zanikania da się przestawiać
	* Rysowanie slad˘w cial niebieskich da sie wylaczyc
* Pierwotna implementacja metody `tick` w Symulatorze
	* Wykonywanie funkcji obliczającej siłę grawitacji dla każdej pary obiektów oraz stosowanie jej efektów na prędkościach obiektów
	* Poruszanie każdym ciałem zgodnie z jego prędkoscia
	* Przeprowadzanie detekcji i obsługi kolizji
* Obsługa kolizji w funkcji statycznej w klasie `Celestial_body`
	* Zachowanie momentu pędu
	* Tworzenie nowego obiektu w zależności od mas obiektów kolidujących ze sobą (RTTI na `Celestial_body*`)
	* Scalenie sladów zderzających się ciał w nowym obiekcie `Celestial_body` i zapisanie ich do specjalnej listy śladów odziedziczonych
	* Spośród zderzających się obiektów: usunięcie pierwszego (w funkcji `Simulator::tick()`) i nadpisanie drugiego nowopowstałym.
* Obsługa poprawnego skalowania okna
* Pauzowanie symulacji
* Zmiana widoku w oknie poprzez scrollowanie i klawisze strzałek
* Generator tekstur przycisków
## Do zrobienia
* Odpowiednie teksturowanie ciał niebieskich
* Pełne wyświetlanie interfejsów graficznych
	* Pełna implementacja zaplanowanych przycisków
	* Pełna implementacja zaplanowanych komunikatów tekstowych
* Konstruktor kopiujący `Simulator`
* Przewidywanie sladów ciał niebieskich
* Stos diagramów alokacji dla `Celestial_body`, potrzebny do zapanowania nad alokacją obiektów w trybie przewidywania sladów
* Przerobić statyczny diagram alokacji w `Celestial_body` z `std::vector<bool>` na `std::map<unsigned int, unsigned int>`
* Okodowanie `CB_selector`, `CB_gen` i `UI_masterpanel`
	* Zwiększenie ilości funkcji i udogodnień podpiętych pod wciśnięcie danego klawisza
	* Generacja ciał niebieskich różnych typów
		* Stworzenie `Textbox` do wybierania masy tworzonego ciała
		* Okodowanie serii przycisków do wyboru typu tworzonego ciala
	* Modyfikacja parametrów ciał niebieskich
	* Stworzenie narzędzia do manipulacji czasem
		* Mechanizm przełączania dokładności symulacji za pomocą zmiennej `Simulator::STEPPINGRATE`
		* Mechanizm sterowania szybkością symulacji, poprzez zmianę liczby ticków symulatora przypadających na jedno odświeżenie okna
* Ładowanie tekstur z plików w funkcji `LoadResources()`
* Rozplanować i dodać tryb sterowalnego statku kosmicznego 
* Funkcje usuwające istniejące ślady ciał niebieskich
* System szybkich zapisów i odtworzeń obecnego stanu symulatora w pamięci
## Jak skompilować?
Aby skompilować projekt, należy skompilować wszystkie dostępne w repozytorium pliki .cpp - wszystko ma zostać składową pliku wykonywalnego "Grawitacja.exe", do którego należy dolinkować bibliotekę SFML (moduł graficzny tej biblioteki)
Wymagany standard C++11 wraz z RTTI.
## Do rozważenia
* Śledzenie widokiem danego ciała niebieskiego
* Zapis i odczyt z pliku przez modul XML
