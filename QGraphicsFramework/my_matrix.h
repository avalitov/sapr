#ifndef MY_MATRIX_H
#define MY_MATRIX_H

#include <QVector>
#include <QString>

//#define  MyDBG  {qDebug() << __LINE__ << " " << __FUNCTION__;}

class my_matrix
{
public:
    QVector<QVector<double>> m;
    my_matrix();
    void new_size(int row, int col);
    void add(int i, int j, double value);
    int size_column();
    int size_row();
    void print();
    double D(QVector<QVector<double> >); // считает определитель
    QVector<QVector<double> > opr(const QVector<QVector<double> > &, int, int); // считает определитель
    QVector<double> kramer(QVector<double> B);
    QVector<QVector<double> > minor(const QVector<QVector<double> >&);
    void alg_dop(QVector<QVector<double> >&);
    void transp(QVector<QVector<double> >&);
};

#endif // MY_MATRIX_H
