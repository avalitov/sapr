#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::change(QTableWidgetItem* item,
                        const int& table_number)
{
    if (of_timer) {
        return;
    }

    off();

    // добавить запрет на напряжение равным нулю в таблице 0?
    bool napr03 = true;

    // определяем таблицу
    QTableWidget* table;
    switch (table_number) {
    case 0: table = table_rod;
            break;
    case 1: table = table_F;
            break;
    case 2: table = table_q;
            break;
    default:    //qDebug() << "нет такой таблицы!";
                abort();
                break;
    }

    int count = 0;
    bool correctly = true;

    bool of_print = false;
    int  my_num_err = INT_MIN;
    bool correctly1 = true;
    int count1 = 0;

    for (int i = 0; i < table->rowCount(); ++i) {
        QVector<double> vec;
        bool bcount = true;
        bool first_j = false;
        bool pustie = true;

        for (int j = 0; j < table->columnCount(); ++j) {
            QString stmp = table->item(i, j)->text();
            for (int i = 0; i < stmp.size(); ++i)
                if (stmp[i] == ',')
                    stmp[i] = '.';
            bool tmp_count = true;
            int num_err = -1;
            bool ok = true;
            double tmp = stmp.toDouble(&ok);
            /*if (table->item(i, j) != NULL) FIXME: предположение что эта проверка не нужна*/{
                if (table == table_rod) {
                    if ((!ok) && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 11;
                    }
                    if (tmp_count && napr03)
                    if ((j == 3) && (tmp <= 0)
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 12;
                    }
                    if (tmp_count)
                    if ((j == 0 || j == 1) && (tmp <= 0)
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = j;
                    }
                    if (tmp_count)
                    if ((j == 2) && (tmp <= 0)
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 6;
                    }
                    if (tmp_count)
                    if (!ok) {
                        correctly = bcount = tmp_count = false;
//                        break;
                    }
                }
                if (table == table_F) {
                    if (j == 0) {
                        bool tmp_ok = ok;
                        tmp = stmp.toInt(&ok);
                        if ((!ok) && (stmp.size() != 0)) {
                            correctly = bcount = tmp_count = false;
                            if (tmp_ok)
                                num_err = 10;
                            else
                                num_err = 11;
                        }
                    } else {
                        if ((!ok) && (stmp.size() != 0)) {
                            correctly = bcount = tmp_count = false;
                            num_err = 11;
                        }
                    }
                    if (tmp_count)
                    if ((j == 1) && (tmp == 0)
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 3;
                    }
                    if (tmp_count)
                    if ((j == 0) && ((tmp < 1) || ((tmp > values_rod.size() + 1))
                                     || (tmp == 1 ? (values_rod.size() == 0 ? true : false) : false))
                                 && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 2;
                    }
                    if (tmp_count)
                    if (j == 0) {
                        bool find_value = false;
                        for (int i = 0; i < values_F.size(); ++i) {
                            if (values_F[i].uzel == tmp) {
                                find_value = true;
                                break;
                            }
                        }
                        if (find_value) {
                            correctly = bcount = tmp_count = false;
                            num_err = 8;
                        }
                    }
                    if (tmp_count)
                    if (!ok)
                        correctly = bcount = tmp_count = false;
                }
                if (table == table_q) {
                    if (j == 0) {
                        bool tmp_ok = ok;
                        tmp = stmp.toInt(&ok);
                        if ((!ok) && (stmp.size() != 0)) {
                            correctly = bcount = tmp_count = false;
                            if (tmp_ok)
                                num_err = 9;
                            else
                                num_err = 11;
                        }
                    } else {
                        if ((!ok) && (stmp.size() != 0)) {
                            correctly = bcount = tmp_count = false;
                            num_err = 11;
                        }
                    }
                    if (tmp_count)
                    if ((j == 1) && (tmp == 0)
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 4;
                    }
                    if (tmp_count)
                    if ((j == 0) && ((tmp < 1) || (tmp > values_rod.size()))
                            && (stmp.size() != 0)) {
                        correctly = bcount = tmp_count = false;
                        num_err = 5;
                    }
                    if (tmp_count)
                    if (j == 0) {
                        bool find_value = false;
                        for (int i = 0; i < values_q.size(); ++i) {
                            if (values_q[i].rod == tmp) {
                                find_value = true;
                                break;
                            }
                        }
                        if (find_value) {
                            correctly = bcount = tmp_count = false;
                            num_err = 7;
                        }
                    }
                    if (tmp_count)
                    if (!ok)
                        correctly = bcount = tmp_count = false;
                }
            }

            if ((j != 0) && (stmp.size() != 0)) {
                if (table->item(i, 0)->backgroundColor() == Qt::white) {
                    table->item(i, 0)->setBackgroundColor(Qt::red);
                }
            }

            if (ok && tmp_count) {
                if (j == 0)
                    first_j = true;
                vec.push_back(tmp);
                if (table->item(i, j)->backgroundColor() == Qt::red)
                    table->item(i, j)->setBackgroundColor(Qt::white);
            } else if (table == table_rod) {
                if ((j == 2) || (j == 3)) {
                    vec.push_back(0);
                    if ((j == 0) || (j == 1))
                        correctly1 *= (stmp.size() != 0);
                }
            }

            bool expr = false;
            if (first_j)
                expr = (j != 0) && (!(ok && tmp_count));
            else
                expr = (!(ok && tmp_count)) && (stmp.size() != 0);

            if (expr) {
                if (table->item(i, j)->backgroundColor() == Qt::white)
                    table->item(i, j)->setBackgroundColor(Qt::red);
            } else {
                if (table->item(i, j)->backgroundColor() == Qt::red)
                    table->item(i, j)->setBackgroundColor(Qt::white);
            }

            if (((table == table_q) && (((stmp.size() != 0) && (j == 0)))
                    && (((tmp > values_rod.size()))
                        ||
                        (!(ok && bcount)))
                        ))
                    if (table->item(i, j)->backgroundColor() == Qt::white)
                        table->item(i, j)->setBackgroundColor(Qt::red);

            if (((table == table_F) && (stmp.size() != 0) && (j == 0)
                    && (((tmp > values_rod.size() + 1))
                        ||
                        (!(ok && bcount)))
                        ))
                    if (table->item(i, j)->backgroundColor() == Qt::white)
                        table->item(i, j)->setBackgroundColor(Qt::red);
            if ((j != 0) && (stmp.size() != 0)) {
                pustie = false;
            }

            if ((item != NULL) && (item->row() == i) && (item->column() == j)) {
                of_print = ((((!ok) && (stmp.size() != 0)) || (!bcount)) && (num_err != -1)
                            && ((is_push_button == 2) || (!of_timer))
                            && (is_push_button != 2) && (!of_timer) && (!of_timer_button));
                my_num_err = num_err;
            }
        }

        if ((pustie && (first_j)) || first_j) {
            if (table->item(i, 0)->backgroundColor() == Qt::red) {
                table->item(i, 0)->setBackgroundColor(Qt::white);
            }
        }

        if ((vec.size() == table->columnCount())) {
            count += bcount;
            ++count1;
            switch (table_number) {
            case 0:
                values_rod.push_back(cRod(vec[0], vec[1], vec[2], vec[3]));
                break;
            case 1:
                values_F.push_back(cF(vec[0], vec[1]));
                break;
            case 2:
                values_q.push_back(cq(vec[0], vec[1]));
                break;
            default:
                break;
            }
            table->setVerticalHeaderItem(i, new QTableWidgetItem(tr("%1").arg(i + 1)));
        }
    }

    if (of_print)
        info_error(my_num_err);

    if ((correctly || (correctly1 && (table = table_rod))) && table) {
        int new_row = table->rowCount() + 1;
        if ((count == table->rowCount())
                || ((correctly1 && (table = table_rod)) && (count1 == table->rowCount()))) {
            table->setRowCount(new_row--);

            for (int col = 0; col < table->columnCount(); ++col)
                init_item(table, new_row, col);

            table->setVerticalHeaderItem(new_row, new QTableWidgetItem("*"));
        }
    }
    num_tables();

    on();
}

