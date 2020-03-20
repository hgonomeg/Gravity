# Gravity
Simplistic, yet fully featured, simulator of gravity between celestial bodies, including stars, planets and asteroids.
SFML, C++11.

## Goals and capabilities of the program
* Realtime simulation of celestial bodies in a multi-bodied system, including stars, planets and asteroids
* Adding new celestial bodies, ad hoc, with spatial and dynamic properties given by mouse clicks.
* Handling collisions of celestial bodies:
	* merging
	* bouncing
	* hybrid
* Deletion of celestial bodies
* Differentiating between different planet types and different star spectral types
* Control of both the pace and accuracy of the simulation
* A special mode made for entertainment purposes: controlling a spaceship

## Building instructions
### Library dependencies
* SFML (graphics, window and system modules)
### Tools required
* Cmake
* C++17 compiler with RTTI
### Instruction
In order to compile the program, please ensure that the SFML library is installed in your system and that you have all the necessary tools like Cmake and a compiler.

Run cmake to generate the build files for your setup (I personally recommend using Cmake GUI).
Please note that static linking will not be available in all setups.

After obtaining the build files, use your toolchain of choice to compile the program.
(eg. run `make` if you are using Unix Makefiles or MinGW Makefiles)

An executable called "Gravity" will appear in the root directory of the project

### Note
I'd be very welcome if you could report any problems with the Cmake script that you encounter.

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
* Gaining some experience with cmake-scripts

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
* Proper texturing of celestial bodies
* Celestial body track prediction
* Further improve `CB_selector`, `CB_gen` and `UI_masterpanel`
	* More key-bindings
	* Implement a `Textbox` to enter numbers like a celestial body's mass
	* Add dedicated buttons to choose between the types of celestial bodies
	* Implement edition of celestial bodies
	* Print more data in the debug mode
* Implement loading textures in the global resources manager
* Design and implement maneuverable spaceship mode
* Tracing celestial bodies with the camera
* Read/Save the simulator state via a dedicated XML/JSON module
* Quick-saves and restores of the simulator state
* Optimize rendering
## Wuxing
In the `pairalgo` folder, there is a little animation engine called Wuxing. It visualizes how individual-unique-pair iteration algorithms work. It was written to serve as a helper in designing efficient algorithms for calculating the force of gravity between every unique pair of celestial bodies.

It helped in developing the Tianche algorithm.


## To consider

