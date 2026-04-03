#include <iostream>
#include <math.h>
using namespace std;

class functionX {
    public:
        double resFunction(double valor) {
            double a = exp(3 * valor);
            double b = 4 * valor * valor;
            double c = a + b;
            double res = sqrt(c);
            return res;
        }
};

class functionX_derivate2 : public functionX{
    public:
        double resFunctionDerivate2(double x, double delta) {
            double num = -resFunction(x + 2*delta) + 16*resFunction(x + delta) - 30*resFunction(x) + 16*resFunction(x - delta) - resFunction(x - 2*delta);
            double results = num / (12*(delta*delta));
            return results;
        }
};

class execute {
    public:
        execute(double x, double deltax) {
            this->x = x;
            this->deltax = deltax;
        }

        void Calc() {
            double deltaAnterior = 0;
            double deltaAtual = deltax;
            double funcaoX = obj.resFunction(x);
            double funcaoDerivada = obj.resFunctionDerivate2(x, deltaAtual);
            cout << "f(x) = " << funcaoX << endl;
            cout << "f``(x) = " << funcaoDerivada << endl << endl;
            while(erro(deltaAtual, deltaAnterior) > 0.00001) {
                deltaAnterior = deltaAtual;
                deltaAtual *= 0.5;
                funcaoDerivada = obj.resFunctionDerivate2(x, deltaAtual);

                cout << "f``(x) = " << funcaoDerivada << endl;
                cout << "delta = " << deltaAtual << endl;
                cout << "Erro = " << erro(deltaAtual, deltaAnterior) << endl << endl;
            }
            
        }

        void mostra() {
            cout << "Valor de x = " << x << endl;
            cout << "Valor de delta = " << deltax << endl; 
        }

    private:
        double x;
        double deltax;
        functionX_derivate2 obj;

        double erro(double deltaAtual, double deltaAnterior) {
            double num  = obj.resFunctionDerivate2(x, deltaAtual) - obj.resFunctionDerivate2(x, deltaAnterior);
            double result = fabs(num / obj.resFunctionDerivate2(x, deltaAtual));
            return result;
        }
};

int main() {
    execute obj3(2.0, 0.5);
    obj3.mostra();
    obj3.Calc();
    return 0;
}