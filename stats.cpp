#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "stats.hpp"

// Statistics class implementation - base class
// public member functions - accessor interfaces

void Statistics::displayData() const
{
    display();
}

unsigned Statistics::getSize() const
{
    return size();
}

float Statistics::getMean() const
{
    return mean();
}

float Statistics::getVar() const
{
    return var();
}

float Statistics::getStd() const
{
    return std();
}

float Statistics::getMin() const
{
    return min();
}

float Statistics::getMax() const
{
    return max();
}

float Statistics::getMedian() const
{
    return median();
}

float Statistics::getMad() const
{
    return mad();
}

float Statistics::retrieveItem(unsigned idx) const
{
    return x[idx];
}

// protected member functions
void Statistics::display() const
{
    if (n > 100) {
        std::cerr << "\n array greater than 100 items";
        return;
    }
    for (int i = 0; i < n; i++) {
        if ((i) % 8 == 0) std::cout << '\n';
        std::cout << std::setw(8) << x[i];
    }
    if (n % 8 == 0) std::cout << "\n\n";
    else std::cout << '\n';
}

unsigned Statistics::size() const
{
    return n;
}

float Statistics::mean() const
{
    float avg = 0;
    for (int i = 0; i < n; i++) avg = (i * avg + x[i]) / (i + 1);
    return avg;
}

float Statistics::min() const
{
    return rank(0);
}

float Statistics::max() const
{
    return rank(n - 1);
}

float Statistics::median() const
{
    int k = n / 2;
    if (n % 2 == 0) {
        float p = rank(n / 2 - 1);
        float q = rank(n / 2);
        return (p + q) / 2.0;
    }
    else
        return rank(n/2);
}

float Statistics::median(const std::vector<float>& x) const
{
    int k = n / 2;
    if (k % n == 0) {
        float p = rank(x, n / 2 - 1);
        float q = rank(x, n / 2);
        return (p + q) / 2.0;
    }
    else
        return rank(x, n/2);
}

float Statistics::mad() const
{
    std::vector<float> tmp;
    float xmed = median();
    for (int i = 0; i < n; i++) tmp.push_back( std::abs(xmed - x[i]) );
    return median(tmp);
}

float Statistics::rank(unsigned k) const
{
    int i, j, left = 0, right = n - 1;
    float pivot;
    // initialize index vector to natural order (zero indexed)
    std::vector<int> idx;
    for (i = 0; i < n; i++) idx.push_back(i);
    while (left < right) {
        pivot = x[idx[k]];
        i = left;
        j = right;
        do {
            while (x[idx[i]] < pivot) i++;
            while (pivot < x[idx[j]]) j--;
            if (i <= j) {
                unsigned idxtmp = idx[i];
                idx[i] = idx[j];
                idx[j] = idxtmp;
                i++;
                j--;
            }
        } while (i <= j);
        if (j < k) left = i;
        if (k < i) right = j;
    }
    return x[idx[k]];
}

float Statistics::rank(const std::vector<float>& x, unsigned k) const
{
    int i, j, left = 0, right = n - 1;
    float pivot;
    // initialize index vector to natural order (zero indexed)
    std::vector<int> idx;
    for (i = 0; i < n; i++) idx.push_back(i);
    while (left < right) {
        pivot = x[idx[k]];
        i = left;
        j = right;
        do {
            while (x[idx[i]] < pivot) i++;
            while (pivot < x[idx[j]]) j--;
            if (i <= j) {
                unsigned idxtmp = idx[i];
                idx[i] = idx[j];
                idx[j] = idxtmp;
                i++;
                j--;
            }
        } while (i <= j);
        if (j < k) left = i;
        if (k < i) right = j;
    }
    return x[idx[k]];
}

// Population derived class implementation
// protected methods
// default constructor accesses this method
void Population::load()
{
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        float r = n * (rand() / NORMALIZER);
        x.push_back(r);
    }
}
// copy constructor accesses this method
void Population::copy(const Population& p)
{
    n = p.getSize();
    for (unsigned i = 0; i < n; i++) x.push_back(p.x[i]);
}


float Population::var() const
{
    float var = 0, xbar = mean();
    for (int i = 0; i < n; i++) var = (i * var + (x[i] - xbar) * (x[i] - xbar)) / (i + 1);
    return var;
}

float Population::std() const
{
    return sqrt(var());
}

// Sample derived class implementation
float Sample::var() const
{
    return (float(n) / float(n - 1)) * Population::var();
}

float Sample::std() const
{
    return sqrt(var());
}

void RandomSample::load(unsigned u, const Population& p)
{
    // n = u;
    std::vector<int> idx(p.getSize());
    for (unsigned i = 0; i < p.getSize(); i++) idx[i] = i;
    for (unsigned i = 0; i < u; i++) {
        unsigned rndidx  = p.getSize() * (rand() / NORMALIZER);
        unsigned tmp = idx[i];
        idx[i] = idx[rndidx];
        idx[rndidx] = tmp;
    }
    for (unsigned i = 0; i < u; i++) x.push_back(p.retrieveItem(idx[i]));
}

// FreqeuncyAnalyzer derived class implementation
// private methods
void FrequencyAnalyzer::load(unsigned n)
{
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        int item = int(n * (rand() /  NORMALIZER));
        data.push_back(item);
        x.push_back(0);
    }
    count_frequencies();
    display_table();
}

