#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::processor_color(QTableWidget *table, bool *res)
{
    bool flag = true;
    bool table_empty = true;
    if (res)
        *res = false;
    for (int i = 0; i < table->rowCount(); ++i)
        for (int j = 0; j < table->columnCount(); ++j) {
            if (table == table_rod)
                table_empty *= (table->item(i, j)->text().size() == 0);
            if (table->item(i, j)->backgroundColor() == Qt::red)
                flag = false;
        }
    if (table_empty && res)
        *res = true;
    return flag;
}

void MainWindow::of_processor()
{
    bool a_processor = true;
    bool res;
    a_processor *= processor_color(table_rod, &res);
    if (a_processor)
        a_processor *= processor_color(table_F, NULL);
    if (a_processor)
        a_processor *= processor_color(table_q, NULL);

    if (res) {
        empty_table = true;
        a_processor = false;
    } else
        empty_table = false;

    if (a_processor) {
        v_processor = true;
        ui->pushButton_2->setStyleSheet(QString::fromUtf8("background-color: white;"
                                                          "border-style: outset;"
                                                          "border-width: 2px;"
                                                          "border-radius: 10px;"
                                                          "border-color: beige;"
                                                          "font: bold 12px;"
                                                          "min-width: 10em;"
                                                          /*"padding: 6px;"*/));
    } else {
        v_processor = false;

        ui->pushButton_2->setStyleSheet(QString::fromUtf8("background-color: red;"
                                                          "border-style: outset;"
                                                          "border-width: 2px;"
                                                          "border-radius: 10px;"
                                                          "border-color: beige;"
                                                          "font: bold 12px;"
                                                          "min-width: 10em;"
                                                          /*"padding: 6px;"*/));
    }
    perexodim();
}

void MainWindow::on_pushButton_2_clicked()
{
    bool flag_per = perexodim();

    if (!v_processor && flag_per) {
        if (empty_table)
            QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Необходимо добавить хотя бы один стержень",
                    QMessageBox::Ok);
        else
            QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Избавьтесь от красных полей",
                    QMessageBox::Ok);
        return;
    } else if (v_processor && !flag_per) {
        QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Необходимо установить хотя бы одну заделку!",
                    QMessageBox::Ok);
        return;
    } else if (!v_processor && !flag_per) {
        if (empty_table)
            QMessageBox::critical(
                    this,
                    "Внимание!",
                    "1. Необходимо добавить хотя бы один стержень.\n"
                    "2. Необходимо установить хотя бы одну заделку!",
                    QMessageBox::Ok);
        else
            QMessageBox::critical(
                    this,
                    "Внимание!",
                    "1. Избавьтесь от красных полей.\n"
                    "2. Необходимо установить хотя бы одну заделку!",
                    QMessageBox::Ok);
        return;
    }

    this->setFixedWidth(size_wid_frame);
    ui->menuBar->resize(size_wid_frame, ui->menuBar->height());
    v_processore = true;
    ui->actionOpen->setEnabled(false);
    table_rod->hide();
    table_F->hide();
    table_q->hide();
    ui->graphicsView->hide();
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    ui->checkBox->hide();
    ui->checkBox_2->hide();
    ui->label_3->hide();
    off();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->label->show();

    filling_the_matrix();

    draw_table();
}

