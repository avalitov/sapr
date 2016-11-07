#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::init_table(QTableWidget** table,
                const QStringList& header_horizontal,
                const QStringList& header_vertical,
                const QVector<int> &size_horizontal,
                const QPoint &size)
{
    if (header_horizontal.size() != size_horizontal.size()) {
//        qDebug() << "Количество заголовков не соответствует количеству длин этих заголовков";
        abort();
        return;
    }

    (*table) = new QTableWidget(this);

    (*table)->setColumnCount(header_horizontal.size());
    (*table)->setRowCount(header_vertical.size());

    (*table)->setHorizontalHeaderLabels(header_horizontal);
    (*table)->setVerticalHeaderLabels(header_vertical);

    int width = sum(size_horizontal);
    int height = 235 - 30; // FIXME

    (*table)->move(size);
    (*table)->resize(width + 20 - 4, height);

    for (int i = 0; i < (*table)->columnCount(); ++i)
        (*table)->setColumnWidth(i, size_horizontal[i]);

    (*table)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    for (int i = 0; i < (*table)->rowCount(); ++i)
        for (int j = 0; j < (*table)->columnCount(); ++j)
            init_item(*table, i, j);

//    (*table)->setSpan(0, 0, 1, 2);
    (*table)->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

int MainWindow::sum(const QVector<int> &vec)
{
    int res = 0;
    for (int i = 0; i < vec.size(); ++i)
        res += vec[i];
    return res;
}

void MainWindow::init_item(QTableWidget *table, int row, int col)
{
    table->setItem(row, col, new QTableWidgetItem(""));
    table->item(row, col)->setTextAlignment(Qt::AlignCenter);
    table->item(row, col)->setBackgroundColor(Qt::white);
}
