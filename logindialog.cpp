#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->txtPassword->setEchoMode(QLineEdit::Password);

    connect(ui->btnIngresar, &QPushButton::clicked, this, &LoginDialog::on_btnIngresar_clicked);
    connect(ui->btnRegistrar, &QPushButton::clicked, this, &LoginDialog::on_btnRegistrar_clicked);
    connect(ui->btnConfirmar, &QPushButton::clicked, this, &LoginDialog::on_btnConfirmar_clicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &LoginDialog::on_btnCancelar_clicked);

    cargarDesdeArchivo();  // ← NOMBRE CORRECTO
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnIngresar_clicked()
{
    esRegistro = false;
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnConfirmar->setText("Iniciar Sesión");

    ui->txtUsuario->clear();
    ui->txtPassword->clear();
    ui->txtUsuario->setFocus();
}

void LoginDialog::on_btnRegistrar_clicked()
{
    esRegistro = true;
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnConfirmar->setText("Registrarse");

    ui->txtUsuario->clear();
    ui->txtPassword->clear();
    ui->txtUsuario->setFocus();
}

void LoginDialog::on_btnConfirmar_clicked()
{
    QString usuario = ui->txtUsuario->text().trimmed();
    QString clave   = ui->txtPassword->text();

    if (usuario.isEmpty() || clave.isEmpty()) {
        QMessageBox::warning(this, "Atención", "Usuario y contraseña son obligatorios.");
        return;
    }

    if (esRegistro) {
        // Registro nuevo usuario
        for (const auto &u : usuarios) {
            if (u.nombre == usuario) {
                QMessageBox::warning(this, "Atención", "Este usuario ya existe.");
                ui->txtUsuario->setFocus();
                return;
            }
        }

        Usuario nuevo;
        nuevo.nombre = usuario;
        nuevo.clave  = clave;
        usuarios.push_back(nuevo);

        guardarEnArchivo();  // ← NOMBRE CORRECTO

        QMessageBox::information(this, "Éxito", "Usuario registrado correctamente.");
        ui->stackedWidget->setCurrentIndex(0);
        ui->txtUsuario->clear();
        ui->txtPassword->clear();
    }
    else {
        // Login
        bool encontrado = false;
        for (const auto &u : usuarios) {
            if (u.nombre == usuario && u.clave == clave) {
                encontrado = true;
                break;
            }
        }

        if (encontrado) {
            accept();
        } else {
            QMessageBox::critical(this, "Error", "Usuario o contraseña incorrectos.");
            ui->txtUsuario->clear();
            ui->txtPassword->clear();
        }
    }
}

void LoginDialog::on_btnCancelar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->txtUsuario->clear();
    ui->txtPassword->clear();
}

void LoginDialog::cargarDesdeArchivo()  // ← NOMBRE EXACTO
{
    QFile file("usuarios.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;  // Archivo no existe = no hay usuarios
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList partes = linea.split("|");
        if (partes.size() == 2) {
            Usuario u;
            u.nombre = partes[0];
            u.clave = partes[1];
            usuarios.push_back(u);
        }
    }
    file.close();
}

void LoginDialog::guardarEnArchivo()  // ← NOMBRE EXACTO
{
    QFile file("usuarios.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo guardar usuarios.txt");
        return;
    }

    QTextStream out(&file);
    for (const auto &u : usuarios) {
        out << u.nombre << "|" << u.clave << "\n";
    }
    file.close();
}
