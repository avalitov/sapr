#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
//    w->setFixedSize(1000, 650);
    w->resize(1000, 650);

    QScrollArea *p_scroll = new QScrollArea;
    p_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    p_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

//    p_scroll->setFixedSize(1000 + 19/* + scroll.verticalScrollBar()->width()*/, 650 + 19/* + scroll.horizontalScrollBar()->height()*/);
    p_scroll->resize(1000 + 19/* + scroll.verticalScrollBar()->width()*/, 650 + 19/* + scroll.horizontalScrollBar()->height()*/);
    p_scroll->setWidget(w->window());
//    scroll.viewport()->setBackgroundRole(QPalette::Dark);
    p_scroll->viewport()->setAutoFillBackground(true);
    p_scroll->setWindowTitle("САПР");
    p_scroll->setWindowIcon(QIcon("icon.png"));
    p_scroll->showMaximized();
//    p_scroll->showFullScreen();

//    //  row 1
//    QTableWidget *tbl = new QTableWidget(3, 3);
//    QTableWidgetItem *tblItem;
//    tblItem = new QTableWidgetItem("text 1");
//    tbl->setItem(0, 0, tblItem);
//    tblItem = new QTableWidgetItem("text 2");
//    tbl->setItem(1, 0, tblItem);
//    tblItem = new QTableWidgetItem("text 3");
//    tbl->setItem(2, 0, tblItem);
//    // row 2
//    tblItem = new QTableWidgetItem("text 4");
//    tblItem->setSpan(1, 2); // вот сдесь мы обьединили для 0 строки 2 колонки, начиная с колонки 1
//    tbl->setItem(0, 1, tblItem);
//    tblItem = new QTableWidgetItem("text 5");
//    tbl->setItem(1, 1, tblItem);
//    tblItem = new QTableWidgetItem("text 6");
//    tbl->setItem(2, 1, tblItem);
//    // row 3
//    tblItem = new QTableWidgetItem("text 7");
//    tbl->setItem(1, 2, tblItem);
//    tblItem = new QTableWidgetItem("text 8");
//    tbl->setItem(2, 2, tblItem);
    return a.exec();


//    const int row = 3;
//    const int col = 3;

//    my_matrix m;
//    m.new_size(row, col);

////    double mas[row][col] = {{ -8,  1, -1,  3},
////                            {  3, -1,  4, -3},
////                            {-14,  2,  5,  1},
////                            {  5,  3,  1, -1}
////                           };

////    double mas[row][col] = {{ 506, 66},
////                            {  66, 11}
////                           };

//    double mas[row][col] = {{ 18, -2,  0},
//                            { -5,  1, -5},
//                            {  1,  7,  2}
//                           };

//    for (int i = 0; i < m.size_row(); ++i)
//        for (int j = 0; j < m.size_column(); ++j)
//            m.add(i, j, mas[i][j]);
//    QVector<double> B;
//    B.push_back(8);
//    B.push_back(3);
//    B.push_back(-4);
//    MyDBG
//    B = m.kramer(B);
//    MyDBG
//    qDebug() << B;
//    m.print();
//    return 0;
}