void MainWindow::filling_the_matrix()
{
    const QVector<cRod> &st = values_rod;

    double E, Ep = 0;/* = st[0].E*/;
    double A, Ap = 0;/* = st[0].A*/;
    double L, Lp = 1;/* = st[0].L*/;

    matrix.new_size(st.size() + 1, st.size() + 1);
    QVector<double> B;
    B.resize(st.size() + 1);
    for (int i = 0; i < B.size(); ++i)
        B[i] = 0;

    for (int i = 0; i < st.size(); ++i) {
        if (i - 1 >= 0) {
            Ep = st[i - 1].E;
            Ap = st[i - 1].A;
            Lp = st[i - 1].L;
        }
        E = st[i].E;
        A = st[i].A;
        L = st[i].L;
        matrix.add(    i,     i,  E*A/L + Ep*Ap/Lp);
        matrix.add(    i, i + 1, -E*A/L);
        matrix.add(i + 1,     i, -E*A/L);
    }

    for (int i = 0; i < values_q.size(); ++i) {
        int index = 0;
        index = values_q[i].rod - 1;
        double value = values_q[i].value*values_rod[index].L/2.;
        B[index] += value;
    }

    for (int i = 0; i < values_q.size(); ++i) {
        int index = 0;
        index = values_q[i].rod;
        if (index < values_q.size()) {
            double value = values_q[i].value*values_rod[index - 1].L/2.;
            B[index] += value;
        } else {
            double value = values_q[i].value*values_rod[index - 1].L/2.;
            B[index] += value;
        }
    }

    for (int i = 0; i < values_F.size(); ++i) {
        int index = values_F[i].uzel - 1;
        B[index] += values_F[i].value;
    }

    matrix.add(st.size(), st.size(), E*A/L);

    if (ui->checkBox->isChecked()) {
        matrix.add(0, 0, 1);
        matrix.add(0, 1, 0);
    }

    if (ui->checkBox_2->isChecked()) {
        matrix.add(st.size(), st.size() - 1, 0);
        matrix.add(st.size(),     st.size(), 1);
    }

    if (ui->checkBox->isChecked())
        B[0] = 0;

    if (ui->checkBox_2->isChecked())
        B[st.size()] = 0;

    roots = matrix.kramer(B);
}

void MainWindow::draw_table()
{
    int gy = 40;

    for (int i = 0; i < values_rod.size() + 1; ++i) {
        lbl.push_back(new QLabel(ui->centralWidget));
        lbl[lbl.size() - 1]->setText("Δ" + QString("%1 = %2").arg(i + 1).arg(roots[i]));
        lbl[lbl.size() - 1]->setGeometry(QRect(110, gy, 100, 20));
        lbl[lbl.size() - 1]->show();

        if (i < values_rod.size()) {
            U0.push_back(roots[i]);
            UL.push_back(roots[i + 1]);
        }

        gy += 20;
    }

    int tmp_y = gy + 40;

    ui->label_2->move(110, tmp_y - 5 - ui->label_2->height()/2.0);
    ui->label_2->show();
    ui->lineEdit_2->move(110 + ui->label_2->width(), tmp_y - 5 - ui->lineEdit_2->height()/2.);
    ui->label_7->move(110 + ui->label_2->width() + ui->lineEdit_2->width() + 10, tmp_y - 5 - ui->label_7->height()/2.);
    ui->label_7->show();
    ui->lineEdit_2->show();

    QStringList list;
    for (int i = 0; i < values_rod.size(); ++i)
        list << tr("Стержень %1").arg(i + 1);
//    for (int i = 0; i <)
    QObject::disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(set_interval(int)));
    QObject::disconnect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(new_value()));
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
    tmp_y += 10;
    ui->comboBox->move(110, tmp_y + ui->lineEdit_2->height()/2.);
    ui->doubleSpinBox->move(110 + ui->comboBox->width() + 10, tmp_y + ui->lineEdit_2->height()/2.);
    ui->label_4->show();
    ui->label_5->show();
    ui->label_6->show();
    ui->lineEdit->show();
    ui->lineEdit_3->show();
    ui->lineEdit_4->show();

    double my_tmp_value_x = 110 + ui->comboBox->width() + 10 + ui->doubleSpinBox->width() + 10;
    ui->label_4->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->label_4->width()/* + 10*/;
    ui->lineEdit->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->lineEdit->width() + 10;
    ui->label_5->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->label_5->width()/* + 10*/;
    ui->lineEdit_3->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->lineEdit_3->width() + 10;
    ui->label_6->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->label_6->width()/* + 10*/;
    ui->lineEdit_4->move(my_tmp_value_x, tmp_y + ui->lineEdit_2->height()/2.);
    my_tmp_value_x += 10 + ui->lineEdit_4->width() + 10;

    ui->comboBox->show();
    ui->doubleSpinBox->show();

    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(set_interval(int)));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(new_value()));

    set_interval(0);
    new_value();
