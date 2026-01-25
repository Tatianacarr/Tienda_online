#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tblProductos->setColumnCount(8);
    ui->tblProductos->setHorizontalHeaderLabels({
        "ID","Producto","Categoría","Precio","Stock","Subtotal","IVA 15%","Total"
    });

    ui->tblProductos->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    cargarArchivo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGuardar_clicked()
{
    int fila = ui->tblProductos->rowCount();
    ui->tblProductos->insertRow(fila);

    int id = ui->txtId->text().toInt();
    QString prod = ui->txtProducto->text();
    QString cat = ui->cmbCategoria->currentText();
    double precio = ui->txtPrecio->text().toDouble();
    int stock = ui->spStock->value();


    double subtotal = precio * stock;
    double iva = subtotal * 0.15;
    double total = subtotal + iva;

    ui->tblProductos->setItem(fila,0,new QTableWidgetItem(QString::number(id)));
    ui->tblProductos->setItem(fila,1,new QTableWidgetItem(prod));
    ui->tblProductos->setItem(fila, 2, new QTableWidgetItem(cat));

        ui->tblProductos->setItem(fila, 0, new QTableWidgetItem(QString::number(id)));
        ui->tblProductos->setItem(fila, 1, new QTableWidgetItem(prod));
        ui->tblProductos->setItem(fila, 2, new QTableWidgetItem(cat));
        ui->tblProductos->setItem(fila, 3, new QTableWidgetItem(QString::number(precio,'f',2)));
        ui->tblProductos->setItem(fila, 4, new QTableWidgetItem(QString::number(stock)));
        ui->tblProductos->setItem(fila, 5, new QTableWidgetItem(QString::number(subtotal,'f',2)));
        ui->tblProductos->setItem(fila, 6, new QTableWidgetItem(QString::number(iva,'f',2)));
        ui->tblProductos->setItem(fila, 7, new QTableWidgetItem(QString::number(total,'f',2)));

        recalcularFila(fila);
        guardarArchivo();
    }


void MainWindow::on_btnBuscar_clicked()
{
    int id = ui->txtId->text().toInt();
    for(int i=0;i<ui->tblProductos->rowCount();i++){
        if(ui->tblProductos->item(i,0)->text().toInt()==id){
            ui->txtProducto->setText(ui->tblProductos->item(i,1)->text());
            ui->cmbCategoria->setCurrentText(ui->tblProductos->item(i,2)->text());
            ui->txtPrecio->setText(
                ui->tblProductos->item(i, 3)->text()
                );

            ui->spStock->setValue(ui->tblProductos->item(i,4)->text().toInt());
            return;
        }
    }
}

void MainWindow::on_btnActualizar_clicked()
{
    int id = ui->txtId->text().toInt();
    for(int i=0;i<ui->tblProductos->rowCount();i++){
        if(ui->tblProductos->item(i,0)->text().toInt()==id){

            double precio = ui->txtPrecio->text().toDouble();
            int stock = ui->spStock->value();


            ui->tblProductos->item(i,1)->setText(ui->txtProducto->text());
            ui->tblProductos->item(i,2)->setText(ui->cmbCategoria->currentText());
            ui->tblProductos->item(i,3)->setText(QString::number(precio,'f',2));
            ui->tblProductos->item(i,4)->setText(QString::number(stock));

            recalcularFila(i);

            guardarArchivo();
            return;
        }
    }
}

void MainWindow::on_btnEliminar_clicked()
{
    int id = ui->txtId->text().toInt();
    for(int i=0;i<ui->tblProductos->rowCount();i++){
        if(ui->tblProductos->item(i,0)->text().toInt()==id){
            ui->tblProductos->removeRow(i);
            guardarArchivo();
            return;
        }
    }
}

void MainWindow::cargarArchivo()
{
    QFile file("Productos.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        QString linea = in.readLine();
        QStringList partes = linea.split("|");
        if(partes.size() == 5){
            int fila = ui->tblProductos->rowCount();
            ui->tblProductos->insertRow(fila);

            ui->tblProductos->setItem(fila, 0, new QTableWidgetItem(partes[0]));
            ui->tblProductos->setItem(fila, 1, new QTableWidgetItem(partes[1]));
            ui->tblProductos->setItem(fila, 2, new QTableWidgetItem(partes[2]));
            ui->tblProductos->setItem(fila, 3, new QTableWidgetItem(partes[3]));
            ui->tblProductos->setItem(fila, 4, new QTableWidgetItem(partes[4]));

            recalcularFila(fila);
        }
    }
    file.close();
}

void MainWindow::guardarArchivo()
{
    QFile file("Productos.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo de productos.");
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < ui->tblProductos->rowCount(); ++i) {
        QString linea = ui->tblProductos->item(i, 0)->text() + "|" +
                        ui->tblProductos->item(i, 1)->text() + "|" +
                        ui->tblProductos->item(i, 2)->text() + "|" +
                        ui->tblProductos->item(i, 3)->text() + "|" +
                        ui->tblProductos->item(i, 4)->text();
        out << linea << "\n";
    }
    file.close();
}

void MainWindow::recalcularFila(int fila)
{
    double precio = ui->tblProductos->item(fila, 3)->text().toDouble();
    int stock = ui->tblProductos->item(fila, 4)->text().toInt();
    double subtotal = precio * stock;
    double iva = subtotal * 0.15;
    double total = subtotal + iva;

    ui->tblProductos->setItem(fila, 5, new QTableWidgetItem(QString::number(subtotal, 'f', 2)));
    ui->tblProductos->setItem(fila, 6, new QTableWidgetItem(QString::number(iva, 'f', 2)));
    ui->tblProductos->setItem(fila, 7, new QTableWidgetItem(QString::number(total, 'f', 2)));
}
