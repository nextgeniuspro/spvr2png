#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QStringListModel>
extern "C"
{
#include "SegaPVRImage.h"
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen_PVR, SIGNAL(triggered()), this, SLOT(OnOpenFile()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->convertButton, SIGNAL(clicked()), this, SLOT(OnConvertButtonClicked()));

    BuildTwiddleTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnOpenFile()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Sega PVR images (*.pvr *.PVR)"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        QStringListModel *model = new QStringListModel();
        model->setStringList(fileNames);
        ui->listView->setModel(model);
    }

    ui->statusBar->showMessage("");
}

void MainWindow::OnConvertButtonClicked()
{
    QAbstractItemModel *model = ui->listView->model();
    if (!model || model->rowCount() == 0)
    {
        return;
    }

    while (model->rowCount())
    {
        QString pvrName = model->data(model->index(0, 0)).toString();
        QString pngName = pvrName;
        pngName.replace(".pvr", ".png", Qt::CaseInsensitive);

        unsigned char* data;
        unsigned long int size;
        PVRTHeader header;
        int ret = LoadPVRFromFile(pvrName.toUtf8().data(), &data, &size, &header);
        if (ret)
        {
            QFile file(pngName);
            file.open(QIODevice::WriteOnly);

            QImage image(data, header.width, header.height, QImage::Format_RGBA8888);
            QPixmap pixmap = QPixmap::fromImage(image);
            pixmap.save(&file, "PNG");

            free(data);
        }

        model->removeRow(0);
    }

    ui->statusBar->showMessage("Done!");
}
