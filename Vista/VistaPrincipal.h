#pragma once

#include "IVistaArchivo.h"
#include "../Controlador/ControladorArchivo.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

public ref class VistaPrincipal : public Form, public IVistaArchivo
{
private:
    System::ComponentModel::Container^ componentes;
    ControladorArchivo^ controlador;
    TextBox^ txtRuta;
    RichTextBox^ txtContenido;
    Label^ lblEstado;
    Button^ btnExaminar;
    Button^ btnLeer;
    Button^ btnGuardar;

    void InicializarComponentes()
    {
        componentes = gcnew System::ComponentModel::Container();

        this->Text = "Hoja de vida del doctor - Manejo de archivos";
        this->StartPosition = FormStartPosition::CenterScreen;
        this->MinimumSize = Drawing::Size(860, 590);
        this->ClientSize = Drawing::Size(980, 680);
        this->BackColor = Color::FromArgb(244, 247, 250);
        this->Font = gcnew Drawing::Font("Segoe UI", 10.0F);

        Panel^ encabezado = gcnew Panel();
        encabezado->Dock = DockStyle::Top;
        encabezado->Height = 94;
        encabezado->BackColor = Color::FromArgb(20, 53, 87);

        Label^ titulo = gcnew Label();
        titulo->AutoSize = true;
        titulo->Location = Point(28, 18);
        titulo->ForeColor = Color::White;
        titulo->Font = gcnew Drawing::Font(
            "Segoe UI Semibold",
            20.0F,
            FontStyle::Bold);
        titulo->Text = "Hoja de vida del doctor";

        Label^ subtitulo = gcnew Label();
        subtitulo->AutoSize = true;
        subtitulo->Location = Point(31, 59);
        subtitulo->ForeColor = Color::FromArgb(210, 225, 239);
        subtitulo->Text =
            "Lectura y escritura de un archivo .txt con arquitectura MVC";

        encabezado->Controls->Add(titulo);
        encabezado->Controls->Add(subtitulo);
        this->Controls->Add(encabezado);

        Label^ lblRuta = gcnew Label();
        lblRuta->AutoSize = true;
        lblRuta->Location = Point(28, 116);
        lblRuta->Font = gcnew Drawing::Font(
            "Segoe UI Semibold",
            10.0F,
            FontStyle::Bold);
        lblRuta->Text = "Archivo .txt existente";
        this->Controls->Add(lblRuta);

        txtRuta = gcnew TextBox();
        txtRuta->Location = Point(31, 144);
        txtRuta->Size = Drawing::Size(685, 30);
        txtRuta->Anchor =
            AnchorStyles::Top | AnchorStyles::Left | AnchorStyles::Right;
        this->Controls->Add(txtRuta);

        btnExaminar = gcnew Button();
        btnExaminar->Location = Point(731, 141);
        btnExaminar->Size = Drawing::Size(105, 35);
        btnExaminar->Anchor = AnchorStyles::Top | AnchorStyles::Right;
        btnExaminar->Text = "Examinar";
        btnExaminar->UseVisualStyleBackColor = true;
        btnExaminar->Click += gcnew EventHandler(
            this,
            &VistaPrincipal::AlExaminar);
        this->Controls->Add(btnExaminar);

        btnLeer = gcnew Button();
        btnLeer->Location = Point(846, 141);
        btnLeer->Size = Drawing::Size(105, 35);
        btnLeer->Anchor = AnchorStyles::Top | AnchorStyles::Right;
        btnLeer->Text = "Leer";
        btnLeer->BackColor = Color::FromArgb(233, 177, 35);
        btnLeer->FlatStyle = FlatStyle::Flat;
        btnLeer->FlatAppearance->BorderSize = 0;
        btnLeer->Click += gcnew EventHandler(
            this,
            &VistaPrincipal::AlLeer);
        this->Controls->Add(btnLeer);

        Label^ lblEditor = gcnew Label();
        lblEditor->AutoSize = true;
        lblEditor->Location = Point(28, 196);
        lblEditor->Font = gcnew Drawing::Font(
            "Segoe UI Semibold",
            10.0F,
            FontStyle::Bold);
        lblEditor->Text = "Contenido";
        this->Controls->Add(lblEditor);

        txtContenido = gcnew RichTextBox();
        txtContenido->Location = Point(31, 224);
        txtContenido->Size = Drawing::Size(920, 365);
        txtContenido->Anchor =
            AnchorStyles::Top | AnchorStyles::Bottom |
            AnchorStyles::Left | AnchorStyles::Right;
        txtContenido->Font = gcnew Drawing::Font("Consolas", 10.5F);
        txtContenido->BackColor = Color::White;
        txtContenido->BorderStyle = BorderStyle::FixedSingle;
        txtContenido->AcceptsTab = true;
        txtContenido->DetectUrls = false;
        this->Controls->Add(txtContenido);

        lblEstado = gcnew Label();
        lblEstado->AutoEllipsis = true;
        lblEstado->Location = Point(31, 611);
        lblEstado->Size = Drawing::Size(760, 26);
        lblEstado->Anchor =
            AnchorStyles::Bottom | AnchorStyles::Left | AnchorStyles::Right;
        lblEstado->ForeColor = Color::FromArgb(70, 82, 94);
        lblEstado->Text =
            "Seleccione un .txt. La aplicación no crea el archivo inicial.";
        this->Controls->Add(lblEstado);

        btnGuardar = gcnew Button();
        btnGuardar->Location = Point(806, 604);
        btnGuardar->Size = Drawing::Size(145, 40);
        btnGuardar->Anchor =
            AnchorStyles::Bottom | AnchorStyles::Right;
        btnGuardar->Text = "Guardar cambios";
        btnGuardar->ForeColor = Color::White;
        btnGuardar->BackColor = Color::FromArgb(20, 107, 86);
        btnGuardar->FlatStyle = FlatStyle::Flat;
        btnGuardar->FlatAppearance->BorderSize = 0;
        btnGuardar->Click += gcnew EventHandler(
            this,
            &VistaPrincipal::AlGuardar);
        this->Controls->Add(btnGuardar);
    }

    void AlExaminar(Object^, EventArgs^)
    {
        OpenFileDialog^ dialogo = gcnew OpenFileDialog();
        dialogo->Title = "Seleccionar hoja de vida del doctor";
        dialogo->Filter = "Archivos de texto (*.txt)|*.txt";
        dialogo->CheckFileExists = true;
        dialogo->CheckPathExists = true;
        dialogo->Multiselect = false;

        if (dialogo->ShowDialog(this) ==
            System::Windows::Forms::DialogResult::OK)
        {
            txtRuta->Text = dialogo->FileName;
            controlador->Leer();
        }
    }

    void AlLeer(Object^, EventArgs^)
    {
        controlador->Leer();
    }

    void AlGuardar(Object^, EventArgs^)
    {
        if (MessageBox::Show(
                this,
                "Se sobrescribirá el archivo seleccionado con el contenido "
                "del editor. ¿Desea continuar?",
                "Confirmar escritura",
                MessageBoxButtons::YesNo,
                MessageBoxIcon::Question) ==
            System::Windows::Forms::DialogResult::Yes)
        {
            controlador->Guardar();
        }
    }

public:
    VistaPrincipal()
    {
        InicializarComponentes();
        controlador = gcnew ControladorArchivo(this);
    }

    ~VistaPrincipal()
    {
        delete controlador;
        if (componentes != nullptr)
        {
            delete componentes;
        }
    }

    virtual property String^ RutaArchivo
    {
        String^ get()
        {
            return txtRuta->Text->Trim();
        }
    }

    virtual property String^ Contenido
    {
        String^ get()
        {
            return txtContenido->Text;
        }

        void set(String^ valor)
        {
            txtContenido->Text = valor;
        }
    }

    virtual void MostrarInformacion(String^ mensaje)
    {
        lblEstado->ForeColor = Color::FromArgb(20, 107, 86);
        lblEstado->Text = mensaje;
    }

    virtual void MostrarError(String^ titulo, String^ mensaje)
    {
        lblEstado->ForeColor = Color::FromArgb(177, 45, 45);
        lblEstado->Text = mensaje;
        MessageBox::Show(
            this,
            mensaje,
            titulo,
            MessageBoxButtons::OK,
            MessageBoxIcon::Error);
    }
};
