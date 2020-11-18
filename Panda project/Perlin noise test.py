from panda3d.core import PerlinNoise3, PNMImage, Filename

import datetime

print ("Generating noise...")

# Panda's Perlin Noise class doesn't seem to handle multiple noise-levels
#  by itself, so we're generating each level individually, and combining them
#  ourselves.
noise = PerlinNoise3(100, 100, 10, 1024, datetime.datetime.now().toordinal())
noise2 = PerlinNoise3(70, 70, 5, 1024, datetime.datetime.now().toordinal())
noise3 = PerlinNoise3(40, 40, 2, 1024, datetime.datetime.now().toordinal())
noise4 = PerlinNoise3(10, 10, 0.2, 1024, datetime.datetime.now().toordinal())

# A PNMImage into which to write our noise
image = PNMImage(1024, 1024, 3)
# The number of layers--i.e. images--to generate and save
numLayers = 1024

# For each layer/image, generate noise at each level and combine it all,
#  then write the result to file.
for layer in range(numLayers):
    for i in range(1024):
        for j in range(1024):
            # Note that each layer is scaled and offset to adjust its prominence
            #  in the final image
            noiseVal = (noise.noise(i, j, layer) + 1 + noise2.noise(i, j, layer)*0.5 + 0.5 + noise3.noise(i, j, layer)*0.3 + 0.3 + noise4.noise(i, j, layer)*0.1 + 0.1)/2.9

            # Write the pixel to the image
            image.setXel(i, j, noiseVal)

    # Determine the file-name, with leading zeroes.
    layerStr = ""
    if layer < 10:
        layerStr += "000"
    elif layer < 100:
        layerStr += "00"
    elif layer < 1000:
        layerStr += "0"
    layerStr += str(layer)

    # Write the image!
    image.write(Filename("images/noise_" + layerStr + ".png"))

print ("Done!")