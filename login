Avance del login 
#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->txtPassword->setEchoMode(QLineEdit::Password);

    // Conexión manual (evita errores de Go to Slot)
    connect(ui->btnIngresar, &QPushButton::clicked,
            this, &LoginDialog::on_btnIngresar_clicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnIngresar_clicked()
{
    QString usuario = ui->txtUsuario->text();
    QString clave   = ui->txtPassword->text();

    if (usuario == "angui" && clave == "1234") {
        accept();   // Login correcto
    } else {
        QMessageBox::critical(
            this,
            "Error",
            "Usuario o contraseña incorrectos"
            );
    }
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnGuardar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
