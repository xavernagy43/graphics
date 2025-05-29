# Számítógépi grafika beadandó leírás

# Wolf Hunting

## Projektleírás

A "Wolf Hunting" egy 3D-s játék, melyben a játékos egy farkast irányít, aki az erdőben különböző állatokra vadászik. A játék OpenGL és SDL2 használatával készül.

## Játékkoncepció

### Alapötlet
A játék célja, hogy a játékos minél több állatot elkapjon egy adott időn belül. A játék során az alábbi főbb elemek jelennek meg:
- Játszható karakter: Egy farkas
- Vadállatok: Nyúl, őz, vaddisznó
- Környezet elemek: Fák, bokrok, sziklák, fatörzsek, gombák

### Játéktér
- Erdős környezet, amely tartalmaz:
  - Hegyes területet: A pálya egyik végén, amely akadályként szolgál.
  - Tavat: A másik végén, amelybe nem lehet bemenni. A tó fényviszonytól függően csillanásokat és (tükröződést?) mutat.

### Technikai megvalósítás
- Grafika: OpenGL + SDL2
- Textúrák és modellek betöltése: OBJ fájlformátum és BMP/PNG/JPG textúrák használatával
- Fények és árnyékok: Alapvető világításmodellek (pl. Phong, ambient/diffuse/specular)
- Ütközésvizsgálat: A karakter nem haladhat át objektumokon és nem mehet be a tóba vagy a hegyekbe.
- Kamera: Egérrel mozgatható a kamera a farkas körül (third person)
- Irányítás: WASD mozgás, egérrel kamera
- Animációk: Az állatok és a farkas mozgása animációval történik
- Hangok: Állatok hangjai és lépéshangok SDL_mixer vagy más hangkönyvtár segítségével
- HUD: Jobb felső sarokban pontszám és idő kijelzés

## Menü

A játék indításakor egy főmenü jelenik meg az alábbi lehetőségekkel:

- Start Game: Játék indítása
- Previous Runs: Az utóbbi 20 játék pontszáma megjelenítve
- Settings:
  - Játék fényerejének beállítása
  - Irányítási gombok testreszabása
  - Előző eredmények törlése
  - Játék hangereje
  - Köd effektus be/ki (shader és mélység-alapú köd)
  - Max játékidő beállítása (perc:másodperc formátumban)
- Help: Rövid ismertető az irányításról és a játékszabályokról (ez megjeleníthető játék közben is h-lenyomására)
- Exit: Kilépés


## Futtatás előtt

1. 
  1. Telepíts egy GLUT implementációt
    A leggyakoribb választás: FreeGLUT (a GLUT nyílt forráskódú, modernizált változata).

  2. Töltsd le a FreeGLUT-ot Windowsra
    http://www.transmissionzero.co.uk/software/freeglut-devel/
    A letöltött fájlok tartalmazni fogják:
    freeglut.dll – futtatáshoz kell
    freeglut.lib vagy libfreeglut.a – linkeléshez kell
    GL/freeglut.h – fejlécek

  3. Telepítés MinGW alá:
    .a fájlokat másold a MinGW\lib mappába
    .h fájlokat másold a MinGW\include\GL mappába
    .dll fájlt másold a programod futtatható fájljának mellé

2. 
  1. Töltsd le az SDL2_mixer Windows/MinGW verzióját
  https://github.com/libsdl-org/SDL_mixer/releases
  – Keresd a fájlt, pl.:
    SDL2_mixer-devel-2.x.x-mingw.tar.gz (nem MSVC!)

  2. Másold a fájlokat:
  A. Fejléc:
    SDL_mixer.h → másold be:
    c_sdk_220203/mingw/include/SDL2/ könyvtárba

  B. Lib fájl:
    libSDL2_mixer.a → másold be:
    c_sdk_220203/mingw/lib/

  C. DLL fájl:
    SDL2_mixer.dll → másold a futtatható (wolf.exe) mellé