void MainWindow::info_error(const int& error) {
    if (!of_load)
    switch (error) {
    case 0: QMessageBox::critical(
                this,
                "Внимание!",
//                "Вы ввели некорректное значение!\n"
                "Длина стержня должна быть больше нуля!",
                QMessageBox::Ok);
            break;
    case 1: QMessageBox::critical(
                this,
                "Внимание!",
//                "Вы ввели некорректное значение!\n"
                "Площадь поперечного сечения стержня должна "
                "быть больше нуля!",
                QMessageBox::Ok);
            break;
    case 2: QMessageBox::critical(
                this,
                "Внимание!",
                //"Вы ввели некорректное значение!\n"
                "Не существует такого узла",
                QMessageBox::Ok);
            break;
    case 3: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Нельзя задавать значение статического "
                    "сосредоточенного продольного усилия равным нулю",
                    QMessageBox::Ok);
        break;
    case 4: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Нельзя задавать значение статической "
                    "погонной нагрузки равной нулю",
                    QMessageBox::Ok);
        break;
    case 5: QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Вы ввели некорректное значение!\n"
                    "Не существует такого стержня",
                    QMessageBox::Ok);
        break;
    case 6: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Модуль упругости должен быть больше нуля",
                    QMessageBox::Ok);
        break;
    case 7: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Нельзя нагружать стержень более одного "
                    "раза статической погонной нагрузкой",
                    QMessageBox::Ok);
        break;
    case 8: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Нельзя нагружать узел более одного "
                    "раза статически сосредоточенным "
                    "продольным усилием",
                    QMessageBox::Ok);
        break;
    case 9: QMessageBox::critical(
                    this,
                    "Внимание!",
                    //"Вы ввели некорректное значение!\n"
                    "Тип стержня должен быть целочисленным",
                    QMessageBox::Ok);
        break;
    case 10: QMessageBox::critical(
                    this,
                    "Внимание!",
//                    "Вы ввели некорректное значение!\n"
                    "Тип узла должен быть целочисленным",
                    QMessageBox::Ok);
        break;
    case 11: QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Ожидается ввод чисел!",
                    QMessageBox::Ok);
        break;
    case 12: QMessageBox::critical(
                    this,
                    "Внимание!",
                    "Напряжение должно быть больше нуля!",
                    QMessageBox::Ok);
        break;
    default: QMessageBox::critical(
                this,
                "Внимание!",
                "Вы ввели некорректное значение!",
                QMessageBox::Ok);
            break;
    }
}

QString MainWindow::my_d(QString str, bool delete_minus)
{
    if (delete_minus && str.size() >= 1 && str[0] == '-')
        str.remove(0, 1);
    int index = str.size();
    while (--index >= 1 && (str[index] == '0' || str[index] == '.'))
        if (!(index == 2 && str[0] == '-'))
            str.resize(str.size() - 1);
        else if (index == 2 && str[0] == '-' && str[1] == '0')
            str.remove(0, 1);
    if (str.size() - 1 >= 0 && str[str.size() - 1] == '.')
        str.resize(str.size() - 1);
    return str;
}

QString MainWindow::md(double str, QString a_nado)
{
    QString new_str = tr("%1").arg(str);

    if (new_str.size() == 1 && new_str[0] == '1' && a_nado == "yes")
        return "";
    if (new_str.size() == 2 && new_str[0] == '-' && new_str[1] == '1')
        return "-";
    return new_str;
}

double MainWindow::md(double number, int precition)
{
    return (tr("%1").arg(number, 0, 'f', precition)).toDouble();
}
