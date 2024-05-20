#include <pybind11/pybind11.h>
#include <matplot/matplot.h>
#include "AudioFile.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <pybind11/stl.h>
#include <cmath>

using namespace std;


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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

void processImage(const string& inputFilePath, const string& outputFilePath, const Matrix& kernel) {
    Image image = readPPM(inputFilePath);

    Image filteredImage = convolve2d(image, kernel);
    savePPM(filteredImage, outputFilePath);
}


void image2d(const string& imageFilePath) {
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

    vector<double> x;
    vector<double> y;

    for (int i = 0; i < 628; i++)
    {
        x.push_back(static_cast<double>(i) / 314);
        y.push_back(sin(3.14 * frequency * x[i]));
    }

    matplot::plot(x, y);
    matplot::show();

}

void test2(double frequency) {

    vector<double> x;
    vector<double> y;

    for (int i = 0; i < 628; i++)
    {
        x.push_back(static_cast<double>(i) / 314);
        y.push_back(cos(3.14 * frequency * x[i]));
    }

    matplot::plot(x, y);
    matplot::show();

}
void test3(double frequency) {

   vector<double> x;
    vector<double> y;

    for (int i = 0; i < 628; ++i) {
        x.push_back(static_cast<double>(i) / 100); // Skaluje x do zakresu od 0 do oko³o 6.28
        double value = sin(3.14 * frequency * x[i]);
        y.push_back(value >= 0 ? 1 : -1);
    }

    matplot::plot(x, y);
    matplot::show();

}

void test4(double frequency) {
    vector<double> x;
    vector<double> y;

    for (int i = 0; i < 628; ++i) {
        x.push_back(static_cast<double>(i) / 100); // Skaluje x do zakresu od 0 do oko³o 2*Pi
        double t = x[i] * frequency;
        y.push_back(2.0 * (t - floor(t + 0.5))); // Generowanie fali pi³okszta³tnej
    }

    matplot::plot(x, y);
    matplot::show();
}


void visualize_audio(const string& audioFilePath) {
    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(audioFilePath);

    if (!loadedOK) {
        cerr << "Nie udalo siê zaladowaæ pliku audio!" << endl;
        return;
    }


    int numSamples = audioFile.getNumSamplesPerChannel();
    int numChannels = audioFile.getNumChannels();

    if (numSamples <= 0) {
        cerr << "Plik audio nie zawiera probek." << endl;
        return;
    }

    vector<double> audioData(audioFile.samples[0].begin(), audioFile.samples[0].end());


    vector<double> x(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        x[i] = static_cast<double>(i) / audioFile.getSampleRate();
    }

    matplot::plot(x, audioData);
    matplot::show();
}



    vector<float> convolve(const vector<float>& signal, const vector<float>& kernel) {
    int signalSize = signal.size();
    int kernelSize = kernel.size(); 
    int outputSize = signalSize + kernelSize - 1;
    vector<float> output(outputSize, 0.0f);

    for (int i = 0; i < outputSize; ++i) {
        int kMin = (i >= kernelSize - 1) ? i - (kernelSize - 1) : 0;
        int kMax = (i < signalSize - 1) ? i : signalSize - 1;
        for (int j = kMin; j <= kMax; ++j) {
            output[i] += signal[j] * kernel[i - j];
        }
    }

    int offset = (kernelSize - 1) / 2; 
    vector<float> trimmedOutput(signalSize);
    for (int i = 0; i < signalSize; ++i) {
        trimmedOutput[i] = output[i + offset];
    }

    return trimmedOutput;
}


void processAudioFile(const string& inputFilename, const string& outputFilename, const vector<float> kernel) {
    AudioFile<float> audioFile;
    if (!audioFile.load(inputFilename)) {
        cerr << "Blad przy probie otwarcia pliku: " << inputFilename << endl;
        return;
    }

    int sampleRate = audioFile.getSampleRate(); 
    int numChannels = audioFile.getNumChannels(); 
    vector<vector<float>> audio;


    if (numChannels == 1) {
        for (int i = 0; i < audioFile.getNumSamplesPerChannel(); ++i) {
            audio.push_back({ audioFile.samples[0][i] });
        }
    }
    else if (numChannels == 2) {
        for (int c = 0; c < numChannels; ++c) {
            vector<float> channelData;
            for (int i = 0; i < audioFile.getNumSamplesPerChannel(); ++i) {
                channelData.push_back(audioFile.samples[c][i]);
            }
            audio.push_back(channelData);
        }
    }
    else {
        cerr << "Za duzo kana³ow: " << numChannels << endl;
        return;
    }

   
    for (int c = 0; c < numChannels; ++c) {
        vector<float> filteredAudio = convolve(audio[c], kernel);

        for (int i = 0; i < filteredAudio.size(); ++i) {
            audioFile.samples[c][i] = filteredAudio[i];
        }
    }

    audioFile.save(outputFilename);

    cout << "Zapisano plik: " << outputFilename << endl;
}


void AutoCorrelation(const string& filePath) {
    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(filePath);

    if (!loadedOK) {
        cerr << "Nie udalo sie zaladowac pliku audio!" << endl;
        return;
    }

    int numSamples = audioFile.getNumSamplesPerChannel();
    int numChannels = audioFile.getNumChannels();

    if (numSamples <= 0) {
        cerr << "Plik audio nie zawiera probek." << endl;
        return;
    }

    vector<double> signal = audioFile.samples[0];
    size_t N = signal.size();

    double mean = accumulate(signal.begin(), signal.end(), 0.0) / N;

    for (auto& sample : signal) {
        sample -= mean;
    }

    vector<double> autocorrelation(N, 0.0);
    for (size_t lag = 0; lag < N; ++lag) {
        for (size_t i = 0; i < N - lag; ++i) {
            autocorrelation[lag] += signal[i] * signal[i + lag];
        }
    }

    for (size_t lag = 0; lag < N; ++lag) {
        autocorrelation[lag] /= N - lag;
    }

    for (size_t i = 0; i < autocorrelation.size(); ++i) {
      cout << "Lag " << i << ": " << autocorrelation[i] << endl;
    }

    auto lags = matplot::linspace(0, autocorrelation.size() - 1, autocorrelation.size());
    matplot::plot(lags, autocorrelation);
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
    m.def("filterAudio", &processAudioFile, "Function to process an image");
    m.def("AutoCorrelation", &AutoCorrelation, "Function toAutoCorrelation");
}