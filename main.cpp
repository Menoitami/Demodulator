#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <complex>
#include <list>
#include <bitset>
#include <cmath>
#include <concepts>

const float pi = std::acos(-1);

template <typename T>
concept Container = requires(T a, typename T::value_type val) {
    { a.push_back(val) };
    { a.begin() };
    { a.end() };
};


std::vector<int> toBinary(const int& number, const int& bits) {
    std::vector<int> binary(bits);
    for (int i = 0; i < bits; ++i) {
        binary[bits - i - 1] = (number >> i) & 1;
    }
    return binary;
}

int binaryToGray(int num) {
    return num ^ (num >> 1); 
}

int graydecode(unsigned int gray) {
    unsigned int bin;
    for (bin = 0; gray; gray >>= 1) {
        bin ^= gray;
    }
    return bin;
}

std::list<double> cumsum(const std::list<double>& data){

        std::list<double> cumsum;
        double sum=0;
        for (auto val : data){
            sum += val;
            cumsum.push_back(sum);
        }
        return cumsum;
}

template <typename ComplexType>
double sum_abs(const std::vector<ComplexType>& data) {
    double sum = 0;
    for (const auto& val : data) {
        sum += std::abs(val);
    }
    return sum;
}

template <typename ComplexType>
std::list<double> angle(const std::list<ComplexType>& data) {
    std::list<double> angles;
    for (const auto& val : data) {
        angles.push_back(std::arg(val));
    }
    return angles;
}

template <typename ComplexType>
std::list<ComplexType> pskmod(const std::vector<int>& data, double M, double phaseOffset = 0.0, bool useGrayCoding = true) {
    std::list<ComplexType> modulatedData;
    int numSymbols = data.size();
    double phaseStep = 2.0 * pi / M;

    for (int i = 0; i < numSymbols; ++i) {
        int symbol = data[i];
        if (symbol >= M) {
            throw std::invalid_argument("Symbol index out of range");
        }

        if (useGrayCoding) {
            symbol = binaryToGray(symbol);
        }

        double phase = phaseOffset + symbol * phaseStep;
        modulatedData.emplace_back(std::cos(phase), std::sin(phase));
    }

    return modulatedData;
}

template <typename ComplexType>
std::list<int> pskDemodGray(const std::list<ComplexType>& iq_cleared, int M, double phaseOffset) {
    std::list<int> demodulatedSymbols;
    double angleStep = 2 * pi / M;

    for (const auto& iq : iq_cleared) {
        double phase = std::arg(iq) - phaseOffset;
        if (phase < 0) phase += 2 * pi;
        int symbolIndex = static_cast<int>(std::round(phase / angleStep)) % M;
        symbolIndex = graydecode(symbolIndex);
        demodulatedSymbols.push_back(symbolIndex);
    }

    return demodulatedSymbols;
}


template <Container ContainerType>
ContainerType Demodulate(const double M, const std::vector<int>& preamb, const std::vector<std::complex<double>>& complexData) {
    unsigned int n = complexData.size();
    unsigned int pLen = preamb.size();

    std::list<std::complex<double>> iq_preamb = pskmod<std::complex<double>>(preamb, M, pi / M, true);

    std::list<std::complex<double>> ph_deltas;
    auto dataIt = complexData.begin();
    auto nextIt = std::next(dataIt);

    for (; nextIt != complexData.end(); ++dataIt, ++nextIt) {
        ph_deltas.push_back(std::pow(std::pow(*nextIt, M) * std::conj(std::pow(*dataIt, M)), 1.0 / M));
    }

    std::list<double> ph_acc{0};
    std::list<double> angles = angle(ph_deltas);
    std::list<double> csum = cumsum(angles);
    ph_acc.insert(ph_acc.end(), csum.begin(), csum.end());

    std::list<std::complex<double>> iq_dedop;
    auto ph_accIt = ph_acc.begin();
    for (const auto& data : complexData) {
        iq_dedop.push_back(data * std::exp(std::complex<double>(0.0, -(*ph_accIt))));
        ++ph_accIt;
    }

    std::complex<double> sum_prod = 0;
    auto iq_dedopIt = iq_dedop.begin();
    auto iq_preambIt = iq_preamb.begin();

    for (size_t i = 0; i < pLen; ++i, ++iq_dedopIt, ++iq_preambIt) {
        sum_prod += (*iq_dedopIt) * std::conj(*iq_preambIt);
    }

    double sum_abs_val = sum_abs(std::vector<std::complex<double>>(iq_dedop.begin(), std::next(iq_dedop.begin(), pLen)));
    std::complex<double> initial_phase = sum_prod / static_cast<std::complex<double>>(sum_abs_val);

    std::cout << "Initial phase: " << initial_phase << std::endl;

    // reuse iq_dedop to store iq_cleared for minimize memory
    for (auto& val : iq_dedop) {
        val *= std::conj(initial_phase);
    }

    
    std::list<int> demodulatedSymbols = pskDemodGray(iq_dedop, M, pi / M);

    ContainerType output_data;
    for (auto i = std::next(demodulatedSymbols.begin(), pLen); i != demodulatedSymbols.end(); ++i) {
        auto bits = toBinary(*i, std::log2(M));
        output_data.insert(output_data.end(), bits.begin(), bits.end());
    }

    return output_data;
}



std::vector<int> binaryToVector(const std::string& binaryStr) {
    std::vector<int> result;
    for (size_t i = 0; i < binaryStr.size(); i += 2) {
        std::string byte = binaryStr.substr(i, 2);
        result.push_back(std::stoi(byte, nullptr, 2));
    }
    return result;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <mod_order> <preamble_binary> <filename>" << std::endl;
        return 1;
    }

    double mod_order = std::stod(argv[1]);

    std::string preamble_binary = argv[2];
    std::vector<int> preamb = binaryToVector(preamble_binary);

    std::ifstream file(argv[3], std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Can't open the file: " << argv[3] << std::endl;
        return 1;
    }

    file.seekg(0, std::ios::end);
    unsigned long int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<int16_t> data(file_size / sizeof(int16_t));

    if (file.read(reinterpret_cast<char*>(data.data()), file_size)) {
        std::cout << "File is read: " << data.size() << std::endl;
    } else {
        std::cerr << "Error reading the file" << std::endl;
    }
    file.close();

    std::vector<std::complex<double>> complexData(data.size() / 2);
    for (unsigned long int i = 0; i < complexData.size(); ++i) {
        complexData[i] = std::complex<double>(data[2 * i], data[2 * i + 1]);
    }


    std::list<double> result = Demodulate<std::list<double>>(mod_order, preamb, complexData); //u can change return container and the tyoe of data in it

    std::cout<<"Demodulated data: ";
    for (auto val:result){
        std::cout<<val;

    }
    std::cout<<"\n";

    return 0;
}
