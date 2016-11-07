#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QPainter>

void MainWindow::add_elem(int add_h)
{
    scene->setBackgroundBrush(QBrush(Qt::white));
    double above = /*ui->graphicsView->size().height()*/350*3./10.;
    double size_y = /*ui->graphicsView->size().height()*/350 - above*2;
    double size_x = /*ui->graphicsView->size().width()*/1000;
    ky = size_y;
    kx = size_x;

    scene->addLine(0, -1, 0, 1, QPen(Qt::white));
    scene->addLine(0, -29, 1, -29, QPen(Qt::white));

    sdvig_x = 80;

    double maxh = INT_MIN;
    double maxw = 0;

    for (int i = 0; i < values_rod.size(); ++i) {
        maxw += values_rod[i].L;
        if (values_rod[i].H > maxh)
            maxh = values_rod[i].H;
    }

    kx = kx/maxw*(7./10);
    ky = ky/maxh*(7./10);

    QVector<double> start_x_rod;
    QVector<double> start_y_rod;

    QVector<double> real_size_w_rod;
    QVector<double> real_size_h_rod;

    maxw = 0;

    QString str = "";
    QFontMetrics f(str);

    // блоки
    for (int i = 0; i < values_rod.size(); ++i) {
            // отрисовка стержня
            double x = maxw;
            start_x_rod.push_back(x);
            double w = values_rod[i].L*kx;
            double h = values_rod[i].H*ky;
            double min_tmp = 95;
            double min_w = 185;
            if (w < min_w)
                w = w/(min_w - 1.)*(min_w - min_tmp - 1.) + min_tmp;
            real_size_w_rod.push_back(w);

            double min_h_tmp = 15;
            double min_h = 80;
            if (h < min_h)
                h = h/(min_h - 1.)*(min_h - min_h_tmp - 1.) + min_h_tmp;
            real_size_h_rod.push_back(h);

            maxw += real_size_w_rod[i];

            double y = size_y/2. - real_size_h_rod[i]/2.;
            start_y_rod.push_back(y);

            scene->addRect(start_x_rod[i] + sdvig_x, start_y_rod[i], real_size_w_rod[i], h, QPen(QBrush(Qt::black), 2))/*->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable)*/;

            // подписывание стержня горизонтально сверху
            str = tr("%1E, %2A").arg(md(values_rod[i].E)).arg(md(values_rod[i].A));
            QGraphicsTextItem *text_rod = new QGraphicsTextItem(str);
            text_rod->setPos(start_x_rod[i] + real_size_w_rod[i]/2. - f.width(str)/2. + sdvig_x, start_y_rod[i] - 23);
            scene->addItem(text_rod);

            // отрисовка номера стержня сверху в круге
            double w1 = 30;
            double h1 = w1;
            double x1 = start_x_rod[i] + real_size_w_rod[i]/2. - w1/2.;
            double y1 = size_y/2. - maxh/2.*ky - above*9./10.;
            scene->addEllipse(x1 + sdvig_x, y1, w1, h1);
            str = tr("%1").arg(i+1);
            QGraphicsTextItem *textItem = new QGraphicsTextItem(str);
            textItem->setPos(int(x1) + w1/2. - f.width(str)/2. - 2 + sdvig_x, int(y1) + 4);
            scene->addItem(textItem);
        int dob_wid = 0;
        int counter = i == values_rod.size() - 1 ? 2 : 1;
        int tmp_counter = counter;
        while (counter--) {
            // отрисовка номеров узла
            double w2 = 30;
            double h2 = w2;
            double x2 = start_x_rod[i] + dob_wid - w2/2.;
            double y2 = size_y/2. + maxh/2.*ky + above*5./10.;
            scene->addRect(x2 + sdvig_x, y2, w2, h2);
            str = tr("%1").arg(i + 1  + (tmp_counter == 2 && counter == 0 ? 1 : 0));

            QGraphicsTextItem *textItem1 = new QGraphicsTextItem(str);
            textItem1->setPos(int(x2) + w2/2. - f.width(str)/2. - 2 + sdvig_x, int(y2) + 4);
            scene->addItem(textItem1);

            if (((counter == 0) && (i != values_rod.size() - 1))
                    || ((counter == 1) && (i == values_rod.size() - 1))) {
                //*** разметка
                // горизонтальная линия
                str = tr("%1L").arg(md(values_rod[i].L));

                QGraphicsTextItem *textL = new QGraphicsTextItem(str);
                textL->setPos(start_x_rod[i] + dob_wid + real_size_w_rod[i]/2 - f.width(str)/2. + sdvig_x, y2 + h2 - 10);
                scene->addItem(textL);
                scene->addLine(start_x_rod[i] + dob_wid + sdvig_x, y2 + h2 + 10,
                               start_x_rod[i] + dob_wid + w + sdvig_x, y2 + h2 + 10);
                // стрелочки горизонтальной линии
                scene->addLine(start_x_rod[i] + dob_wid + sdvig_x, y2 + h2 + 10,
                               start_x_rod[i] + dob_wid + 6 + sdvig_x, y2 + h2 + 10 - 3, QPen(QBrush(Qt::black), 2));
                scene->addLine(start_x_rod[i] + dob_wid + sdvig_x, y2 + h2 + 10,
                               start_x_rod[i] + dob_wid + 6 + sdvig_x, y2 + h2 + 10 + 3, QPen(QBrush(Qt::black), 2));
                scene->addLine(start_x_rod[i] + dob_wid + real_size_w_rod[i] + sdvig_x, y2 + h2 + 10,
                               start_x_rod[i] + dob_wid + w - 6 + sdvig_x, y2 + h2 + 10 - 3, QPen(QBrush(Qt::black), 2));
                scene->addLine(start_x_rod[i] + dob_wid + real_size_w_rod[i] + sdvig_x, y2 + h2 + 10,
                               start_x_rod[i] + dob_wid + w - 6 + sdvig_x, y2 + h2 + 10 + 3, QPen(QBrush(Qt::black), 2));
            }
                // вертикальные линии
                scene->addLine(start_x_rod[i] + dob_wid + sdvig_x, start_y_rod[i] + h,
                               start_x_rod[i] + dob_wid + sdvig_x, y2/*, QPen(Qt::blue)*/);
                scene->addLine(start_x_rod[i] + dob_wid + sdvig_x, y2 + h2,
                               start_x_rod[i] + dob_wid + sdvig_x, y2 + h2 + 15 + add_h/*, QPen(Qt::blue)*/);
            if (dob_wid == 0)
                dob_wid += real_size_w_rod[i];
            if (counter == 1)
                start_x_rod.push_back(start_x_rod[i] + dob_wid);

        //***
        }
    }
    if (start_x_rod.size() - 1 >= 0 && real_size_w_rod.size() - 1 >= 0)
        start_x_rod.push_back(start_x_rod[start_x_rod.size() - 1] + real_size_w_rod[real_size_w_rod.size() - 1]);

    QVector<int> ogr;

//    for (int i = 1; i < values_rod.size(); ++i)
//        ogr.push_back(i);
    if (ui->checkBox->isChecked())
        ogr.push_back(0);
    if (ui->checkBox_2->isChecked())
        ogr.push_back(values_rod.size());

    for (int i = 0; i < values_rod.size(); ++i) {
        bool f_find = false;
        bool f_end = false;
        for (int j = 0; j < ogr.size(); ++j) {
            if (ogr[j] == values_rod.size())
                f_end = true;
            if (i == ogr[j])
                f_find = true;
        }
        bool ogr_end = (i == values_rod.size() - 1) && (f_end)/* && (ogr[ogr.size() - 1] == -1)*/ ? true : false;
        if (f_find) {
            double lx = start_x_rod[i];
            double ly1 = size_y/2. - maxh*14./10.*ky/2.;
            double ly2 = size_y/2. + maxh*14./10.*ky/2.;

            scene->addLine(lx + sdvig_x, ly1, lx + sdvig_x, ly2, QPen(QBrush(Qt::black), 2));

            while (ly2 - ly1 > 0) {
                scene->addLine(lx - 2 + sdvig_x, ly1,
                               lx - 5 - 2 + sdvig_x, ly1 + 3);
                ly1 += 6;
            }
        }
        if (ogr_end) {
            double lx = start_x_rod[i] + real_size_w_rod[i];
            double ly1 = size_y/2. - maxh*14./10.*ky/2.;
            double ly2 = size_y/2. + maxh*14./10.*ky/2.;

            scene->addLine(lx + sdvig_x, ly1, lx + sdvig_x, ly2, QPen(QBrush(Qt::black), 2));

            while (ly2 - ly1 > 0) {
                scene->addLine(lx + sdvig_x, ly1,
                               lx + sdvig_x + 5, ly1 - 3);
                ly1 += 6;
            }
        }

    }

    // ёлочки
    for (int i = 0; i < values_q.size(); ++i) {
        if ((values_q[i].rod >= 1) && (values_q[i].rod <= values_rod.size())) {
            int sign = 1;
            if (values_q[i].value < 0)
                sign = -1;

            int start_x = start_x_rod[values_q[i].rod - 1] + sdvig_x;
            int end_x = start_x_rod[values_q[i].rod] + sdvig_x;

            int width_strelki = abs(end_x - start_x)/2.;

            //*** подписываю
            str = tr("%1q").arg(md(fabs(values_q[i].value)));
            QGraphicsTextItem *text_rod = new QGraphicsTextItem(str);
            int index_rod = values_q[i].rod - 1;
            double size_h = real_size_h_rod[index_rod];

            double sdvig_str_h = 0;
            double sdvig_str_w = 0;
            if (size_h < 52) {
//                qDebug() << "надо перемещать у " << values_q[i].rod;
                sdvig_str_w = 14;
                sdvig_str_h = -size_h - 15;

                double beg_x_line = start_x_rod[index_rod] + real_size_w_rod[index_rod]*1./10. + sdvig_x;
                double x_beg = beg_x_line + 20;
                double x_end = x_beg + f.width(str)/**12./10.*/ + 2;
                scene->addLine(beg_x_line,
                               size_y/2.,
                               x_beg,
                               size_y/2. + sdvig_str_h,
                               QPen(QBrush(Qt::black), 2));
                scene->addLine(x_beg,
                               size_y/2. + sdvig_str_h,
                               x_end,
                               size_y/2. + sdvig_str_h,
                               QPen(QBrush(Qt::black), 2));
                double avarage = (x_beg + x_end)/2.;
                text_rod->setPos(avarage - f.width(str)/2. + (str.size() > 2 ? 2 : -2),
                                 size_y/2. - 19 + sdvig_str_h);
            } else {
                text_rod->setPos((start_x > end_x ? end_x : start_x) - f.width(str)/2./* + sdvig_x*/ + width_strelki + sdvig_str_w,
                                 size_y/2. - 25 + sdvig_str_h);
            }

            scene->addItem(text_rod);
            //***

            scene->addLine(start_x, size_y/2., end_x, size_y/2., QPen(QBrush(Qt::black), 2));

            double begin = start_x_rod[values_q[i].rod + (sign == 1 ? 0 : -1)];
            sign *= -1;
            if (sign > 0) {
                while (begin < start_x_rod[values_q[i].rod]) {
                    scene->addLine(begin + sdvig_x, size_y/2., begin + sign*8/* + tmp*/ + sdvig_x, size_y/2. + 6/* - tmp*/, QPen(QBrush(Qt::black), 2));
                    scene->addLine(begin + sdvig_x, size_y/2., begin + sign*8/* + tmp*/ + sdvig_x, size_y/2. - 6/* + tmp*/, QPen(QBrush(Qt::black), 2));
                    begin += 6*3;
                }
            } else if (sign < 0) {
                while (begin > start_x_rod[values_q[i].rod - 1]) {
                    scene->addLine(begin + sdvig_x, size_y/2., begin + sign*8/* - tmp*/ + sdvig_x, size_y/2. + 6/* - tmp*/, QPen(QBrush(Qt::black), 2));
                    scene->addLine(begin + sdvig_x, size_y/2., begin + sign*8/* - tmp*/ + sdvig_x, size_y/2. - 6/* + tmp*/, QPen(QBrush(Qt::black), 2));
                    begin -= 6*3;
                }
            }
        }
    }

    // стрелка вправо или влево
    for (int i = 0; i < values_F.size(); ++i) {
        if ((values_F[i].uzel >= 1) && (values_F[i].uzel <= values_rod.size() + 1)
                && (i < start_x_rod.size())) {
            int sign = 1;
            if (values_F[i].value < 0)
                sign = -1;

            bool first = false;
            bool second = false;
            for (int j = 0; j < values_q.size(); ++j)
                if ((values_q[j].rod == values_F[i].uzel) ||
                        (values_q[j].rod + 1 == values_F[i].uzel)) {
                    first = (values_q[j].rod == values_F[i].uzel);
                    second = (values_q[j].rod + 1 == values_F[i].uzel);
                }
            int sdvig = 0;
            if (first && (sign == 1))
                sdvig = -(start_x_rod[values_F[i].uzel - 1] + sign*(40 + 4) - (start_x_rod[values_F[i].uzel - 1] + sign*2)) - 6;
            if (second && (sign == -1))
                sdvig = -(start_x_rod[values_F[i].uzel - 1] + sign*(40 + 4) - (start_x_rod[values_F[i].uzel - 1] + sign*2)) + 6;

            int start_x = start_x_rod[values_F[i].uzel - 1] + sign*2 + sdvig + sdvig_x;
            int end_x = start_x_rod[values_F[i].uzel - 1] + sign*(40 + 4) + sdvig/* - sign*2*/ + sdvig_x;

            int width_strelki = abs(end_x - start_x)/2.;
//            int find_begin_x = (start_x > end_x ? end_x : start_x);

            //*** подписываю
            str = tr("%1F").arg(my_d(md(values_F[i].value), true));
            QGraphicsTextItem *text_rod = new QGraphicsTextItem(str);

            //******************************************
            int index_rod = 0/*values_F[i].uzel - 1*/;

            int left_rigth = 0;
            bool f_not_init = true;

            if (sign > 0) {
                if (values_F[i].uzel - 1 < real_size_w_rod.size()
                        && values_F[i].uzel - 1 >= 0) {
                    index_rod = values_F[i].uzel - 1;
                    if (values_F[i].uzel - 1 != 0)
                        f_not_init = false;
                }
                left_rigth++;
            }
            else if (sign < 0) {
                if (values_F[i].uzel - 2 < real_size_w_rod.size()
                        && values_F[i].uzel - 2 >= 0) {
                    index_rod = values_F[i].uzel - 2;
                    f_not_init = false;
                }
                left_rigth--;
            }

            if (sdvig == 0) {
//                index_rod = values_F[i].uzel - 1;
            } else if (sdvig < 0) {
                if (index_rod - 1 >= 0) {
                    --index_rod;
                    if (left_rigth == 1)
                        left_rigth = -1;
                }
            } else if (sdvig > 0) {
                if (index_rod + 1 < real_size_w_rod.size()) {
                    ++index_rod;
                    if (left_rigth == -1)
                        left_rigth = 1;
                }
            }

//            qDebug() << index_rod + 1 << " " << left_rigth;
            double size_h = real_size_h_rod[index_rod];

            double sdvig_str_h = 0;
            double sdvig_str_w = 0;
            if (!f_not_init && (size_h < 52
                    && (!(index_rod == 0 && left_rigth == 1) || !(index_rod == values_rod.size() - 1 && left_rigth == -1)))) {
                sdvig_str_w = 14;
                sdvig_str_h = -size_h - 15 + (left_rigth == -1 ? -20 : 0);

                if (left_rigth == -1)
                    if (index_rod + 1 <= real_size_w_rod.size())
                        ++index_rod;

                double my_tmp = 0;

                if (index_rod < real_size_w_rod.size())
                    my_tmp = real_size_w_rod[index_rod];

                double beg_x_line = start_x_rod[index_rod] + left_rigth*my_tmp*1./10. + (my_tmp == 0 ? -10 : 0) + sdvig_x;
                double x_beg = beg_x_line + (left_rigth == 1 ? 20 : -20);
                double x_end = x_beg + f.width(str)/**12./10.*/ + 2;
                scene->addLine(beg_x_line,
                               size_y/2.,
                               x_beg + (left_rigth == -1 ? (x_end - x_beg) : 0),
                               size_y/2. - sdvig_str_h,
                               QPen(QBrush(Qt::blue), 2));
                scene->addLine(x_beg,
                               size_y/2. - sdvig_str_h,
                               x_end,
                               size_y/2. - sdvig_str_h,
                               QPen(QBrush(Qt::blue), 2));
                double avarage = (x_beg + x_end)/2.;
                text_rod->setPos(avarage - f.width(str)/2. + (str.size() > 2 ? 2 : -2),
                                 size_y/2. - 19 - sdvig_str_h);
            } else {
                text_rod->setPos((start_x > end_x ? end_x : start_x) - f.width(str)/2./* + sdvig_x*/ + width_strelki + sdvig_str_w,
                                 size_y/2. - 25 + sdvig_str_h);
            }

            //******************************************
//            text_rod->setPos(find_begin_x - f.width(str)/2./* + sdvig_x*/ + width_strelki, size_y/2. - 20);
            scene->addItem(text_rod);
            //***

            scene->addLine(start_x, size_y/2., end_x, size_y/2., QPen(QBrush(Qt::blue), 4));
            scene->addLine(start_x_rod[values_F[i].uzel - 1] + sign*44 + sign*2 + sdvig + sdvig_x, size_y/2., start_x_rod[values_F[i].uzel - 1] + sign*(32 + 4) + sdvig + sdvig_x, size_y/2. + 6, QPen(QBrush(Qt::blue), 4));
            scene->addLine(start_x_rod[values_F[i].uzel - 1] + sign*44 + sign*2 + sdvig + sdvig_x, size_y/2., start_x_rod[values_F[i].uzel - 1] + sign*(32 + 4) + sdvig + sdvig_x, size_y/2. - 6, QPen(QBrush(Qt::blue), 4));
        }
    }

    if (add_h != 0) {
        add_elem_type(start_x_rod, real_size_w_rod, add_h,
                      size_y/2. + maxh/2.*ky + above*5./10. + (add_h - 29)*1./3.,
                      1);
        add_elem_type(start_x_rod, real_size_w_rod, add_h,
                      size_y/2. + maxh/2.*ky + above*5./10. + (add_h - 29)*2./3. - 15,
                      2);
        add_elem_type(start_x_rod, real_size_w_rod, add_h,
                      size_y/2. + maxh/2.*ky + above*5./10. + (add_h - 29)*3./3. - 30 - 1,
                      3);
    }
}

