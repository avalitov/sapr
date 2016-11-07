#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "init.cpp"
#include "graphics1.cpp"
#include "error1.cpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    all = new QHBoxLayout;
//    ui->centralWidget->setLayout(all);

    pixmap.load("rw.png");
    pixmap.createMaskFromColor(QColor(255, 255, 255));
    // инициализация сцены
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setGeometry(0, 250 - 30, 1000, 350);
    // инициализация таблицы стержней

    move_table_y = 21;
    v_processore = false;

    // задаём названия горизонтальных стобцов
    QStringList h_rod_header;
    h_rod_header << "Длина, L" << "Поперечное сечение, A"
             << "Модуль упругости, E" << "Напряжение, σ";

    // задаём названия вертикальных строк
    QStringList v_rod_header;
    v_rod_header << "*";

    // задаём размеры горизонтальных строк
    size_rod_width << 65 + 15 << 150 + 15 << 120 + 15 << 85 + 15;

    // инициализируем таблицу стержней
    init_table(&table_rod,
               h_rod_header,
               v_rod_header,
               size_rod_width,
               QPoint(0, move_table_y));

    // инициализация таблицы усилий F

    // задаём названия горизонтальных стобцов
    QStringList h_F_header;
    h_F_header << "номер узла" << "значение, F";

    // задаём названия вертикальных строк
    QStringList v_F_header;
    v_F_header << "*";

    // задаём размеры горизонтальных строк
    size_F_width << 85 << 65 + 15;

    // инициализируем таблицу усилий F
    init_table(&table_F,
               h_F_header,
               v_F_header,
               size_F_width,
               QPoint(sum(size_rod_width) + 20, move_table_y));


    // инициализация таблицы статических погонных нагрузок q

    // задаём названия горизонтальных стобцов
    QStringList h_q_header;
    h_q_header << "номер стержня" << "значение, q";

    // задаём названия вертикальных строк
    QStringList v_q_header;
    v_q_header << "*";

    // задаём размеры горизонтальных строк
    size_q_width << 100 << 65 + 15;

    // инициализируем таблицу статических погонных нагрузок q
    init_table(&table_q,
               h_q_header,
               v_q_header,
               size_q_width,
               QPoint(sum(size_rod_width) + 20 + sum(size_F_width) + 20, move_table_y));

    of_timer = false;
    of_timer_button = false;
    is_push_button = 0;
    sdvig_button = 10;
    v_post_processore = false;

    timer_button = new QTimer(this);
    timer = new QTimer(this);
    timer_load = new QTimer(this);

    old_current_item = NULL;
    old_current_table = NULL;
    rascheti = NULL;

    current_table = table_rod;
    current_item = current_table->item(0, 0);
    current_table->setCurrentItem(current_item);

    of_once = true;
    table_change(current_item);
    of_once = false;

    QObject::connect(timer,
                     SIGNAL( timeout()   ),
                     SLOT(   my_timer()  ));
    QObject::connect(timer_button,
                     SIGNAL( timeout()          ),
                     SLOT(   my_timer_button()  ));
    QObject::connect(timer_load,
                     SIGNAL( timeout() ),
                     SLOT(my_timer_load()));
    on();

    ui->pushButton_3->setStyleSheet(QString::fromUtf8("background-color: white;"
                                                      "border-style: outset;"
                                                      "border-width: 2px;"
                                                      "border-radius: 10px;"
                                                      "border-color: beige;"
                                                      "font: bold 12px;"
                                                      "min-width: 10em;"
                                                      /*"padding: 6px;"*/));

    ui->pushButton_4->setStyleSheet(QString::fromUtf8("background-color: white;"
                                                      "border-style: outset;"
                                                      "border-width: 2px;"
                                                      "border-radius: 10px;"
                                                      "border-color: beige;"
                                                      "font: bold 12px;"
                                                      "min-width: 10em;"
                                                      /*"padding: 6px;"*/));
    ui->pushButton_5->setStyleSheet(QString::fromUtf8("background-color: white;"
                                                      "border-style: outset;"
                                                      "border-width: 2px;"
                                                      "border-radius: 10px;"
                                                      "border-color: beige;"
                                                      "font: bold 12px;"
                                                      "min-width: 10em;"
                                                      /*"padding: 6px;"*/));

    ui->pushButton->setShortcut(QKeySequence::Delete);
