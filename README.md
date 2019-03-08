# Grawitacja
Banalny symulator grawitacji i planet, SFML, C++.
## Cel projektu
* Ćwiczenia programistyczne w zakresie praktycznego wykorzystania programowania obiektowego w języku C++, w tym metod wirtualnych, dziedziczenia.
* Ćwiczenia korzystania z kontenerów biblioteki standardowej języka C++.
* Upłynnienie posługiwania się inteligentnymi wskaźnikami (`std::unique_ptr<>`).
* Ćwiczenia w zakresie tworzenia prostych interaktywnych aplikacji graficznych w bibliotece SFML.
* Stworzenie prostej zabawki pomagającej zrozumieć działanie mechaniki orbitalnej.
* Bliższe zapoznanie się z mechaniką Git oraz GitHub; ćwiczenia w kooperacji wieloosobowej.
## Możliwości programu
* Symulacja grawitacji w układzie wielu ciał w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych ciał niebieskich w czasie działania programu posiadających prędkość, położenie i zwrot nadawane w ramach zdarzeń wciśnięcia i opuszczenia przycisku myszy.
* Zcalanie ciał niebieskich w razie kolizji (planety wchłaniają asteroidy, a gwiazdy wchłaniają wszystko pozostałe).
* Usuwanie istniejących ciał niebieskich.
* Rozróżnianie między różnymi typami spektralnymi gwiazd oraz różnymi typami planet
## Opis techniczny
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura główna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę `draw` (sam obiekt dziedzicyz `sf::Drawable`). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (`std::list<std::unique_ptr<Celestial_body>>`) które wszystkie dziedziczą z `Celestial_body`, która to klasa również jest pochodną klasy `sf::Drawable`). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda `tick`, w ramach której prowadzona jest obsługa całej symulacji.
Program będzie się obsługiwać za pomocą zarówno myszy jak i klawiatury. Okno programu będzie posiadało specjalny obiekt przechowujący i sterujący stanem interfejsu graficznego (`UI_state`). Występujące w oknie zdarzenia sterujące programem są przekierowywane do specjalnych funkcji obiektu sterującego. Pewnym klawiszom zostaną przypisane funkcje przełączające tzw. narzędzie interfejsu graficznego (`UI_tool`). Planowanych jest kilka narzędzi UI, w tym m.in. generator ciał niebieskich, selektor i usuwacz.
## Obecny stan implementacji
* Podstawowa mechanika klasy `Celestial_body`
* Zarys klas `Planet` oraz `Star`
* Szkielet mechanizmu interfejsów graficznych
* Podstawowa mechanika pokazywania i odświeżania okna, a także tworzenia i rysowania obiektu symulatora.
* Pierwotna implementacja metody `tick` w Symulatorze
	* Wykonywanie fukncji obliczającej siłę grawitacji dla każdej pary
	  obiektów
	* Poruszenie każdym ciałem zgodnie z jego prędkością
## Do zrobienia
* Odpowiednie teksturowanie ciał niebieskich
* Przełączanie narzędzi GUI
* Obsługa kolizji w specjalnej funkcji zaprzyjaźnionej
* Wyświetlanie tekstu interfejsów graficznych
* Stworzenie przycisków w GUI
* Obsługa poprawnego skalowania okna
* Zmiana widoku w oknie zgodnie poprzez scrollowanie i klawisze strzałek
* Funkcja `Simulator::at_pos`
* Okodowanie `CB_selector` i `CB_gen`
	* Generacja ciał niebieskich różnych typów
	* Usuwanie ciał niebieskich
	* Modyfikacja parametrów ciał niebieskich
* Pauzowanie symulacji
* Detekcja kolizji
## Jak skompilować?
Aby skompilować projekt, należy skompilować wszystkie dostępne w repozytorium pliki .cpp - wszystko ma zostać składową pliku wykonywalnego "Grawitacja.exe", do którego należey dolinkować bibliotekę SFML (moduł graficzny tej biblioteki)
Wymagany standard C++11 wraz z RTTI.
## Do rozważenia
* Przypisanie pewnym klawiszom zmiany tempa symulacji
* Śledzenie widokiem danego ciała niebieskiego