void FrequencyAnalyzer::count_frequencies()
{
    for (unsigned i = 0; i < n; i++) x[data[i]] = x[data[i]] + 1;
}

void FrequencyAnalyzer::display_table() const
{
    for (unsigned i = 0; i < n; i++)
        std::cout << "\n x[" << i << "] = " << x[i];
}

// TextAnalyzer derived class implementation
// constructor definition
TextAnalyzer::TextAnalyzer(const char * textfile)
{
    load(textfile);
}

// public methods - html output
void TextAnalyzer::printReport() const
{
    std::cout << std::setprecision(4);
    std::cout << std::fixed;
    std::cout << "\n <div>variance = " << getVar() << "</div>";
    std::cout << "\n <div>standard deviation = " << getStd() << "</div>";
    std::cout << "\n <div>minimum = " << getMin() << "</div>";
    std::cout << "\n <div>median = " << getMedian() << "</div>";
    std::cout << "\n <div>maximum = " << getMax() << "</div>";
    std::cout << "\n <div>median absolute deviation = " << getMad() << "</div>";
    std::cout << "\n <div>coefficient of variation = " << getCov() << "</div>";
    // std::cout << "\n index of coincidence = " << getIoc();
}
// private data
const float TextAnalyzer::english_data[C] = {.0781,.0128,.0293,.0411,.1305,
.0288,.0139,.0565,.0677,.0023,.0042,.036,.0262,.0728,.0821,.0215,.0014,.0664,
.0646,.0902,.0277,.01,.0149,.003,.0151,.0009};

// private methods
void TextAnalyzer::load(const char* textfile)
{
    n = C;
    charcount = n;
    x.assign(english_data, english_data + C);
    std::cout << "\n <div>english</div>";
    printReport();
    int count = 0;
    for (int i = 0; i < C; i++) data.push_back(0);
    std::string linebuf;
    std::ifstream infile;
    infile.open(textfile, std::ios::in);
    getline(infile, linebuf);
    while (!infile.eof()) {
        int buflen = linebuf.length();
        for (int i = 0; i < buflen; i++) {
            if (linebuf[i] >= 'a' && linebuf[i] <= 'z') {
                int idx = linebuf[i] - 97;
                (data[idx])++;
                count++;
            }
            else if (linebuf[i] >= 'A' && linebuf[i] <= 'Z') {
                int idx = linebuf[i] - 65;
                (data[idx])++;
                count++;
            }
        }
        getline(infile, linebuf);
    }
    infile.close();
    charcount = count;
    for (int i = 0; i < C; i++) x[i] = float(data[i]) / float(count);
}

void TextAnalyzer::display_table() const
{
    int engidx[C];
    int txtidx[C];
    std::vector<float> tmp(english_data, english_data + C);
    select_idx(tmp, engidx);
    select_idx(x, txtidx);
    std::cout << std::setprecision(4);
    std::cout << std::fixed;
    std::cout << "\n <style>td { text-align: center; }</style>";
    std::cout << "\n <table class = \"right-nowrap\">";
    std::cout << "\n <tr><th>letter</th><th>english</th><th>textfile</th>";
    std::cout << "<th></th>";
    std::cout << "<th>letter</th><th>english</th><th>letter</th><th>textfile</th></tr>";
    for (int i = 0; i < n; i++) {
        char ch = char(65 + i);
        std::cout << "\n <tr><td>" << ch << "</td><td>" << english_data[i] << "</td>";
        std::cout << "<td>" << x[i] << "</td>";
        std::cout << "<td>. . .</td>";
        ch = char(65 + engidx[i]);
        std::cout << "<td>" << ch << "</td><td>" << english_data[engidx[i]] << "</td>";
        ch = char(65 + txtidx[i]);
        std::cout << "<td>" << ch << "</td><td>" << x[txtidx[i]] << "</td></tr>";
    }
    std::cout << "\n </table>";
}

float TextAnalyzer::getCov() const
{
    return coefficient_of_variation();
}

float TextAnalyzer::getIoc() const
{
    return index_of_coincidence();
}

float TextAnalyzer::getCorr() const
{
    return english_correlation();
}

// private methods - TextAnalyzer

float TextAnalyzer::coefficient_of_variation() const
{
    return std() / mean();
}

float TextAnalyzer::index_of_coincidence() const
{
    float ioc = 0, nfloat = float(charcount), cfloat = float(C);
    for (int i = 0; i < C; i++) ioc += float(data[i]) * (data[i] - 1);
    ioc /= (nfloat * (nfloat - 1)) / cfloat;
    return ioc;
}

float TextAnalyzer::english_correlation() const
{
    float corr = 0;
    for (int i = 0; i < n; i++) corr += english_data[i] * x[i];
    return corr;
}

void TextAnalyzer::select_idx(const std::vector<float>& a, int idx []) const
{
    for (int i = 0; i < n; i++) idx[i] = i;
    for (int j = 0; j < n - 1; j++) {
        int imin = j;
        for(int i = j + 1; i < n; i++) if (a[idx[i]] > a[idx[imin]]) imin = i;
        if (imin != j) {
            int tmp = idx[j];
            idx[j] = idx[imin];
            idx[imin] = tmp;
        }
    }
}
