#include "image_io.hpp"
#include "filters.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

static void print_help(const char* prog) {
    std::cerr << "Uso:\n"
              << "  " << prog << " algoritmo1 --input <arquivo.bmp> --threshold <valor> [--output <saida.bmp>]\n"
              << "  " << prog << " algoritmo2 --input <arquivo.bmp> [--tolerance <valor>] [--output <saida.bmp>]\n"
              << "  " << prog << " --help\n\n"
              << "Opcoes:\n"
              << "  --input       Caminho da imagem BMP de entrada (obrigatorio)\n"
              << "  --threshold   Valor de threshold para algoritmo1 (float, obrigatorio p/ alg1)\n"
              << "  --tolerance   Tolerancia para algoritmo2 (float, padrao: 0.0001)\n"
              << "  --output      Caminho da imagem BMP de saida (padrao: output.bmp)\n"
              << "  --help        Exibir esta mensagem\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "--help" || command == "-h") {
        print_help(argv[0]);
        return 0;
    }

    std::string input_path;
    std::string output_path = "output.bmp";
    double threshold = 0.5;
    double tolerance = 0.0001;
    bool has_threshold = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--input" || arg == "-i") && i + 1 < argc) {
            input_path = argv[++i];
        } else if ((arg == "--output" || arg == "-o") && i + 1 < argc) {
            output_path = argv[++i];
        } else if (arg == "--threshold" && i + 1 < argc) {
            threshold = std::atof(argv[++i]);
            has_threshold = true;
        } else if (arg == "--tolerance" && i + 1 < argc) {
            tolerance = std::atof(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            print_help(argv[0]);
            return 0;
        } else {
            std::cerr << "Argumento desconhecido: " << arg << "\n";
            print_help(argv[0]);
            return 1;
        }
    }

    if (input_path.empty()) {
        std::cerr << "Erro: --input e obrigatorio.\n";
        print_help(argv[0]);
        return 1;
    }

    try {
        std::cerr << "Carregando imagem: " << input_path << "\n";
        auto img = tarefa1b::load_bmp(input_path);
        std::cerr << "Imagem carregada: " << img.cols() << "x" << img.rows() << "\n";

        tarefa1b::Matrix<double> result;

        if (command == "algoritmo1") {
            if (!has_threshold) {
                std::cerr << "Aviso: --threshold nao fornecido, usando padrao 0.5\n";
            }
            std::cerr << "Executando algoritmo1 com threshold=" << threshold << "\n";
            result = tarefa1b::algoritmo1(img, threshold);
        } else if (command == "algoritmo2") {
            std::cerr << "Executando algoritmo2 com tolerance=" << tolerance << "\n";
            result = tarefa1b::algoritmo2(img, tolerance);
        } else {
            std::cerr << "Comando desconhecido: " << command << "\n";
            print_help(argv[0]);
            return 1;
        }

        tarefa1b::save_bmp(output_path, result);
        std::cerr << "Imagem salva em: " << output_path << "\n";
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }
}
