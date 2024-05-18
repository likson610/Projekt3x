import sys
sys.path.append('out/build/x64-debug')
import numpy as np

import cmakes

import numpy as np

kernel = np.array([
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722]
], dtype=np.float32)

kernel_list = kernel.tolist()


cmakes.processImage("monument.ppm", "newImages.ppm", kernel_list)