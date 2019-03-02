# Grawitacja
Banalny symulator grawitacji i planet, SFML, C++.
# Cel projektu
* Ćwiczenia programistyczne w zakresie praktycznego wykorzystania programowania obiektowego w języku C++, w tym metod wirtualnych, dziedziczenia, korzystania z kontenerów biblioteki standardowej.
* Ćwiczenia w zakresie tworzenia prostych interaktywnych aplikacji graficznych w bibliotece SFML.
* Stworzenie prostej zabawki pomagającej zrozumieć działanie mechaniki orbitalnej.
# Możliwości programu
* Symulacja grawitacji w układzie wielu ciał w czasie rzeczywistym - gwiazd, planet i asteroid.
* Dodawanie nowych ciał niebieskich w czasie działania programu posiadających prędkość, położenie i zwrot nadawane w ramach zdarzeń wciśnięcia i opuszczenia przycisku myszy.
* Zcalanie ciał niebieskich w razie kolizji.
* Usuwanie istniejących ciał niebieskich.
# Opis techniczny
Cały program głęboko bazuje na bibliotece SFML i jej filozofii obiektowej.
Procedura główna (main) otwiera okno na którym w każdym cyklu odświeżenia okna (60FPS) rysuje obiekt Symulatora, wywołując metodę <code>draw</code> (sam obiekt dziedzicy z <code>sf::Drawable</code>). Rysowanie obiektu sprowadza się do narysowania całej jego zawartości (jest to lista obiektów (<code>std::list</code>) które wszystkie dziedziczą z <code>Celestial_body</code>, która to klasa również jest pochodną klasy <code>sf::Drawable</code>). Ponadto oprócz rysowania, na obiekcie symulatora wywoływana jest metoda <code>tick</code>, w ramach której prowadzona jest obsługa całej symulacji.
