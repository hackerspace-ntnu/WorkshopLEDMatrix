# defines the following characters in a 'font'
# ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.?!:-)([]{}<>
# Font is being read from an image created in inkscape
# between each character is a collumn of white pixels
# this is used to detect the individual characters

import numpy as np
from PIL import Image

class Font:
    charKeys = r"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.?!:-)([]{}<>"
    def __init__(self):
        self.characters = {}

    def readFromImage(self, pathToImage):
        image = np.asarray(Image.open(pathToImage))
        image = image[:,:,0:3] # get rid of alpha value
        # get image dimension
        sizeX = image.shape[1]
        sizeY = image.shape[0]

        # get 1/0 image (0 = black, 1 = any color)
        image = np.array([[(1 if sum(image[j,i])>0 else 0) for i in range(sizeX)] for j in range(sizeY)])

        startIndex = 0
        for i in range(sizeX):
            empty = True
            for j in range(sizeY):
                if image[j,i]==0:                    
                    empty=False
            if empty == True:
                if i-startIndex == 0:
                    break
                pixelmap = image[:, startIndex:i+1]
                char = Character(i-startIndex+1, sizeY, pixelmap)
                key = self.charKeys[len(self.characters)]
                self.characters[key] = char
                startIndex = i+1

class Character:
    def __init__(self, width, height, pixelmap):
        self.width, self.height = width, height
        self.pixelmap = pixelmap

    def __str__(self):
        string = ""
        for j in range(self.height):
            for i in range(self.width):
                string += ("██" if self.pixelmap[j,i] != 0 else "░░")
            string += "\n"
        return string[:-1]

    def get_pixelmap(self):
        return self.pixelmap
        
class Text:
    def __init__(self, text, font):
        self.text = text
        self.font = font

    def __str__(self):
        pixelmap = self.get_pixelmap()
        string = ""
        for j in range(pixelmap.shape[0]):
            for i in range(pixelmap.shape[1]):
                string += ("██" if pixelmap[j,i] != 0 else "░░")
            string += "\n"
        return string[:-1]

    def get_pixelmap(self):
        pixelmap = self.font.characters[self.text[0]].get_pixelmap()
        for char in self.text[1:]:
            pixelmap = np.hstack([pixelmap, self.font.characters[char].get_pixelmap()])
        return pixelmap

if __name__ == "__main__":
    font = Font()
    font.readFromImage("./font2.png")
    #print(font.textToBitmap("HACKERSPACE"))
    hs = Text("HACKERSPACE!:)", font)
    print(hs)