#include "my_matrix.h"

my_matrix::my_matrix()
{

}

void my_matrix::new_size(int row, int col)
{
    m.resize(row);
    for (int i = 0; i < m.size(); ++i)
        m[i].resize(col);
    for (int i = 0; i < m.size(); ++i)
        for (int j = 0; j < m[i].size(); ++j)
            m[i][j] = 0;
}

void my_matrix::add(int i, int j, double value)
{
    m[i][j] = value;
}

int my_matrix::size_column()
{
    return m[0].size();
}

int my_matrix::size_row()
{
    return m.size();
}

void my_matrix::print()
{
//    qDebug() << "вывожу";
    QString s;

    for (int i = 0; i < size_row(); ++i) {
        s = "";
        for (int j = 0; j < size_column(); ++j)
            s += (j == 0 ? "" : " ") + QString("%1").arg(m[i][j]);
//        qDebug() << s;
    }
//    qDebug() << "D = " << D(m);
}

double my_matrix::D(QVector<QVector<double>> matrix)
{
    if ((matrix.size() == 0) || (matrix.size() == 1)
            || ((matrix.size() > 0) && (matrix.size() != matrix[0].size()))) {
        return -1;
    }

    if (matrix.size() > 2) {
        int sign = 1;
        double res = 0;
        for (int i = 0; i < matrix.size(); ++i) {
//            qDebug() << (i == 0 ? "" : " + ") << sign*matrix[0][i] << "*" << /*D(*/opr(matrix, 0, i)/*)*/;
            res  += sign*matrix[0][i]*D(opr(matrix, 0, i));
            sign = -sign;
        }
        return res;
    }

    if (matrix.size() == 2)
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];

    return -1;
}

QVector<QVector<double> > my_matrix::opr(const QVector<QVector<double> > &matrix, int _i, int _j)
{
    QVector<QVector<double> > mat;
    for (int i = 0; i < matrix.size(); ++i) {
        QVector<double> tmp;
        for (int j = 0; j < matrix.size(); ++j)
            if ((i != _i) && (j != _j))
                tmp.push_back(matrix[i][j]);
        if (tmp.size() != 0) {
            mat.push_back(tmp);
//            qDebug() << tmp;
        }
    }
    return mat;
}

QVector<double> my_matrix::kramer(QVector<double> B)
{
    // нахожу главный определитель
    double gD = D(m);
    if (gD == 0) {
//        qDebug() << "Беда, не могу решить";
        return QVector<double>();
    }
//    if (m.size() != B.size())
//        qDebug() << "размер вектора B должен быть равен матрице";

    QVector<double> korni;

    for (int i = 0; i < m.size(); ++i) {
        QVector<QVector<double> > tmp = m;

        for (int j = 0; j < m.size(); ++j)
            tmp[j][i] = B[j];

        korni.push_back(D(tmp)/gD);
//        alg_dop(tmp);
//        transp(tmp);
    }
    return korni;
}

QVector<QVector<double> > my_matrix::minor(const QVector<QVector<double> > &matrix)
{
    QVector<QVector<double> > min = matrix;

    for (int i = 0; i < matrix.size(); ++i)
        for (int j = 0; j < matrix[i].size(); ++j)
            min[i][j] = D(opr(matrix, i, j));

    return min;
}

void my_matrix::alg_dop(QVector<QVector<double> > &matrix)
{
    for (int i = 0; i < matrix.size(); ++i) {
        int sign = 1;
        for (int j = 0; j < matrix.size(); ++j) {
            matrix[i][j] *= sign;
            sign *= -1;
        }
    }
}

void my_matrix::transp(QVector<QVector<double> > &matrix)
{
    QVector<QVector<double> > copy = matrix;

    for (int i = 0; i < matrix.size(); ++i)
        for (int j = 0; j < matrix.size(); ++j)
            copy[j][i] = matrix[i][j];
}

