#include <pybind11/pybind11.h>
#include <matplot/matplot.h>
#include "AudioFile.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <pybind11/stl.h>

using namespace std;


int add(int i, int j) {
    return i + j + 200;
}

typedef vector<vector<vector<float>>> Image;
typedef vector<vector<float>> Matrix;

Image readPPM(const string& imageFilePath) {
    ifstream image(imageFilePath);
    string type, width, height, RGB;
    image >> type >> width >> height >> RGB;

    int W = stoi(width);
    int H = stoi(height);
    int C = 3; // ile mamy kana³ów koloru RGB (3 kanaly, red green i blue)

    Image img(H, vector<vector<float>>(W, vector<float>(C)));

    int r, g, b;
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            image >> r >> g >> b;
            img[h][w][0] = r;
            img[h][w][1] = g;
            img[h][w][2] = b;
        }
    }
    return img;
}

void savePPM(const Image& image, const string& outputFilePath) {
    ofstream newImage(outputFilePath);
    int H = image.size();
    int W = image[0].size();
    int C = image[0][0].size();

    newImage << "P3" << endl;
    newImage << W << " " << H << endl;
    newImage << "255" << endl;

    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            for (int c = 0; c < C; ++c) {
                newImage << static_cast<int>(image[h][w][c]);
                if (c < C - 1) {
                    newImage << " ";
                }
            }
            newImage << endl;
        }
    }
}

Image convolve2d(const Image& image, const Matrix& kernel) {
    int H = image.size();
    int W = image[0].size();
    int C = image[0][0].size();
    int h = kernel.size();
    int w = kernel[0].size();

    Image output(H - h + 1, Matrix(W - w + 1, vector<float>(C, 0.0f)));

    for (int i = 0; i <= H - h; ++i) {
        for (int j = 0; j <= W - w; ++j) {
            for (int kh = 0; kh < h; ++kh) {
                for (int kw = 0; kw < w; ++kw) {
                    for (int c = 0; c < C; ++c) {
                        output[i][j][c] += image[i + kh][j + kw][c] * kernel[kh][kw];
                    }
                }
            }
        }
    }

    return output;
}

void processImage(const std::string& inputFilePath, const std::string& outputFilePath, const Matrix& kernel) {
    Image image = readPPM(inputFilePath);

    Image filteredImage = convolve2d(image, kernel);
    savePPM(filteredImage, outputFilePath);
}


void image2d(const std::string& imageFilePath) {
    ifstream image;
    ofstream newImage;
    image.open(imageFilePath);
    newImage.open("newImage.ppm");
    string type = "", width = "", height = "", RGB = "";
    image >> type;
    image >> width;
    image >> height;
    image >> RGB;

    newImage << type << endl;
    newImage << width << " " << height << endl;
    newImage << RGB << endl;

    string red = "", green = "", blue = "";
    int r = 0, g = 0, b = 0;

    while (!image.eof()) {
        image >> red;
        image >> green;
        image >> blue;

        stringstream redstream(red);
        stringstream greenstream(green);
        stringstream bluestream(blue);

        redstream >> r;
        greenstream >> g;
        bluestream >> b;

        if (b + 50 >= 255)
        {
            b = 255;
        }
        else {
            b += 50;
        }

        newImage << r << " " << g << " " << b << endl;
    }
}



void test(double frequency) {

    std::vector<double> x;
    std::vector<double> y;

    for (int i = 0; i < 628; i++)
    {
        x.push_back(static_cast<double>(i) / 314);
        y.push_back(std::sin(3.14 * frequency * x[i]));
    }

    matplot::plot(x, y);
    matplot::show();

}

void test2(double frequency) {

    std::vector<double> x;
    std::vector<double> y;

    for (int i = 0; i < 628; i++)
    {
        x.push_back(static_cast<double>(i) / 314);
        y.push_back(std::cos(3.14 * frequency * x[i]));
    }

    matplot::plot(x, y);
    matplot::show();

}
void test3(double frequency) {

    std::vector<double> x;
    std::vector<double> y;

    for (int i = 0; i < 628; ++i) {
        x.push_back(static_cast<double>(i) / 100); // Skaluje x do zakresu od 0 do oko³o 6.28
        double value = std::sin(3.14 * frequency * x[i]);
        y.push_back(value >= 0 ? 1 : -1);
    }

    matplot::plot(x, y);
    matplot::show();

}

void test4(double frequency) {
    std::vector<double> x;
    std::vector<double> y;

    for (int i = 0; i < 628; ++i) {
        x.push_back(static_cast<double>(i) / 100); // Skaluje x do zakresu od 0 do oko³o 6.28 (2?)
        double t = x[i] * frequency;
        y.push_back(2.0 * (t - std::floor(t + 0.5))); // Generowanie fali pi³okszta³tnej
    }

    matplot::plot(x, y);
    matplot::show();
}


void visualize_audio(const std::string& audioFilePath) {
    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(audioFilePath);

    if (!loadedOK) {
        std::cerr << "Nie udalo siê zaladowaæ pliku audio!" << std::endl;
        return;
    }


    int numSamples = audioFile.getNumSamplesPerChannel();
    int numChannels = audioFile.getNumChannels();

    if (numSamples <= 0) {
        std::cerr << "Plik audio nie zawiera probek." << std::endl;
        return;
    }

    std::vector<double> audioData(audioFile.samples[0].begin(), audioFile.samples[0].end());


    std::vector<double> x(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        x[i] = static_cast<double>(i) / audioFile.getSampleRate();
    }

    matplot::plot(x, audioData);
    matplot::show();
}



namespace py = pybind11;

PYBIND11_MODULE(cmakes, m) {
    m.def("add", &add, "add");
    m.def("matplot", &test, "sinus(x)");
    m.def("matplot2", &test2, "cosunus(x)");
    m.def("matplot3", &test3, "cosunus(x)");
    m.def("matplot4", &test4, "cosunus(x)");
    m.def("matplot5", &visualize_audio, "cosunus(x)");
    m.def("poka", &image2d, "image2dPorcessing");
    m.def("processImage", &processImage, "Function to process an image");
}