//    ui->comboBox->setCurrentIndex(ui->comboBox->currentIndex());
//    ui->doubleSpinBox->setValue(ui->doubleSpinBox->value() - 0.01);
//    ui->doubleSpinBox->setValue(ui->doubleSpinBox->value() + 0.01);

    gy += 120;

    QStringList h_header;
    h_header << "x" << "Nx" << "ux" << "σx" << "[σ]";
    QStringList v_header;
    int step = ui->lineEdit_2->text().toInt();
    int row_count = values_rod.size() + values_rod.size()*(step + 1);
    for (int i = 0; i < row_count; ++i)
        v_header << tr("%1").arg(i + 1);
    QVector<int> size_hor;
    size_hor << 50 << 150 << 150 << 150 << 150;
    if (rascheti != NULL) {
        rascheti->hide();
        delete rascheti;
    }
    init_table(&rascheti, h_header, v_header, size_hor, QPoint(110, gy));

    int n = 0;
    for (int i = 0; i < rascheti->rowCount(); i += step + 2) {
        rascheti->setSpan(i, 0, 1, size_hor.size());
        QString tmp_str = tr("Cтержень %1").arg(++n);
        rascheti->item(i, 0)->setText(tmp_str);
        rascheti->item(i, 0)->setBackgroundColor(Qt::cyan);
        QFont font(tmp_str, -1, 2);
        font.setBold(true);
        rascheti->item(i, 0)->setFont(font);
        QTableWidgetItem* tmp_item = new QTableWidgetItem("");
        rascheti->setVerticalHeaderItem(i, tmp_item);
//        tmp_item->setBackgroundColor(QColor(0, 255, 255));
    }

    v_x.clear();
    v_Nx.clear();
    v_ux.clear();
    v_qx.clear();

    double copy_x_begin = 0;

    for (int i = 0; i < values_rod.size(); ++i) {
        const double x_step = values_rod[i].L/step;
        double x_begin = 0;
        double x_begin2 = 0;
        for (int j = 0; j < step + 1; ++j) {
            QTableWidgetItem* tmp_item = new QTableWidgetItem(tr("%1").arg(j + 1));
            rascheti->setVerticalHeaderItem(i*step + i + 1 + j + i, tmp_item);
//            tmp_item->setBackgroundColor(QColor(0, 255, 255));
            int index = 0;
            v_x.push_back(x_begin);
            v_Nx.push_back(func_N(x_begin, i));
            v_ux.push_back(func_u(x_begin, i));
            v_qx.push_back(func_q(x_begin, i));
            rascheti->item(i*step + i + 1 + j + i, index++)->setText(my_d(tr("%1").arg(  md(x_begin2, 3),  0, 'f', 3)));
            rascheti->item(i*step + i + 1 + j + i, index++)->setText(my_d(tr("%1").arg(func_N(md(x_begin2, 3), i),  0, 'f', 4)));
            rascheti->item(i*step + i + 1 + j + i, index++)->setText(my_d(tr("%1").arg(func_u(md(x_begin2, 3), i),  0, 'f', 4)));
            int tmp_index = index;
            rascheti->item(i*step + i + 1 + j + i, index++)->setText(my_d(tr("%1").arg(func_q(md(x_begin2, 3), i),  0, 'f', 4)));
            rascheti->item(i*step + i + 1 + j + i, index++)->setText(my_d(tr("%1").arg(      values_rod[i].G,  0, 'f', 4)));

            for (int index_tmp = 0; index_tmp < index; ++index_tmp)
                rascheti->item(i*step + i + 1 + j + i, index_tmp)->setBackgroundColor(QColor(230, 255, 255));

            if (fabs(func_q(x_begin, i)) > values_rod[i].G)
                rascheti->item(i*step + i + 1 + j + i, tmp_index)->setBackgroundColor(Qt::red);

            x_begin += x_step;
            x_begin2 += md(x_step, 3);
            copy_x_begin += x_step;
        }
        copy_x_begin -= x_step;
    }
    rascheti->setEditTriggers(false);

    rascheti->show();

    rascheti->resize(sum(size_hor) + 9 + 22, 25 + rascheti->rowHeight(0)*row_count);
    gy += rascheti->height();
//    gy += 10;
    ui->pushButton_3->move(220 - ui->pushButton_3->width(), gy);
    ui->pushButton_4->move(780, gy);
    this->setFixedHeight(gy + ui->pushButton_3->height() + 40);
}