void MainWindow::add_elem_type(
                    const QVector<double>& start_x_rod,
                    const QVector<double>& real_size_w_rod,
                    int add_h, int center,
                    int type)
{
    QGraphicsPixmapItem *p_pix = NULL;
    QPixmap pix;

    if (type == 1) {
        pix = pixmap.copy(639, 414, 684-639, 431-414);
    } else if (type == 2) {
        pix = pixmap.copy(403, 405, 452-403, 437-405);
    } else if (type == 3) {
        pix = pixmap.copy(545, 408, 586-545, 437-408);
    }
    p_pix = scene->addPixmap(pix);
    p_pix->setPos(sdvig_x - pix.width() - 10, center - pix.height()/2.);

    int my_diapazon = (add_h - 100.)/3.;

    QVector<double> line_start_x;
    line_start_x.push_back(start_x_rod[0] + sdvig_x);

    QVector<double> line_start_y;
    line_start_y.push_back(center - my_diapazon/2.);
    line_start_y.push_back(center);
    line_start_y.push_back(center + my_diapazon/2.);

    QVector<double> line_end_x;
    line_end_x.push_back(start_x_rod[start_x_rod.size() - 2] + sdvig_x);

    QVector<double> line_end_y;
    line_end_y.push_back(center - my_diapazon/2.);
    line_end_y.push_back(center);
    line_end_y.push_back(center + my_diapazon/2.);

    //*-*-*-*-*-*-*-*-*-
    for (int i = 1; i < 2; ++i)
        scene->addLine(line_start_x[0], line_start_y[i], line_end_x[0], line_end_y[i], QPen(Qt::black));
    //*-*-*-*-*-*-*-*-*-

    QVector<double> *hei = NULL;

    switch (type) {
    case 1: hei = &v_Nx; break;
    case 2: hei = &v_ux; break;
    case 3: hei = &v_qx; break;
    }

    double max_h_y = 0;

    for (int i = 0; i < hei->size(); ++i) {
        if (max_h_y < fabs((*hei)[i]))
            max_h_y = fabs((*hei)[i]);
    }

    if (max_h_y == 0) {
        return;
    }

    double ky = my_diapazon/max_h_y/2.;

    int index_width = 0;

    double sum = 0;

    QVector<double> ver_x;
    QVector<double> ver_y;
    QVector<int> number_st;

    QString a_nujno_li[2];

    QString str;
    double step_line_w = 3;
    double step_sum = sdvig_x;
    for (int i = 0; i < v_x.size() - 1; ++i) {
        double tmp = 0;
        if (i != 0 && v_x[i] == 0) {
            sum += real_size_w_rod[index_width++];
        }
        if (v_x[i + 1] == 0) {
            tmp = real_size_w_rod[index_width];
        }
        double start_line_x = sum + real_size_w_rod[index_width]/values_rod[index_width].L*v_x[i] + sdvig_x;
        double start_line_y = line_start_y[1] - (*hei)[i]*ky;
        double end_line_x = sum + tmp + real_size_w_rod[index_width]/values_rod[index_width].L*v_x[i + 1] + sdvig_x;
        double end_line_y = line_start_y[1] - (*hei)[i + 1]*ky;
        scene->addLine(start_line_x,
                       start_line_y,
                       end_line_x,
                       end_line_y,
                       QPen(QBrush(Qt::blue), 1));

        for (double step = step_sum; (step >= start_line_x) && (step <= end_line_x); step += step_line_w, step_sum += step_line_w) {
            double y1 = start_line_y;
            double y2 = end_line_y;
            double x1 = start_line_x;
            double x2 = end_line_x;

            double line_x = step;
            double line_y = (line_x - x1)*(y2 - y1)/(x2 - x1) + y1;

            ver_x.push_back(line_x);
            ver_y.push_back(line_y);
            number_st.push_back(index_width);
        }

        //***

        if (v_x[i] == 0) {
            if ((md((*hei)[i], 2) != 0)) {
                double mas[4] = {sum + sdvig_x, line_start_y[1] - (*hei)[i]*ky/1.5,
                                 sum + 30 + sdvig_x, line_start_y[1] - ((*hei)[i]*ky/2. < 0 ? 1 : -1)*20 };
                str = tr("%1").arg(md(md(fabs((*hei)[i]), 4), "no"));
                a_nujno_li[0] = str;
                if ((*hei)[i] < 0)
                    a_nujno_li[0] += "-";
                if (a_nujno_li[0] != a_nujno_li[1]) {
                    scene->addLine(mas[0], mas[1],
                                   mas[2], mas[3]);
                    QFontMetrics f(str);
                    QGraphicsTextItem *text = new QGraphicsTextItem(str);
                    text->setPos(mas[2] + 3, mas[3] - f.height() + 10);
                    scene->addItem(text);
                }

            }
        }

        if (v_x[i + 1] == 0 || i == v_x.size() - 2) {
            if ((md((*hei)[i + (i == v_x.size() - 2 ? 1 : 0)], 2) != 0)) {
                double mas[4] = {sum + real_size_w_rod[index_width] + sdvig_x,
                                 line_start_y[1] - (*hei)[i]*ky/3.5,
                                 sum + real_size_w_rod[index_width] - 30 + sdvig_x,
                                 line_start_y[1] - ((*hei)[i]*ky/4. < 0 ? 1 : -1)*30 };
                str = tr("%1").arg(md(md(fabs((*hei)[i + (i == v_x.size() - 2 ? 1 : 0)]), 4), "no"));
                a_nujno_li[1] = str;
                if ((*hei)[i + (i == v_x.size() - 2 ? 1 : 0)] < 0)
                    a_nujno_li[1] += "-";
                if (a_nujno_li[0] != a_nujno_li[1]) {
                    scene->addLine(mas[0], mas[1],
                                   mas[2], mas[3]);
                    QFontMetrics f(str);
                    QGraphicsTextItem *text = new QGraphicsTextItem(str);
                    text->setPos(mas[2] - 3 - f.width(str), mas[3] - f.height() + 10);
                    scene->addItem(text);
                }
            }
        }
        //***
    }

    QVector<double> plus_begin_y;
    plus_begin_y.resize(values_rod.size());
    QVector<double> minus_begin_y;
    minus_begin_y.resize(values_rod.size());

    QVector<double> plus_begin_x;
    plus_begin_x.resize(values_rod.size());
    QVector<double> minus_begin_x;
    minus_begin_x.resize(values_rod.size());

    QVector<double> plus_end_y;
    plus_end_y.resize(values_rod.size());
    QVector<double> plus_end_x;
    plus_end_x.resize(values_rod.size());
    QVector<double> plus_flag;
    plus_flag.resize(values_rod.size());

    QVector<double> minus_end_y;
    minus_end_y.resize(values_rod.size());
    QVector<double> minus_end_x;
    minus_end_x.resize(values_rod.size());
    QVector<double> minus_flag;
    minus_flag.resize(values_rod.size());

    double diametr = 15;
    for (int i = 0; i < values_rod.size(); ++i) {
        plus_begin_y[i] = minus_begin_y[i]
        = plus_end_y[i] = minus_end_y[i]
        = plus_begin_x[i] = minus_begin_x[i]
          = plus_end_x[i] = minus_end_x[i] = -1;
        plus_flag[i] = minus_flag[i] = true;
    }

    for (int i = 0; i < ver_x.size(); ++i) {
        scene->addLine(ver_x[i], ver_y[i], ver_x[i], line_start_y[1]/*, QPen(color(number_st[i]))*/);
        if (plus_begin_y[number_st[i]] == -1) {
            if (line_start_y[1] - ver_y[i] >= diametr) {
                plus_begin_y[number_st[i]] = i/*fabs(ver_y[i] - line_start_y[1])*/;
                plus_begin_x[number_st[i]] = ver_x[i];
            }
        }
        if (plus_begin_y[number_st[i]] != -1) {
            if (plus_flag[number_st[i]] && (line_start_y[1] - ver_y[i] >= diametr)) {
                plus_end_y[number_st[i]] = i/*fabs(ver_y[i] - line_start_y[1])*/;
                plus_end_x[number_st[i]] = ver_x[i];
            } else
                plus_flag[number_st[i]] = false;
        }

        if (minus_begin_y[number_st[i]] == -1) {
            if (ver_y[i] - line_start_y[1] >= diametr) {
                minus_begin_y[number_st[i]] = i/*fabs(ver_y[i] - line_start_y[1])*/;
                minus_begin_x[number_st[i]] = ver_x[i];
            }
        }
        if (minus_begin_y[number_st[i]] != -1) {
            if (minus_flag[number_st[i]] && (ver_y[i] - line_start_y[1] >= diametr)) {
                minus_end_y[number_st[i]] = i/*fabs(ver_y[i] - line_start_y[1])*/;
                minus_end_x[number_st[i]] = ver_x[i];
            } else
                minus_flag[number_st[i]] = false;
        }
    }

    for (int i = 0; i < values_rod.size(); ++i) {
        if ((plus_begin_y[i] != -1)
                && (fabs(plus_begin_x[i] - plus_end_x[i]) >= diametr)) {
            double ellipse_sx = plus_begin_x[i] + fabs(plus_end_x[i] - plus_begin_x[i])/2. - diametr/2.;
            double ellipse_sy = line_start_y[1] - fabs(ver_y[(plus_end_y[i] + plus_begin_y[i])/2] - line_start_y[1])/2.
                    - diametr/2.;
            scene->addEllipse(ellipse_sx,
                              ellipse_sy,
                              diametr, diametr, QPen(), QBrush(Qt::white));
            str = "+";
            QFontMetrics f(str);
            QGraphicsTextItem *text = new QGraphicsTextItem(str);
            text->setPos(ellipse_sx + diametr/2. - f.width(str) + 4, ellipse_sy + diametr/2. - f.height() + 8);
            scene->addItem(text);
        }
        if ((minus_begin_y[i] != -1)
                && (fabs(minus_begin_x[i] - minus_end_x[i]) >= diametr)) {
            double ellipse_sx = minus_begin_x[i] + fabs(minus_end_x[i] - minus_begin_x[i])/2. - diametr/2.;
            double ellipse_sy = line_start_y[1] + fabs(ver_y[(minus_end_y[i] + minus_begin_y[i])/2] - line_start_y[1])/2.
                    - diametr/2.;
            scene->addEllipse(ellipse_sx,
                              ellipse_sy,
                              diametr, diametr, QPen(), QBrush(Qt::white));
            str = "-";
            QFontMetrics f(str);
            QGraphicsTextItem *text = new QGraphicsTextItem(str);
            text->setPos(ellipse_sx + diametr/2. - f.width(str) + 1, ellipse_sy + diametr/2. - f.height() + 8);
            scene->addItem(text);
        }
    }
}
