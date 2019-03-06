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
## Opis techniczny
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura główna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę <code>draw</code> (sam obiekt dziedzicy z <code>sf::Drawable</code>). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (<code>std::list<std::unique_ptr<Celestial_body>></code>) które wszystkie dziedziczą z <code>Celestial_body</code>, która to klasa również jest pochodną klasy <code>sf::Drawable</code>). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda <code>tick</code>, w ramach której prowadzona jest obsługa całej symulacji.
## Obecny stan implementacji
* Podstawowa mechanika klasy `Celestial_body`
* Podstawowa mechanika pokazywania i odświeżania okna, a także tworzenia i rysowania obiektu symulatora.
## Do zrobienia
* Wszystko pozostałe
## Jak skompilować?
Aby skompilować projekt, należy skompilować wszystkie dostępne w repozytorium pliki .cpp - wszystko ma zostać składową pliku wykonywalnego "Grawitacja.exe", do którego należey dolinkować bibliotekę SFML (moduł graficzny tej biblioteki)
