import numpy as np
from pixelFont import *

# display class
class Display:
    def __init__(self, width, height):
        self.width, self.height = width, height
        self.pixels = np.zeros((width, height, 3))

    def __getitem__(self, key):
        return self.pixels[key]

    def __setitem__(self, key, value):
        self.pixels[key] = value

    def __str__(self):
        string = ""
        for j in range(self.height):
            for i in range(self.width):
                string += ("██" if self.pixels[i, j, 0]== True else "░░")
            string += "\n"
        return string[:-1]

    def reset(self):
        self.pixels = np.zeros((width, height, 3))

    def display_text(self, text, color=[255, 0, 0]):
        pixelmap = text.get_pixelmap()
        self.pixels = [[color if pixel==1 else [0,0,0] for i in range(self.width)]]
        self.pixels = text.get_pixelmap()[self.width, self.height]

if __name__ == "__main__":
    font1 = Font()
    font1.readFromImage("./font1.png")
    font2 = Font()
    font2.readFromImage("./font2.png")
    text1 = Text("HACKERSPACE", font1)
    text2 = Text("HACKERSPACE", font2)
    print(text1)
    print(text2)
    d = Display(6, 7)
    d.display_text(text2)
    print(d)