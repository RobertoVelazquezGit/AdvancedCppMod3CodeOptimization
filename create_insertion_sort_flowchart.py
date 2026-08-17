from pathlib import Path

from reportlab.lib import colors
from reportlab.lib.pagesizes import A4
from reportlab.pdfbase.pdfmetrics import stringWidth
from reportlab.pdfgen import canvas


OUT = Path("output/pdf/insertionSort_diagrama_de_flujo.pdf")


def centered_text(c, text, x, y, width, size=10, leading=12):
    lines = text.split("\n")
    start = y + (len(lines) - 1) * leading / 2
    for idx, line in enumerate(lines):
        c.drawCentredString(x + width / 2, start - idx * leading, line)


def rounded_box(c, x, y, w, h, text, fill, stroke=colors.HexColor("#16324F")):
    c.setFillColor(fill)
    c.setStrokeColor(stroke)
    c.setLineWidth(1.2)
    c.roundRect(x, y, w, h, 10, stroke=1, fill=1)
    c.setFillColor(colors.HexColor("#14212B"))
    c.setFont("Helvetica", 10)
    centered_text(c, text, x, y + h / 2 - 4, w)


def diamond(c, x, y, w, h, text):
    p = c.beginPath()
    p.moveTo(x + w / 2, y + h)
    p.lineTo(x + w, y + h / 2)
    p.lineTo(x + w / 2, y)
    p.lineTo(x, y + h / 2)
    p.close()
    c.setFillColor(colors.HexColor("#FFF3C4"))
    c.setStrokeColor(colors.HexColor("#8A6200"))
    c.setLineWidth(1.2)
    c.drawPath(p, stroke=1, fill=1)
    c.setFillColor(colors.HexColor("#14212B"))
    c.setFont("Helvetica", 9)
    centered_text(c, text, x, y + h / 2 - 4, w, 9)


def arrow(c, x1, y1, x2, y2, label=None, lx=None, ly=None):
    c.setStrokeColor(colors.HexColor("#355C7D"))
    c.setFillColor(colors.HexColor("#355C7D"))
    c.setLineWidth(1.3)
    c.line(x1, y1, x2, y2)
    c.saveState()
    import math
    angle = math.atan2(y2 - y1, x2 - x1)
    c.translate(x2, y2)
    c.rotate(angle * 180 / math.pi)
    p = c.beginPath(); p.moveTo(0, 0); p.lineTo(-7, 3.5); p.lineTo(-7, -3.5); p.close()
    c.drawPath(p, stroke=0, fill=1)
    c.restoreState()
    if label:
        c.setFont("Helvetica-Bold", 8)
        c.drawString(lx, ly, label)


def main():
    OUT.parent.mkdir(parents=True, exist_ok=True)
    c = canvas.Canvas(str(OUT), pagesize=A4)
    page_w, page_h = A4
    c.setTitle("Diagrama de flujo - insertionSort")
    c.setFillColor(colors.HexColor("#16324F"))
    c.setFont("Helvetica-Bold", 18)
    c.drawCentredString(page_w / 2, 806, "Diagrama de flujo: insertionSort")
    c.setFillColor(colors.HexColor("#52616B"))
    c.setFont("Helvetica", 9)
    c.drawCentredString(page_w / 2, 788, "BasicCodeOptimization.cpp - ordenacion por insercion")

    x, w = 190, 215
    rounded_box(c, x, 742, w, 30, "Inicio: insertionSort(data)", colors.HexColor("#D9EEF7"))
    diamond(c, x + 17, 666, 181, 54, "i = 1;\ni < data.size()?")
    rounded_box(c, x, 600, w, 40, "key = data[i]\nj = static_cast<int>(i) - 1", colors.HexColor("#E5F4E3"))
    diamond(c, x + 17, 516, 181, 56, "j >= 0 Y\ndata[j] > key?")
    rounded_box(c, x, 440, w, 42, "data[j + 1] = data[j]\nj--", colors.HexColor("#E5F4E3"))
    rounded_box(c, x, 354, w, 38, "data[j + 1] = key", colors.HexColor("#E5F4E3"))
    rounded_box(c, x, 276, w, 32, "++i", colors.HexColor("#E5F4E3"))
    rounded_box(c, x, 208, w, 30, "Fin: vector ordenado", colors.HexColor("#F7D9DE"))

    arrow(c, x + w / 2, 742, x + w / 2, 720)
    arrow(c, x + w / 2, 666, x + w / 2, 640, "Si", 413, 650)
    arrow(c, x + w / 2, 600, x + w / 2, 572)
    arrow(c, x + w / 2, 516, x + w / 2, 482, "Si", 413, 494)
    arrow(c, x + w / 2, 440, x + w / 2, 392)
    # Back edge within the while loop.
    c.setStrokeColor(colors.HexColor("#355C7D")); c.setLineWidth(1.3)
    c.line(x, 461, 150, 461); c.line(150, 461, 150, 544); c.line(150, 544, x + 17, 544)
    arrow(c, 150, 544, x + 17, 544, "repetir", 153, 550)
    arrow(c, x + w / 2, 516, x + w / 2, 392, "No", 413, 420)
    arrow(c, x + w / 2, 354, x + w / 2, 308)
    # Back edge for outer loop.
    c.line(x + w, 292, 470, 292); c.line(470, 292, 470, 693); c.line(470, 693, x + 198, 693)
    arrow(c, 470, 693, x + 198, 693, "repetir", 472, 700)
    arrow(c, x + 17, 693, x - 52, 693, "No", 145, 700)
    arrow(c, x - 52, 693, x - 52, 223)
    arrow(c, x - 52, 223, x, 223)

    c.setFillColor(colors.HexColor("#52616B"))
    c.setFont("Helvetica-Oblique", 8)
    c.drawString(58, 90, "El bucle interno desplaza a la derecha los elementos mayores que key.")
    c.drawString(58, 77, "Al terminar, key se inserta en la posicion j + 1; el prefijo [0..i] queda ordenado.")
    c.showPage()
    c.save()


if __name__ == "__main__":
    main()