//    qDebug() << ui->graphicsView->x() << " " << ui->graphicsView->height();
    int y_sdvig = ui->graphicsView->y() + ui->graphicsView->height() + 25;
    ui->pushButton_2->move(780, y_sdvig - ui->pushButton_2->height()/2.);
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();

    ui->label->move(100, 20);
    ui->label->hide();
    ui->pushButton_3->move(220 - ui->pushButton_3->width(), y_sdvig - ui->pushButton_3->height()/2.);
    of_processor();

    ui->graphicsView->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/ );
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->scroll(0, 0);

    ui->lineEdit_2->hide();
    ui->label_7->hide();
    ui->label_2->hide();
    QRegExp exp("^[0-9]{0,2}$");
    ui->lineEdit_2->setValidator(new QRegExpValidator(exp, this));
    QObject::connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(lineEdit_2_textChanged(QString)));
    ui->label_3->move(100, y_sdvig - ui->label_3->height()/2.);
    ui->checkBox->move(100 + ui->label_3->width() + 20, y_sdvig - ui->checkBox->height()/2.);
    ui->checkBox_2->move(100 + ui->label_3->width() + 20 + ui->checkBox->width() + 20, y_sdvig - ui->checkBox_2->height()/2.);


    ui->pushButton_5->hide();

//    ui->pushButton_6->hide();
//    sdvig_po_x += ui->button_n6->width() + 15;
    ui->comboBox->hide();
    ui->doubleSpinBox->hide();
    ui->doubleSpinBox->setSingleStep(0.01);

    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();

    ui->lineEdit->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();

    of_load = false;

    size_wid_frame = size_wid_frame_first = 1000;

//    resize_hor_tables();

    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(set_interval(int)));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(new_value()));
    QObject::connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(new_value()));
    ui->actionOpen->setShortcut(QKeySequence::Open);
    QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(act_open()));
}

void MainWindow::on() {
    QObject::connect(table_rod, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));
    QObject::connect(table_q,   SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));
    QObject::connect(table_F,   SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));

    QObject::connect(table_rod, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));
    QObject::connect(table_q,   SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));
    QObject::connect(table_F,   SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));

    QObject::connect(ui->checkBox,      SIGNAL(clicked(bool)),  this,   SLOT(otrisovka()));
    QObject::connect(ui->checkBox_2,    SIGNAL(clicked(bool)),  this,   SLOT(otrisovka()));
    timer->start(30);
}

