#include <pybind11/pybind11.h>
#include <matplot/matplot.h>
#include "AudioFile.h"
#include <stdio.h>


int add(int i, int j) {
    return i + j+200;
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
    // Utworzenie obiektu AudioFile i za³adowanie pliku audio
    AudioFile<double> audioFile;
    bool loadedOK = audioFile.load(audioFilePath);

    if (!loadedOK) {
        std::cerr << "Nie udalo siê zaladowaæ pliku audio!" << std::endl;
        return;
    }

    // Pobranie liczby próbek i liczby kana³ów
    int numSamples = audioFile.getNumSamplesPerChannel();
    int numChannels = audioFile.getNumChannels();

    if (numSamples <= 0) {
        std::cerr << "Plik audio nie zawiera probek." << std::endl;
        return;
    }

    // Wyodrêbnienie danych dla pierwszego kana³u
    std::vector<double> audioData(audioFile.samples[0].begin(), audioFile.samples[0].end());

    // Przygotowanie osi x
    std::vector<double> x(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        x[i] = static_cast<double>(i) / audioFile.getSampleRate();
    }

    // Rysowanie sygna³u audio
    matplot::plot(x, audioData);
    // Wyœwietlenie wykresu
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

}