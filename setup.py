import sys
sys.path.append('out/build/x64-debug')

import cmakes

import numpy as np



kernel = np.array([
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722],
    [0.2126, 0.7152, 0.0722]
], dtype=np.float32)

kernel_list = kernel.tolist()


highpass_kernel = np.array([
    -0.0329587, -0.03509184, -0.03702822, -0.03871797, -0.0401169,
    -0.04118868, -0.04190512, -0.04224642, -0.04220262, -0.04176578,
    -0.04093459, -0.03971685, -0.03812693, -0.03618525, -0.03391798,
    -0.0313554,  0.874417,   -0.0313554,  -0.03391798, -0.03618525,
    -0.03812693, -0.03971685, -0.04093459, -0.04176578, -0.04220262,
    -0.04224642, -0.04190512, -0.04118868, -0.0401169,  -0.03871797,
    -0.03702822, -0.03509184, -0.0329587
])

black_white_filter_kernel = np.array([
    [0.333, 0.333, 0.333],
    [0.333, 0.333, 0.333],
    [0.333, 0.333, 0.333]
], dtype=np.float32)

kernel_list_black = kernel.tolist()


kernel_hipass = highpass_kernel.tolist()

cmakes.AutoCorrelation("square.wav")

#cmakes.processImage("monument.ppm", "newMomnument.ppm",kernel_list)

#cmakes.filterAudio("jezyk.wav", "filteredHipass.wav", kernel_hipass)

#cmakes.filterAudioLow("jezyk.wav","outfilewaff.wav")

#cmakes.processImage("monument.ppm", "newImages.ppm", kernel_list)