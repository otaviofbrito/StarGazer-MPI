import matplotlib.pyplot as plot
import sys

"""
The `Image` class represents an image with the following attributes:
- `matrix`: A 2D matrix representing the image.
- `height`: The height of the image.
- `width`: The width of the image.
- `gray_level`: The maximum gray level in the image.
"""


class Image:
    def __init__(self, matrix, height, width, gray_level):
        self.matrix = matrix
        self.height = height
        self.width = width
        self.gray_level = gray_level


"""
Loads a `.pgm` image and returns an instance of the `Image` class.
"""


def readpgm(name) -> Image:
    with open(name, "r") as f:

        assert f.readline() == 'P2\n'
        line = f.readline()
        while (line[0] == '#'):
            line = f.readline()

        (width, height) = [int(i) for i in line.split()]
        depth = int(f.readline())
        assert depth <= 255

        img = []
        row = []
        j = 0
        for line in f:
            values = line.split()
            for val in values:
                row.append(int(val))
                j = j + 1
                if j >= width:
                    img.append(row)
                    j = 0
                    row = []
        pic = Image(matrix=img, height=height, width=width, gray_level=depth)

    return pic


def histogram(img: Image):
    hist = [0] * (img.gray_level+1)
    for i in range(img.height):
        for j in range(img.width):
            hist[img.matrix[i][j]] += 1
    return hist


def main(argc, argv):
    if argc != 2:
        raise RuntimeError(f"Usage: python {argv[0]} path/to/image.pgm")

    img = readpgm(argv[1])
    plot.imshow(img.matrix, cmap='gray')

    plot.axis('off')

    histograma = histogram(img)

    plot.figure()
    plot.title("Histogram")
    plot.xlabel("Pixel value")
    plot.ylabel("Num. of pixels")
    plot.plot(histograma, color='black')
    plot.xlim([0, 255])
    plot.show()

    return 0


if __name__ == '__main__':
    main(len(sys.argv), sys.argv)
