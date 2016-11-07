#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
//#include <QGraphicsItem>
#include <QVector>
#include <QTableWidget>
#include <QStringList>
#include <QMessageBox>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QScrollBar>
#include <QFont>
#include <QFontMetrics>
#include <QMouseEvent>
#include <QWidget>
#include <QPalette>
#include <QStyleFactory>
#include <QTableView>
#include <my_matrix.h>
#include <QLabel>
#include <QRegExp>
#include <QScrollArea>
#include <QTableView>
#include <QString>
#include <QLineEdit>
#include <QFileDialog> //show
#include <QFile>
#include <QTextStream>
#include <QBitmap>
//#include <QList>

//#define  MyDBG  {qDebug() << qPrintable(my_otstup) << __LINE__ << qPrintable(" ") << __FUNCTION__; my_otstup += "   "; }
//#define  MyDBG2  {qDebug() << __LINE__ << __FUNCTION__; }
//#define  MyDBG1 {my_otstup.remove(0, 3); qDebug() << qPrintable(my_otstup) << __LINE__ << qPrintable(" ") << __FUNCTION__; }

class cRod {
public:
    double L;
    double A;
    double E;
    double G;
    double H;

    cRod() {}
    cRod(double L, double A, double E, double G)
        : L(L), A(A), E(E), G(G) {
        H = A;
    }
};

class cF {
public:
    int uzel;
    double value;

    cF() {}
    cF(int uzel, double value)
        : uzel(uzel), value(value) {
    }
};

class cq {
public:
    int rod;
    double value;

    cq() {}
    cq(int rod, double value)
        : rod(rod), value(value) {
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString my_otstup = "";
// для препроцессора
    QTableWidget *table_rod;
    QTableWidget *table_F;
    QTableWidget *table_q;
    QTableWidget *current_table;
    QTableWidget *old_current_table;
    QTableWidgetItem *current_item;
    QTableWidgetItem *old_current_item;

    QPixmap pixmap;
    int move_table_y;

//    QGraphicsPixmapItem *pix;
    QGraphicsScene *scene;
//    QGraphicsScene *my_scene;
//    QGraphicsView *my_view;
    double kx, ky;

    int line_edit;

    QVector<cRod> values_rod;
    QVector<int> size_rod_width;

    QVector<cF>   values_F;
    QVector<int> size_F_width;

    QVector<cq> values_q;
    QVector<int> size_q_width;

    int sdvig_x;
//    int sdvig_y;
    void add_elem(int add_h = 0);
    void add_elem_type(const QVector<double> &start_x_rod,
                       const QVector<double> &real_size_w_rod,
                       int add_h, int center, int type);
    void init_table(QTableWidget **table,
                    const QStringList &header_horizontal,
                    const QStringList &header_vertical,
                    const QVector<int> &size_horizontal,
                    const QPoint& size);

    int sum(const QVector<int>&);
    Ui::MainWindow *ui;
    void change(QTableWidgetItem* item,
                const int &table_number = 0);
    void on();
    void off();

    QTableWidgetItem * opredelenie_item();
    QTimer* timer;
    QTimer* timer_button;
    QTimer* timer_load;
    int is_push_button;
    bool of_timer;
    bool of_timer_button;
    bool of_load;
    int sdvig_button;
    int of_once;
// для процессора
    bool v_processor;
    bool v_processore;
    bool v_post_processore;
    bool empty_table;
    my_matrix matrix;
    QVector<double> roots;
    QVector<QLabel *> lbl;
    QVector<double> U0, UL;

    QVector<double> v_x;
    QVector<double> v_Nx;
    QVector<double> v_qx;
    QVector<double> v_ux;
    QTableWidget *rascheti;
    int size_wid_frame;
    int size_wid_frame_first;
private:
// для процессора
    bool perexodim();
    void of_processor();
    bool processor_color(QTableWidget *table, bool *res);
    void filling_the_matrix();
    void   draw_table();
    double func_u(double x, int p);
    double func_N(double x, int p);
    double func_q(double x, int p);
    double my_find_q(int p);
    void ferger(QTextStream &stream, QTableWidget *table, bool in_excel = false);
// для препроцессора
    void num_tables();
    void num_table(QTableWidget *table);
    void update_button(QTableWidget *table = NULL);
    void resize_hor_table(QTableWidget* table);
    QString table_text(QTableWidget* table);
    void init_item(QTableWidget *table, int row, int col);
    void info_error(const int& error);
// обработка чисел double
    QString my_d(QString str, bool delete_minus = false);
    QString md(double str, QString a_nado = "yes");
    double md(double number, int precition);
// краткое заполнение ячейки
    QTableWidget* for_za;
    int for_za_i;
    int for_za_j;
    int for_za_col;
    void new_table_za(QTableWidget*, int new_col);
    void za(double value);
    void za(QString value);
public slots:
    void resize_hor_tables();
    void table_change(QTableWidgetItem* item);
    void select_table(QTableWidgetItem* item);
    void my_timer();
    void my_timer_button();
    void my_timer_load();
    void otrisovka(QTableWidgetItem *tmp_item = NULL);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void lineEdit_2_textChanged(const QString arg1);
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void set_interval(int index);
    void new_value();
    void act_open();
    void on_actionSave_triggered();
    void on_action_2_triggered();
};

#endif // MAINWINDOW_H
