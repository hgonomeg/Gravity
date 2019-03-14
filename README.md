# Grawitacja
Prosty symulator grawitacji między ciałami niebieskimi, w tym gwiazdami, planetami i asteroidami.
SFML, C++11.
## Cel projektu
* Ćwiczenia programistyczne w zakresie praktycznego wykorzystania programowania obiektowego w języku C++, w tym metod wirtualnych, dziedziczenia.
* Ćwiczenia korzystania z kontenerów biblioteki standardowej języka C++.
* Upłynnienie posługiwania się inteligentnymi wskaźnikami (`std::unique_ptr<>`).
* Ćwiczenia w zakresie tworzenia prostych interaktywnych aplikacji graficznych w bibliotece SFML.
* Stworzenie prostej zabawki pomagającej zrozumieć działanie mechaniki orbitalnej.
* Zdobycie doświadczenia w zakresie rysowania treści graficznych i kontroli okna  w bibliotece SFML
* Bliższe zapoznanie się z mechaniką Git oraz GitHub; ćwiczenia w kooperacji wieloosobowej.
## Zadania i możliwości programu
* Symulacja grawitacji w układzie wielu ciał w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych ciał niebieskich w czasie działania programu posiadających prędkość, położenie i zwrot nadawane w ramach zdarzeń wciśnięcia i opuszczenia przycisku myszy.
* Scalanie ciał niebieskich w razie kolizji.
* Usuwanie istniejących ciał niebieskich.
* Rozróżnianie między różnymi typami spektralnymi gwiazd oraz różnymi typami planet
* Kontrola tempa symulacji
* Rozrywkowy tryb polegający na kontroli statku kosmicznego
## Opis techniczny
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura główna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę `draw` (sam obiekt dziedzicyz `sf::Drawable`). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (`std::list<std::unique_ptr<Celestial_body>>`) które wszystkie dziedziczą z `Celestial_body`, która to klasa również jest pochodną klasy `sf::Drawable`). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda `tick`, w ramach której prowadzona jest obsługa całej symulacji.
Program będzie się obsługiwać za pomocą zarówno myszy jak i klawiatury. Okno programu będzie posiadało specjalny obiekt przechowujący i sterujący stanem interfejsu graficznego (`UI_state`). Występujące w oknie zdarzenia sterujące programem są przekierowywane do specjalnych funkcji obiektu sterującego. Pewnym klawiszom zostaną przypisane funkcje przełączające tzw. narzędzie interfejsu graficznego (`UI_tool`). Planowanych jest kilka narzędzi UI, w tym m.in. generator ciał niebieskich, selektor i usuwacz.
## Obecny stan implementacji
* Kompletna mechanika klasy `Celestial_body`
* Zarys klas `Planet` oraz `Star`
* Podstawa mechanizmu interfejsów graficznych
	* Wyświetlanie zielonych wiadomości tekstowych
	* Przełączanie narzędzi GUI
	* `CB_selector` usuwa ciała niebieskie
* Mechanika pokazywania i odświeżania okna, a także tworzenia i rysowania obiektu symulatora.
	* Każde ciało niebieskie jest rysowane osobno
	* Każde ciało niebieskie ma uprzednio rysowany jego ślad orbity,
	  którego czas zanikania da się przestawiać
* Pierwotna implementacja metody `tick` w Symulatorze
	* Wykonywanie fukncji obliczającej siłę grawitacji dla każdej pary
	  obiektów oraz stosowanie jej efektów na prędkościach obiektów
	* Poruszenie każdym ciałem zgodnie z jego prędkością
* Detekcja kolizji
* Obsługa kolizji w funkcji statycznej w klasie `Celestial_body`
	* Zachowanie momentu pędu
	* Tworzenie nowego obiektu w zależności od mas obiektów kolidujących ze sobą (RTTI na `Celestial_body*`)
	* Scalenie śladów zderzających się ciał w nowym obiekcie `Celestial_body` i zapisanie ich do specjalnej listy sladów odziedziczonych
	* Spośród zderzających się obiektów: usunięcie pierwszego (w funkcji `Simulator::tick()`) i nadpisanie drugiego nowopowstałym.
* Obsługa poprawnego skalowania okna
* Pauzowanie symulacji
* Zmiana widoku w oknie zgodnie poprzez scrollowanie i klawisze strzałek
## Do zrobienia
* Stworzyć klasę `Asteroid` jako kolejny typ ciał niebieskich
* Odpowiednie teksturowanie ciał niebieskich
* Wyświetlanie interfejsów graficznych
* Przerobić statyczny diagram alokacji w `Celestial_body` z `std::vector<bool>` na `std::map<unsigned int, unsigned int>`
* Stworzenie przycisków w GUI
* Okodowanie `CB_selector`, `CB_gen` i `UI_masterpanel`
	* Generacja ciał niebieskich różnych typów
		* Stworzenie `Textbox` do wybierania masy tworzonego ciała
	* Modyfikacja parametrów ciał niebieskich
	* Stworzenie narzędzia do manipulacji czasem
* Ładowanie tekstur z pliku w `LoadResources()`
* Rozplanować i dodać tryb sterowalnego statku kosmicznego
## Jak skompilować?
Aby skompilować projekt, należy skompilować wszystkie dostępne w repozytorium pliki .cpp - wszystko ma zostać składową pliku wykonywalnego "Grawitacja.exe", do którego należey dolinkować bibliotekę SFML (moduł graficzny tej biblioteki)
Wymagany standard C++11 wraz z RTTI.
## Do rozważenia
* Śledzenie widokiem danego ciała niebieskiego
* Zapis i odczyt z pliku przez moduł XML
