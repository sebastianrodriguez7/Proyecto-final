import argparse
import math
import os
import sys

from PIL import Image


def color_desde_texto(texto):
    texto = texto.lstrip("#")
    if len(texto) != 6:
        raise ValueError("El color debe ser como #FF00FF")
    return tuple(int(texto[i:i + 2], 16) for i in (0, 2, 4))


def quitar_fondo(imagen, color, tolerancia):
    """Vuelve transparente todo lo parecido al color de fondo, con borde suave."""
    imagen = imagen.convert("RGBA")
    pixeles = imagen.load()
    ancho, alto = imagen.size
    margen = tolerancia * 0.6
    for y in range(alto):
        for x in range(ancho):
            r, g, b, a = pixeles[x, y]
            d = math.sqrt((r - color[0]) ** 2 + (g - color[1]) ** 2 + (b - color[2]) ** 2)
            if d <= tolerancia:
                pixeles[x, y] = (r, g, b, 0)
            elif d < tolerancia + margen:
                nuevo_alfa = int(a * (d - tolerancia) / margen)
                pixeles[x, y] = (r, g, b, nuevo_alfa)
    return imagen


def recortar(imagen):
    """Recorta al contenido visible (así los pies quedan en el borde de abajo)."""
    caja = imagen.getchannel("A").point(lambda v: 255 if v > 8 else 0).getbbox()
    if caja is None:
        raise ValueError("La imagen quedó vacía: revisa el color de fondo o la tolerancia")
    return imagen.crop(caja)


def main():
    p = argparse.ArgumentParser(description="Prepara sprites para el juego")
    p.add_argument("entradas", nargs="+", help="imágenes PNG/JPG con el nombre final (roca.png...)")
    p.add_argument("--salida", required=True, help="carpeta de salida (por ejemplo recursos/sprites)")
    p.add_argument("--tam", type=int, default=384, help="lado mayor del sprite en píxeles")
    p.add_argument("--fondo", default=None, help="color de fondo a quitar, por ejemplo #FF00FF")
    p.add_argument("--tolerancia", type=float, default=70, help="qué tan parecido al fondo se borra")
    p.add_argument("--grupo", action="store_true", help="misma escala para todas las imágenes")
    a = p.parse_args()

    os.makedirs(a.salida, exist_ok=True)
    color = color_desde_texto(a.fondo) if a.fondo else None

    recortadas = []
    for ruta in a.entradas:
        img = Image.open(ruta).convert("RGBA")
        if color is not None:
            img = quitar_fondo(img, color, a.tolerancia)
        recortadas.append((ruta, recortar(img)))

    mayor_comun = max(max(img.size) for _, img in recortadas)
    for ruta, img in recortadas:
        lado = mayor_comun if a.grupo else max(img.size)
        escala = a.tam / lado
        nuevo = (max(1, round(img.width * escala)), max(1, round(img.height * escala)))
        final = img.resize(nuevo, Image.LANCZOS)
        destino = os.path.join(a.salida, os.path.splitext(os.path.basename(ruta))[0] + ".png")
        final.save(destino)
        print(f"{os.path.basename(ruta)} -> {destino}  ({final.width} x {final.height})")


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print("Error:", error)
        sys.exit(1)