# ecg-filter
Wendet filter auf eine Datei mit EKG-Daten an und gibt einen Graph als svg-Datei aus.

# Herunterladen:

Entweder

    git clone https://github.com/rasmusantons/ecg-filter

oder als .zip runterladen und entpacken

# Kompilieren:
Voraussetzung: gcc (oder anderer C Compiler), optional cmake

Entweder

    cmake .
    make

oder

    gcc -o ecg-filter main.c filters.c graph.c csv.c

# Anwenden:

Hilfe mit ./ecg-filter --help anzeigen

Mögliche Filter sind:

<ul>
<li>mean2, mean5, mean10: Mittelwert aus 2/5/10 vorherigen Werten</li>
<li>derivative: Differenz zum vorherigen Wert</li>
<li>square: Quadrierter Wert</li>
<li>detect: 1 (falls Wert größer als 0.1), 0 sonst</li>
</ul>

Beispielaufruf:

    ./ecg-filter test.in -o test.out -g test.svg -l 10000 -f mean5,derivative,square,detect
