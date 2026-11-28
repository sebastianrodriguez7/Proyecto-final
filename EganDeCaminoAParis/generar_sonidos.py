#!/usr/bin/env python3
"""
Genera sonidos ORIGINALES para el juego (efectos y música en bucle) y los guarda como .wav.
No necesita instalar nada: usa solo la biblioteca estándar de Python.

Uso:   python generar_sonidos.py recursos/audio

Son sonidos sencillos, pensados como base para que el juego suene desde el primer día.
Puedes reemplazar cualquiera por otro archivo con el mismo nombre.
"""
import math
import os
import random
import struct
import sys
import wave

SR = 22050                        # muestras por segundo
random.seed(7)                    # para que siempre salgan igual


# ------------------------------------------------------------ herramientas

def silencio(segundos):
    return [0.0] * int(SR * segundos)


def forma(tipo, fase):
    """Una onda entre -1 y 1. fase va de 0 a 1 en cada ciclo."""
    if tipo == "cuadrada":
        return 0.6 if fase < 0.5 else -0.6
    if tipo == "triangulo":
        return 4 * abs(fase - 0.5) - 1
    if tipo == "sierra":
        return 2 * fase - 1
    return math.sin(2 * math.pi * fase)


def tono(f0, f1, dur, tipo="seno", vol=1.0, caida=4.0, ataque=0.004):
    """Un tono que va de la frecuencia f0 a f1, con envolvente que se apaga."""
    n = int(SR * dur)
    salida = []
    fase = 0.0
    for i in range(n):
        t = i / n
        f = f0 * (f1 / f0) ** t                       # barrido exponencial
        fase = (fase + f / SR) % 1.0
        env = math.exp(-caida * t)
        if i < ataque * SR:
            env *= i / (ataque * SR)
        salida.append(forma(tipo, fase) * env * vol)
    return salida


def ruido(dur, vol=1.0, caida=6.0, suavizado=0.5):
    """Ruido filtrado (más grave cuanto mayor es el suavizado)."""
    n = int(SR * dur)
    salida = []
    previo = 0.0
    for i in range(n):
        crudo = random.uniform(-1, 1)
        previo = previo * suavizado + crudo * (1 - suavizado)
        salida.append(previo * math.exp(-caida * i / n) * vol)
    return salida


def pulsacion(freq, dur, vol=1.0, amortiguacion=0.996):
    """Cuerda pulsada (Karplus-Strong): suena como una guitarra o un arpa."""
    n = int(SR * dur)
    largo = max(2, int(SR / freq))
    cuerda = [random.uniform(-1, 1) for _ in range(largo)]
    salida = []
    idx = 0
    for _ in range(n):
        actual = cuerda[idx]
        siguiente = cuerda[(idx + 1) % largo]
        cuerda[idx] = amortiguacion * 0.5 * (actual + siguiente)
        salida.append(actual * vol)
        idx = (idx + 1) % largo
    return salida


def colocar(base, pista, inicio, envolver=False):
    """Suma 'pista' dentro de 'base' a partir del segundo 'inicio'."""
    desde = int(SR * inicio)
    for i, v in enumerate(pista):
        pos = desde + i
        if pos >= len(base):
            if not envolver:
                break
            pos %= len(base)                          # la cola vuelve al principio: el bucle no se corta
        base[pos] += v


def suavizar_bucle(muestras, milisegundos=12):
    """Pequeño fundido al principio y al final para que el bucle no haga 'clic' al reiniciar."""
    n = int(SR * milisegundos / 1000)
    for i in range(n):
        factor = i / n
        muestras[i] *= factor
        muestras[-1 - i] *= factor
    return muestras