void MainWindow::set_interval(int index) {
    ui->comboBox->setCurrentIndex(index);
    if (values_rod.size() > 0 && index < values_rod.size()) {
        ui->doubleSpinBox->setMinimum(0);
        ui->doubleSpinBox->setMaximum(values_rod[index].L);
    }
}

void MainWindow::new_value()
{
    if (values_rod.size() > 0 && ui->comboBox->currentIndex() < values_rod.size()) {
        ui->lineEdit->setText(my_d(tr("%1").arg(func_N(ui->doubleSpinBox->value(), ui->comboBox->currentIndex()),  0, 'f', 4)));
        ui->lineEdit_3->setText(my_d(tr("%1").arg(func_u(ui->doubleSpinBox->value(), ui->comboBox->currentIndex()),  0, 'f', 4)));
        ui->lineEdit_4->setText(my_d(tr("%1").arg(func_q(ui->doubleSpinBox->value(), ui->comboBox->currentIndex()),  0, 'f', 4)));
    }
}

// p - индекс стержня
double MainWindow::func_u(double x, int p)
{
    double Lp = values_rod[p].L;

    double qp = my_find_q(p);
    double Ep = values_rod[p].E;
    double Ap = values_rod[p].A;

    return U0[p] + x/Lp*(UL[p] - U0[p])
            + qp*Lp*Lp*x/(2.*Ep*Ap*Lp)*(1. - x/Lp);
}

double MainWindow::func_N(double x, int p)
{
    double Lp = values_rod[p].L;

    double qp = my_find_q(p);
    double Ep = values_rod[p].E;
    double Ap = values_rod[p].A;

    return Ep*Ap/Lp*(UL[p] - U0[p])
            + qp*Lp/2.*(1. - 2.*x/Lp);
}

double MainWindow::func_q(double x, int p)
{
    return func_N(x, p)/values_rod[p].A;
}

double MainWindow::my_find_q(int p)
{
    for (int i = 0; i < values_q.size(); ++i)
        if (values_q[i].rod - 1 == p)
            return values_q[i].value;
    return 0;
}

void MainWindow::ferger(QTextStream &stream, QTableWidget *table, bool in_excel)
{
    int n = 0;
    for (int i = 0; i < table->rowCount(); ++i) {
        for (int j = 0; j < table->columnCount(); ++j) {
            QString tmp_str = table->item(i, j)->text();
            if ((tmp_str.size() == 0) && !in_excel)
                tmp_str = "_";
            if (in_excel && (tmp_str.size() == 0 || tmp_str.contains("\xD0\xB5\xD1\x80\xD0\xB6\xD0\xB5\xD0\xBD\xD1\x8C"))) {
                n = 1;
            }
            if (in_excel)
                for (int index_s = 0; index_s < tmp_str.size(); ++index_s)
                    if (tmp_str[index_s] == '.')
                        tmp_str[index_s] = ',';
            if (in_excel && (j == 0) && !tmp_str.contains("\xD0\xB5\xD1\x80\xD0\xB6\xD0\xB5\xD0\xBD\xD1\x8C")) {
                stream << n++ << ";";
//                qDebug() << tmp_str.toUtf8() << " != " << "\xD0\xB5\xD1\x80\xD0\xB6\xD0\xB5\xD0\xBD\xD1\x8C";
            }


            stream << (j == 0 ? "" : " ") + tmp_str + (in_excel ? ";" : "");
        }
        stream << endl;
    }
}

void MainWindow::lineEdit_2_textChanged(const QString arg1)
{
    QObject::disconnect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(lineEdit_2_textChanged(QString)));
    for (int i = 0; i < lbl.size(); ++i) {
        lbl[i]->hide();
        delete lbl[i];
    }
    lbl.clear();

    QString tmp = arg1;
    int index = 0;
    while ((index < tmp.size()) && (tmp[index] == '0')) {
        tmp.remove(0, 1);
        ++index;
    }
    if (tmp.size() == 0) {
        tmp = "0";
    }
    ui->lineEdit_2->setText(tmp);

    draw_table();
    QObject::connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(lineEdit_2_textChanged(QString)));
}
