#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>

using Eigen::Map;
using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::VectorXd;
using Eigen::VectorXf;
using namespace std;

class ANN
{
private:
    MatrixXd Wh, W0;
    VectorXd bh, b0;
    int input_layer = 5 * 5, hidden_layer = 16, output_layer;
    double lr = 1.;
    double eps = 1e-6;

public:
    ANN(vector<MatrixXd *> &train_symbols)
    {
        double HI = 2., LO = -2.;
        double range = HI - LO;

        output_layer = train_symbols.size();

        Wh = MatrixXd::Random(hidden_layer, input_layer);
        Wh = (Wh + MatrixXd::Constant(hidden_layer, input_layer, 1.)) * range / 2.;
        Wh = (Wh + MatrixXd::Constant(hidden_layer, input_layer, LO));

        bh = VectorXd::Zero(hidden_layer);

        W0 = MatrixXd::Random(output_layer, hidden_layer);
        W0 = (W0 + MatrixXd::Constant(output_layer, hidden_layer, 1.)) * range / 2.;
        W0 = (W0 + MatrixXd::Constant(output_layer, hidden_layer, LO));

        b0 = VectorXd::Zero(output_layer);
        train(train_symbols);
    }

    double sigmoid(double x)
    {
        return 1.0 / (1.0 + exp(-x));
    }

    void train(vector<MatrixXd *> train_symbols)
    {
        MatrixXd EW0, EWh, EI0, EIh;
        do
        {
            int i = 0;
            for (std::vector<MatrixXd *>::iterator it = train_symbols.begin(); it != train_symbols.end(); ++it)
            {
                MatrixXd yd = MatrixXd::Zero(output_layer, 1);
                yd(i) = 1.;
                // Forward pass
                MatrixXd X = **it;
                Map<VectorXd> X_flat(X.data(), X.size());

                MatrixXd Vh = Wh * X_flat + bh;
                MatrixXd Zh = Vh.unaryExpr([](double x) { return 1.0 / (1.0 + exp(-x)); });
                MatrixXd U0 = W0 * Zh + b0;
                MatrixXd Y0 = U0.unaryExpr([](double x) { return 1.0 / (1.0 + exp(-x)); });

                // Backward propagation
                MatrixXd EA0 = Y0 - yd;
                EI0 = EA0.cwiseProduct(Y0.cwiseProduct((MatrixXd)(1 - Y0.array())));
                EW0 = EI0 * Zh.transpose();
                MatrixXd EAh = W0.transpose() * EI0;
                EIh = EAh.cwiseProduct(Zh.cwiseProduct((MatrixXd)(1 - Zh.array())));
                EWh = EIh * X_flat.transpose();

                //Update params
                W0 = (W0 - lr * EW0);
                Wh = (Wh - lr * EWh);
                b0 = (b0 + lr * EI0);
                bh = (bh + lr * EIh);

                i += 1;
            }
        } while (!(EW0.isZero(eps) && EWh.isZero(eps) && EI0.isZero(eps) && EIh.isZero(eps)));
    }

    VectorXd *test(MatrixXd &symbol)
    {
        MatrixXd X = symbol;
        Map<VectorXd> X_flat(X.data(), X.size());

        MatrixXd Vh = Wh * X_flat + bh;
        MatrixXd Zh = Vh.unaryExpr([](double x) { return 1.0 / (1.0 + exp(-x)); });
        MatrixXd U0 = W0 * Zh + b0;
        MatrixXd *Y0 = new MatrixXd(U0.rows(), U0.cols());
        *Y0 << U0.unaryExpr([](double x) { return 1.0 / (1.0 + exp(-x)); });
        return (VectorXd *)Y0;
    }
};

int main()
{
    srand((unsigned int)time(0));
    vector<MatrixXd *> train_symbols;
    MatrixXd m1, m2;
    m1 = MatrixXd::Random(5, 5);
    train_symbols.push_back(&m1);
    m2 = MatrixXd::Random(5, 5);
    train_symbols.push_back(&m2);
    ANN a(train_symbols);
    VectorXd *y2 = a.test(m2);
    cout << *y2 << '\n';
    VectorXd *y1 = a.test(m1);
    cout << *y1 << '\n';
}