def guardar(carpeta, nombre, muestras, pico=0.8):
    mayor = max(1e-9, max(abs(m) for m in muestras))
    escala = pico / mayor
    datos = b"".join(struct.pack("<h", int(max(-1.0, min(1.0, m * escala)) * 32767)) for m in muestras)
    ruta = os.path.join(carpeta, nombre + ".wav")
    with wave.open(ruta, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(datos)
    return ruta


# ------------------------------------------------------------ efectos

def efectos():
    e = {}

    e["salto"] = tono(300, 720, 0.18, "cuadrada", 0.7, caida=3)

    base = ruido(0.22, 0.9, caida=7, suavizado=0.6)
    colocar(base, tono(160, 55, 0.22, "seno", 1.0, caida=5), 0)
    e["golpe"] = base

    moneda = silencio(0.2)
    colocar(moneda, tono(880, 880, 0.08, "cuadrada", 0.6, caida=2), 0.0)
    colocar(moneda, tono(1320, 1320, 0.12, "cuadrada", 0.6, caida=3), 0.08)
    e["bidon"] = moneda

    guitarra = silencio(0.9)
    for i, f in enumerate([196, 247, 294, 392]):                # sol, si, re, sol
        colocar(guitarra, pulsacion(f, 0.6, 0.7), i * 0.09)
    e["guitarra"] = guitarra

    pajara = tono(420, 110, 0.8, "triangulo", 0.9, caida=1.5)
    for i in range(len(pajara)):                                # temblor de cansancio
        pajara[i] *= 0.75 + 0.25 * math.sin(2 * math.pi * 9 * i / SR)
    e["pajara"] = pajara

    desinflado = tono(520, 90, 0.55, "sierra", 0.5, caida=2.5)
    colocar(desinflado, ruido(0.55, 0.35, caida=3, suavizado=0.8), 0)
    e["desinflado"] = desinflado

    victoria = silencio(1.4)
    for i, f in enumerate([262, 330, 392, 523]):                # do, mi, sol, do alto
        colocar(victoria, tono(f, f, 0.5, "cuadrada", 0.5, caida=2.0), i * 0.16)
    for f in (262, 330, 392, 523):
        colocar(victoria, tono(f, f, 0.7, "triangulo", 0.35, caida=2.5), 0.7)
    e["victoria"] = victoria

    derrota = silencio(1.2)
    for i, f in enumerate([392, 330, 262, 196]):
        colocar(derrota, tono(f, f * 0.97, 0.38, "triangulo", 0.8, caida=2.5), i * 0.27)
    e["derrota"] = derrota

    lanzar = ruido(0.26, 0.5, caida=3, suavizado=0.85)
    colocar(lanzar, tono(220, 520, 0.26, "seno", 0.5, caida=3), 0)
    e["lanzar"] = lanzar

    impacto = tono(130, 45, 0.3, "seno", 1.0, caida=5)
    colocar(impacto, ruido(0.12, 0.6, caida=8, suavizado=0.5), 0)
    e["impacto"] = impacto

    bonk = tono(620, 190, 0.22, "cuadrada", 0.6, caida=4)
    colocar(bonk, ruido(0.1, 0.5, caida=8, suavizado=0.55), 0)
    e["rival_detenido"] = bonk

    zumbido = silencio(0.6)
    colocar(zumbido, tono(220, 220, 0.28, "sierra", 0.5, caida=2.0), 0.0)
    colocar(zumbido, tono(165, 165, 0.3, "sierra", 0.5, caida=2.0), 0.28)
    e["vida_perdida"] = zumbido

    afic = silencio(0.35)
    colocar(afic, tono(660, 990, 0.12, "cuadrada", 0.55, caida=2), 0.0)
    colocar(afic, tono(990, 990, 0.18, "cuadrada", 0.55, caida=3), 0.12)
    e["aficionado"] = afic

    return e


# ------------------------------------------------------------ música (bucles)

NOTAS = {"A2": 110.0, "F2": 87.3, "C3": 130.8, "G2": 98.0, "E2": 82.4, "E3": 164.8, "A3": 220.0,
         "F3": 174.6, "G3": 196.0, "B3": 246.9, "C4": 261.6, "D4": 293.7, "E4": 329.6, "G4": 392.0,
         "A4": 440.0, "B2": 123.5, "D3": 146.8, "F#4": 370.0}


def musica_sentimental():
    """8 segundos exactos: un arpegio de guitarra (sol - re - mi menor - do). Música original."""
    dur = 8.0
    base = silencio(dur)
    acordes = [("G3", "B3", "D4", "G4"),      # sol mayor
               ("D3", "A3", "D4", "F#4"),     # re mayor
               ("E3", "G3", "B3", "E4"),      # mi menor
               ("C4", "E3", "G3", "C4")]      # do mayor
    patron = [0, 1, 2, 3, 2, 1, 2, 1]
    for c, acorde in enumerate(acordes):
        for k, indice in enumerate(patron):
            colocar(base, pulsacion(NOTAS[acorde[indice]], 1.3, 0.8), c * 2.0 + k * 0.25, envolver=True)
    return base


def musica_nivel(duracion, bpm, acordes, melodia_por_compas, rapido):
    """Bajo + melodía de cuerda pulsada, en bucle."""
    base = silencio(duracion)
    pulso = 60.0 / bpm
    compases = int(round(duracion / (pulso * 4)))
    for c in range(compases):
        raiz, quinta, notas = acordes[c % len(acordes)]
        t0 = c * pulso * 4
        # bajo
        paso = pulso / 2 if rapido else pulso
        golpes = int(round(pulso * 4 / paso))
        for g in range(golpes):
            f = NOTAS[raiz] if g % 2 == 0 else NOTAS[quinta]
            colocar(base, tono(f, f, paso * 0.9, "triangulo", 0.55, caida=3), t0 + g * paso, envolver=True)
        # melodía
        for k, indice in enumerate(melodia_por_compas[c % len(melodia_por_compas)]):
            if indice is None:
                continue
            colocar(base, pulsacion(NOTAS[notas[indice]], pulso * 1.6, 0.9), t0 + k * pulso, envolver=True)
    return base


def musica_nivel1():
    acordes = [("A2", "E3", ("A3", "C4", "E4")), ("F2", "C3", ("F3", "A3", "C4")),
               ("C3", "G3", ("C4", "E4", "G4")), ("G2", "D4", ("G3", "B3", "D4"))]
    melodia = [[0, 1, 2, 1], [2, 1, 0, 1], [0, 2, 1, 2], [1, 0, 1, 2]]
    return musica_nivel(16.0, 120, acordes, melodia, rapido=False)


def musica_nivel2():
    acordes = [("A2", "E3", ("A3", "C4", "E4")), ("G2", "D4", ("G3", "B3", "D4")),
               ("F2", "C3", ("F3", "A3", "C4")), ("E2", "B2", ("E3", "G3", "B3"))]
    melodia = [[2, None, 1, 2], [1, 2, None, 2], [2, 1, 0, None], [0, 2, 1, 2]]
    return musica_nivel(12.8, 150, acordes, melodia, rapido=True)


# ------------------------------------------------------------ principal

def main():
    if len(sys.argv) != 2:
        print("Uso: python generar_sonidos.py carpeta_de_salida   (por ejemplo recursos/audio)")
        sys.exit(1)
    carpeta = sys.argv[1]
    os.makedirs(carpeta, exist_ok=True)

    todos = efectos()
    todos["musica_sentimental"] = suavizar_bucle(musica_sentimental())
    todos["musica_nivel1"] = suavizar_bucle(musica_nivel1())
    todos["musica_nivel2"] = suavizar_bucle(musica_nivel2())

    for nombre, muestras in todos.items():
        ruta = guardar(carpeta, nombre, muestras)
        print(f"{nombre + '.wav':26s} {len(muestras) / SR:5.2f} s   {os.path.getsize(ruta) / 1024:6.1f} KB")


if __name__ == "__main__":
    main()