void MainWindow::off() {
    timer->stop();
    QObject::disconnect(table_rod,  SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));
    QObject::disconnect(table_q,    SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));
    QObject::disconnect(table_F,    SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(table_change(QTableWidgetItem*)));

    QObject::disconnect(table_rod,  SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));
    QObject::disconnect(table_q,    SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));
    QObject::disconnect(table_F,    SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(select_table(QTableWidgetItem*)));

    QObject::disconnect(ui->checkBox,   SIGNAL(clicked(bool)),  this,   SLOT(otrisovka()));
    QObject::disconnect(ui->checkBox_2, SIGNAL(clicked(bool)),  this,   SLOT(otrisovka()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::table_change(QTableWidgetItem *item)
{
    off();

    int row = item->row();
    int col = item->column();

    QTableWidgetItem *tmp_item = item;

    if ((!of_timer_button) && (!of_once)) {
        if (col == current_table->columnCount() - 1) {
            if (current_table->item(row + 1, 0)) {
                item = current_table->item(row + 1, 0);
                current_table->setCurrentItem(item);
            }
        } else
            if (current_table->item(row, col + 1)) {
                item = current_table->item(row, col + 1);
                current_table->setCurrentItem(item);
            }
    }

    select_table(item);

    if (!of_timer) {

        otrisovka(tmp_item);

        of_processor();
    }

    on();
}

void MainWindow::select_table(QTableWidgetItem *item)
{
    off();

    current_item  = item;
    old_current_table = current_table;
    current_table = current_item->tableWidget();

    if (current_table != table_rod)
        table_rod->setCurrentItem(NULL);
    if (current_table != table_F)
        table_F->setCurrentItem(NULL);
    if (current_table != table_q)
        table_q->setCurrentItem(NULL);

    update_button();

    on();
}

void MainWindow::my_timer_button()
{
    off();
    of_timer_button = true;
    ++is_push_button;

    current_item = opredelenie_item();
    table_change(current_item);

    bool f = (old_current_table != current_table);
    old_current_item = current_table->currentItem();
    if (f) current_table->setCurrentItem(NULL);

    timer_button->stop();
    of_timer_button = false;

    //*** а убирать ли выделение ячейки? удобно ли это?
//    if (current_item) {
//        old_current_item = current_item;
//        current_table->setCurrentItem(NULL);
//    }
    //***

    on();
}

void MainWindow::my_timer_load()
{
    off();

    table_rod->setCurrentItem(NULL);
    table_F->setCurrentItem(NULL);
    table_q->setCurrentItem(NULL);

    timer_load->stop();

    on();
}

void MainWindow::otrisovka(QTableWidgetItem *tmp_item)
{
    if (tmp_item == NULL)
        of_processor();

    scene = new QGraphicsScene(this);

    ui->graphicsView->setGeometry(0, 250 - 30, 1000, 350);

    if (current_table == table_rod) {
        values_rod.clear();
        change(tmp_item, 0);
        values_F.clear();
        change(NULL, 1);
        values_q.clear();
        change(NULL, 2);
    }
    if (current_table == table_F) {
        values_F.clear();
        change(tmp_item, 1);
        values_rod.clear();
        change(NULL, 0);
        values_q.clear();
        change(NULL, 2);
    }
    if (current_table == table_q) {
        values_q.clear();
        change(tmp_item, 2);
        values_rod.clear();
        change(NULL, 0);
        values_F.clear();
        change(NULL, 1);
    }

    add_elem();

    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::my_timer()
{
    off();
    of_timer = true;

    if (current_table->currentItem() != NULL) {
        current_item = current_table->currentItem();
    }
    else current_item = old_current_item;
    update_button();

    resize_hor_tables();
    of_timer = false;

    on();
}

void MainWindow::on_pushButton_clicked()
{
    off();
    if (current_table->rowCount() > 0) {
        if (current_table->rowCount() == 1) {
            current_table->setRowCount(1);
            for (int col = 0; col < current_table->columnCount(); ++col)
                init_item(current_table, 0, col);
            current_table->setVerticalHeaderItem(0, new QTableWidgetItem("*"));
        } else
            current_table->removeRow(current_item->row());

        num_tables();

        ++is_push_button;
        timer_button->start(5);
        is_push_button = 0;
    }
    on();
}

QTableWidgetItem *MainWindow::opredelenie_item() {
    off();
    int y = ui->pushButton->y() - 22;
    const int y_height = 30;

    int row = y + ui->pushButton->height()/2.;

    int find_row = row/y_height + 1;
    int find_column = 0;
    if (current_table->currentItem())
        find_column = current_table->currentItem()->column();

    for (int i = 0; i < current_table->rowCount(); ++i)
        if ((row >= current_table->rowViewportPosition(i))
                && (row <= current_table->rowViewportPosition(i) + current_table->rowHeight(i))) {
            find_row = i;
        }

    while (find_row >= current_table->rowCount())
        --find_row;

    while (find_column >= current_table->columnCount())
        --find_column;

    QTableWidgetItem *res = current_table->item(find_row, find_column);
    current_table->setCurrentItem(res);

    on();
    return res;
}

bool MainWindow::perexodim()
{
    if (!ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()) {
        ui->pushButton_2->setStyleSheet(QString::fromUtf8("background-color: red;"
                                                          "border-style: outset;"
                                                          "border-width: 2px;"
                                                          "border-radius: 10px;"
                                                          "border-color: beige;"
                                                          "font: bold 12px;"
                                                          "min-width: 10em;"
                                                          /*"padding: 6px;"*/));
        return false;
    }
    return true;
}

void MainWindow::num_tables()
{
    off();
    num_table(table_rod);
    num_table(table_F);
    num_table(table_q);
    on();
}

void MainWindow::num_table(QTableWidget *table)
{
    off();
    int n = 0;
    QString str = "";
    for (int i = 0; i < table->rowCount() - 1; ++i) {
        bool find = false;
        for (int j = 0; j < table->columnCount(); ++j)
            if (table->item(i, j)->backgroundColor() == Qt::red) {
                find = true;
                break;
            }
        if (find) {
            str = "*";
            table->setVerticalHeaderItem(i, new QTableWidgetItem(str));
        } else {
            str = tr("%1").arg(++n);
            table->setVerticalHeaderItem(i, new QTableWidgetItem(str));
        }
    }
    if (str != "") {
        QFontMetrics f(str);
        bool vertical_visible = table->verticalScrollBar()->isVisible();
        QVector<int> size_table;
        if (current_table == table_rod)
            size_table = size_rod_width;
        if (current_table == table_F)
            size_table = size_F_width;
        if (current_table == table_q)
            size_table = size_q_width;

        int new_size = sum(size_table) + f.width(str) + 9
                + (str.size() >= 2 ? -2*(str.size() - 1) : 0)
                + (vertical_visible ? table->verticalScrollBar()->width() - 1 : 0);
//        qDebug() << "vertical_visible = " << table->verticalScrollBar()->width();
        if (current_table->width() != new_size) {
            current_table->resize(new_size, current_table->height());
        }
    }
    on();
}

void MainWindow::update_button(QTableWidget *table)
{
    off();
    if (table == NULL)
        table = current_table;
    int dy = table->rowViewportPosition(current_item->row())
            + table->rowHeight(current_item->row())/2. - ui->pushButton->height()/2.;
    int x = 0;
    int y = 23 + dy;
    bool new_move = false;
    int new_F_x = 0;
    int new_q_x = 0;

    if (table == table_rod) {
        x = table_rod->width() + sdvig_button;
        new_F_x = x + ui->pushButton->width() + sdvig_button;
        new_q_x = new_F_x + table_F->width() + 5;

        new_move = (ui->pushButton->x() != x) || (ui->pushButton->y() != y);
    }
    if (table == table_F) {
        x = table_rod->width() + 5 + table_F->width() + sdvig_button;
        new_F_x = table_rod->width() + 5;
        new_q_x = x + ui->pushButton->width() + sdvig_button;

        new_move = (ui->pushButton->x() != x) || (ui->pushButton->y() != y);
    }
    if (table == table_q) {
        x = table_rod->width() + 5 + table_F->width() + 5 + table_q->width() + sdvig_button;
        new_F_x = table_rod->width() + 5;
        new_q_x = new_F_x + table_F->width() + 5;

        new_move = (ui->pushButton->x() != x) || (ui->pushButton->y() != y);
    }

    if (new_move) {
        if (y > table->height() - (ui->pushButton->height()/2.))
            ui->pushButton->hide();
        else
            ui->pushButton->show();

        table_F->move(new_F_x, move_table_y);
        table_q->move(new_q_x, move_table_y);

        ui->pushButton->move(x, y);
    }
    on();
}

void MainWindow::resize_hor_tables()
{
    off();

    if (old_current_table != current_table) {
        bool bolshe = false;

        if ((current_table == table_F) && (old_current_table == table_rod))
            bolshe = true;
        if (!bolshe)
        if ((current_table == table_q) && (old_current_table == table_F))
            bolshe = true;
        if (!bolshe)
            if ((current_table == table_q) && (old_current_table == table_rod))
                bolshe = true;
        if (bolshe) {
//            qDebug() << table_text(old_current_table) << " " << table_text(current_table);
            int promejutok = ui->pushButton->width() + sdvig_button*2;

            current_table->cursor().setPos(current_table->cursor().pos().x() - promejutok/* - sdvig_button*2 - ui->pushButton->width()*/,
                               current_table->cursor().pos().y());
        }
        select_table(current_item);
    }
    resize_hor_table(table_rod);
    resize_hor_table(table_F);
    resize_hor_table(table_q);
    on();
}

void MainWindow::resize_hor_table(QTableWidget *table)
{
    QVector<int>* size_table = NULL;
    if (table == table_rod)
        size_table = &size_rod_width;
    if (table == table_F)
        size_table = &size_F_width;
    if (table == table_q)
        size_table = &size_q_width;

    for (int col = 0; col < table->columnCount(); ++col) {
        int new_size_col = table->columnWidth(col);
        if (new_size_col != (*size_table)[col]) {
            int new_width_table = table->width() + new_size_col - (*size_table)[col];
//            QTableWidget *table2 = (table == table_rod ? table_F : (table == table_F ? table_rod : table_rod));
//            QTableWidget *table3 = (((table == table_rod)  && (table2 == table_F))   ? table_q
//                                  : ((table == table_F)    && (table2 == table_q))   ? table_rod
//                                  : ((table == table_q)    && (table2 == table_rod)) ? table_F
//                                  : ((table2 == table_rod) && (table == table_F))    ? table_q
//                                  : ((table2 == table_F)   && (table == table_q))    ? table_rod
//                                  : ((table2 == table_q)   && (table == table_rod))  ? table_F
//                                  : NULL);
            int promejutok = ui->pushButton->width();

            if (current_table == table_rod)
                promejutok += 5 + sdvig_button*2;

            if (current_table == table_F)
                promejutok += 5 + sdvig_button*2;

            if (current_table == table_q)
                promejutok += 5*2 + sdvig_button - 1;

            // делаем небольшой запас, учитывая все Scroll'ы,
            // расширения ширины вертикальных столцов
            promejutok += 9*3 + 17*3;

            /*if ((new_width_table + table2->width() + table3->width() + promejutok > 1000)
                    && (new_size_col > (*size_table)[col])) {
                static bool is_draw = true;
                if (is_draw) {
                    QMessageBox::critical(
                            this,
                            "Внимание!",
                            "Нельзя выходить за размеры окна!",
                            QMessageBox::Ok);
                    is_draw = false;
                }
//                (*size_table)[col] = 1000 - (table2->width() + table3->width() + promejutok)
//                                     - (sum(*size_table) - (*size_table)[col]);
                table->setColumnWidth(col, (*size_table)[col]);
            } else */{
                table->resize(new_width_table, table->height());
                (*size_table)[col] = new_size_col;
                table->setColumnWidth(col, (*size_table)[col]);

                if (current_table != table) {
                    table->setCurrentItem(table->item(0, 0));
                    select_table(table->item(0, 0));
                } else
                    update_button(table);
                int new_size_menu = promejutok + sum(size_rod_width) + sum(size_F_width) + sum(size_q_width);
                if (new_size_menu >= size_wid_frame) {
                    size_wid_frame_first = new_size_menu;
                    this->setFixedWidth(size_wid_frame_first);
                }
                if (new_size_menu > this->width()) {
                    ui->menuBar->resize(new_size_menu, ui->menuBar->height());
                }
            }
        }
    }
}

QString MainWindow::table_text(QTableWidget *table)
{
    QString res = "NULL";
    if (table == table_rod)
        res = "table_rod";
    if (table == table_F)
        res = "table_F";
    if (table == table_q)
        res = "table_q";
    return res;
}

void MainWindow::on_pushButton_3_clicked()
{
//    ui->graphicsView = new QGraphicsView(this);
//    ui->graphicsView->setScene(scene);

//    scene = new QGraphicsScene(this);
//    ui->graphicsView->setGeometry(0, 250, 1000, 350);
//    add_elem();
//    ui->graphicsView->setScene(scene);


    ui->lineEdit_2->setText("1");

    otrisovka(NULL);
//    ui->graphicsView->setScene(scene);

    this->setFixedWidth(size_wid_frame_first);
    ui->actionOpen->setEnabled(true);
    ui->actionSave->setEnabled(true);
    v_processore = false;
    ui->doubleSpinBox->hide();
    ui->comboBox->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
    ui->lineEdit->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    rascheti->hide();
    ui->lineEdit_2->hide();
    ui->label_7->hide();
    ui->label_2->hide();
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    table_rod->show();
    table_F->show();
    table_q->show();
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->checkBox->show();
    ui->checkBox_2->show();
    ui->label_3->show();

    this->setFixedHeight(650);

    //***
    for (int i = 0; i < lbl.size(); ++i) {
        lbl[i]->hide();
        delete lbl[i];
    }
    lbl.clear();
    UL.clear();
    U0.clear();

    //***

    on();
}

void MainWindow::new_table_za(QTableWidget *new_table_za, int new_col)
{
    for_za = new_table_za;
    for_za_i = 0;
    for_za_j = 0;
    for_za_col = new_col;
}

void MainWindow::za(double value)
{
    int j = for_za_j++ % for_za_col;
    if (j == 0 && for_za_j != 1)
        ++for_za_i;
    for_za->item(for_za_i, j)->setText(tr("%1").arg(value));
}

void MainWindow::za(QString value)
{
    int j = for_za_j++ % for_za_col;
    if (j == 0 && for_za_j != 1)
        ++for_za_i;
    for_za->item(for_za_i, j)->setText(value);
}

void MainWindow::on_pushButton_4_clicked()
{
    line_edit = ui->lineEdit_2->text().toInt();
//    if (ui->lineEdit_2->text().toInt() == 0)
    ui->lineEdit_2->setText(tr("%1").arg(99));

    v_post_processore = true;
//    ui->actionSave->setEnabled(false);
    v_processore = false;
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
    ui->label->hide();
    ui->label_2->hide();

    ui->lineEdit->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();

    ui->lineEdit_2->hide();
    ui->label_7->hide();
    rascheti->hide();
    for (int i = 0; i < lbl.size(); ++i) {
        lbl[i]->hide();
    }

    //***
    scene = new QGraphicsScene(this);
    int dob_hei = 700;
    add_elem(dob_hei);
    ui->graphicsView->setGeometry(0, 0, 1000, 350 + dob_hei);
    ui->graphicsView->setScene(scene);
    //***
    ui->graphicsView->show();

    int sdvig_y = ui->graphicsView->height() + 30;
    ui->pushButton_5->move(220 - ui->pushButton_5->width(), sdvig_y - ui->pushButton_5->height()/2.);
    ui->pushButton_5->show();

    sdvig_y += ui->pushButton_5->height();

    this->setFixedHeight(sdvig_y + 30);

    ui->comboBox->hide();
    ui->doubleSpinBox->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->label_6->hide();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->lineEdit_2->setText(tr("%1").arg(line_edit));
    ui->graphicsView->hide();
    ui->pushButton_5->hide();
    v_processore = true;
    v_post_processore = false;
//    ui->actionSave->setEnabled(true);

    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->label->show();

    draw_table();
}

void MainWindow::act_open() {
    QString str = QFileDialog::getOpenFileName(this, "открытие файла", "", "*.md");
    if (str.isEmpty())
        return;

    QFile file(str);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::information(this, "Ошибка!", "Файл не открылся!");
        return;
    }

    QTextStream stream(&file);

    QVector< QVector<QString> > m[3];

    int index = -1;

    while (!stream.atEnd()) {
        QTextStream buf;
        QString temp = stream.readLine();
        buf.setString(&temp);

        if (temp == "rod"
            || temp == "F"
            || temp == "q") {
            ++index;
            continue;
        }
        if (temp == "zadelki")
            break;

        QVector<QString> tmp_vec;

        while (!buf.atEnd()) {
            QString my_str;
            buf >> my_str;
            if (my_str == "_")
                tmp_vec.push_back("");
            else tmp_vec.push_back(my_str);
        }
        m[index].push_back(tmp_vec);
    }

    int mas_zadelki[2];
    QString temp = stream.readLine();
    QTextStream buf;
    buf.setString(&temp);

    buf >> mas_zadelki[0];
    buf >> mas_zadelki[1];

    of_load = true;
    off();
    QTableWidget* table = table_rod;
    table->setRowCount(m[0].size());

    for (int i = 0; i < table->rowCount(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            init_item(table, i, j);

    new_table_za(table, table->columnCount());

    for (int i = 0; i < m[0].size(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            za(m[0][i][j]);

    table = table_F;
    table->setRowCount(m[1].size());

    for (int i = 0; i < table->rowCount(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            init_item(table, i, j);

    new_table_za(table, table->columnCount());

    for (int i = 0; i < m[1].size(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            za(m[1][i][j]);

    table = table_q;
    table->setRowCount(m[2].size());

    for (int i = 0; i < table->rowCount(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            init_item(table, i, j);

    new_table_za(table, table->columnCount());

    for (int i = 0; i < m[2].size(); ++i)
        for (int j = 0; j < table->columnCount(); ++j)
            za(m[2][i][j]);

    ui->checkBox->setChecked(mas_zadelki[0]);
    ui->checkBox_2->setChecked(mas_zadelki[1]);

    select_table(table_rod->item(0, 0));
    table_change(table_rod->item(0, 0));
    old_current_item = table_rod->item(0, 0);
    table_rod->setCurrentItem(table_rod->item(0, 0));
    timer_load->start(150);

    on();
    of_load = false;
}

void MainWindow::on_actionSave_triggered()
{
//    qDebug() << v_processore << " " << v_post_processore;
    if (!v_processore && !v_post_processore) {
        QString str = QFileDialog::getSaveFileName(this, "сохранить файл", "", "*.md");
        if (str.isEmpty())
            return;

        if (str.contains(".md"))
            str.remove(".md");

        str += ".md";
        QFile file(str);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::information(this, "Ошибка!", "Файл не сохранился!");
            return;
        }

        QTextStream stream(&file);

        stream << "rod" << endl;
        ferger(stream, table_rod);

        stream << "F" << endl;
        ferger(stream, table_F);

        stream << "q" << endl;
        ferger(stream, table_q);

        stream << "zadelki" << endl;
        stream << int(ui->checkBox->isChecked()) << " ";
        stream << int(ui->checkBox_2->isChecked());

        file.close();
    } else if (!v_post_processore) {
        QString str = QFileDialog::getSaveFileName(this, "сохранить файл", "", "*.csv");
        if (str.isEmpty())
            return;
        if (str.contains(".csv"))
            str.remove(".csv");

        str += ".csv";
        QFile file(str);

        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::information(this, "Ошибка!", "Файл не сохранился!");
            return;
        }

        QTextStream stream(&file);

        stream << "number;" << "x;" << "Nx;" << "ux;" << "qx;" << "[q];" << endl;

        ferger(stream, rascheti, true);

        file.close();
    } else {
        QString str = QFileDialog::getSaveFileName(this, "сохранить файл", "", "*.png");
        if (str.isEmpty())
            return;

        if (str.contains(".png"))
            str.remove(".png");

        str += ".png";
        QFile file(str);

        if (!file.open(QFile::WriteOnly)) {
            QMessageBox::information(this, "Ошибка!", "Файл не сохранился!");
            return;
        }

        QImage image(scene->width(), scene->height(), QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&image);
        scene->render(&painter);
        image.save(str);
    }
}

void MainWindow::on_action_2_triggered()
{
    exit(0);
}
