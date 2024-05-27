#include "window.h"
#include "menu.h"
#include "./ui_window.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);
    new Menu(this);
    addScrollArea();
}

Window::~Window()
{
    delete ui;
}

void Window::addScrollArea() {
    widget = new QWidget();
    scrollArea = new QScrollArea();
    vboxlayuot = new QVBoxLayout();
    textEdit = new QTextEdit();

    vboxlayuot->addWidget(textEdit);
    widget->setLayout(vboxlayuot);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setWidget(widget);
    setCentralWidget(scrollArea);
}

void Window::DrawText(int x, int y, std::string text){
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    textEdit->cursorForPosition(QPoint(x, y));
    cursor.insertText(QString::fromStdString(text));
    textEdit->cursorForPosition(QPoint(x, y));
    //cursor.setPosition(500);

    cursor.insertText(QString::fromStdString("ghgh"));
}

/*void Window::DrawLine(int x1, int y1, int x2, int y2){

}*/

void Window::ClearGlyph(int x, int y, int width, int height){

}
