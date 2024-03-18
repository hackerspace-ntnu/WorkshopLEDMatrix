# defines the following characters in a 'font'
# ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ1234567890.?!:-)([]{}
# Font is being read from an image; font3 was created in gimp, font and font2 were created in inkscape
# between each character is a collumn of white pixels
# this is used to detect the individual characters

import numpy as np
from PIL import Image

class Font:
    charKeys = r"ABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ1234567890.!?:-)([]{}"
    
    def __init__(self):
        self.characters = {}

    @classmethod
    def readFromImage(cls, path='./font3.png'):
        image = np.asarray(Image.open(path))
        image = image[:,:,0:3] # get rid of alpha value
        # get image dimension
        width = image.shape[1]
        height = image.shape[0]
        # get 1/0 image (0 = black, 1 = any color)
        image = np.array([[(1 if sum(image[j,i]) > 0 else 0) for i in range(width)] for j in range(height)])
        f = cls()
        startIndex = 0
        for i in range(width):
            empty = True
            for j in range(height):
                if image[j,i] == 0:                    
                    empty = False
            if empty == True:
                if i-startIndex == 0:
                    break
                pixelmap = image[:, startIndex:i+1]
                char = Character(i-startIndex+1, height, pixelmap)
                key = f.charKeys[len(f.characters)]
                f.characters[key] = char
                startIndex = i+1
        # add space character
        space_width = int((i+2)/len(f.charKeys))
        f.characters[' '] = Character(space_width, height, [[1 for _ in range(space_width)] for _ in range(height)])
        return f 

class Character:
    def __init__(self, width, height, pixelmap):
        self.width = width
        self.height = height
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
        self.text = text.upper() # only capitalized letters are supported
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
       # img = Image.fromarray(pixelmap*255, mode='1')
       # img_dim = img.size
       # print(np.asarray(img, dtype=np.int8))
       # final = np.asarray(img.resize((int(img_dim[0]*8/7),int(img_dim[1]*8/7))))
        #final_final = np.array([[(1 if sum(final[j,i])>0 else 0) for i in range(img_dim[0])] for j in range(img_dim[1])])
        return pixelmap


def create_pixelmap():
    image_path = "./font3.png"
    font_image = Image.open(image_path)
    font_image = font_image.convert("1")
    letter_width = 8
    letter_height = 12
    padding = 0

    for y in range(0, font_image.height, letter_height):
        for x in range(0, font_image.width, letter_width):
            
            letter_box = (x + padding, y+1, x + letter_width + padding, y + letter_height)
            letter_image = font_image.crop(letter_box)
            padding = padding + 1
            
            pixel_map = [(1 if pixel>1 else 0) for pixel in letter_image.getdata()]
            pixel_map = [pixel_map[i:i+letter_width] for i in range(0, len(pixel_map), letter_width)]
            
            print(f"Pixel Map for Letter at ({x}, {y}):")
            string = ""
            for row in pixel_map:
                for i in row:
                    string += ("██" if i != 0 else "░░")
                string += "\n"
            print(string)

if __name__ == "__main__":
    font = Font.readFromImage("./font3.png")
    hs = Text("vi har åpent", font)
    print(hs)