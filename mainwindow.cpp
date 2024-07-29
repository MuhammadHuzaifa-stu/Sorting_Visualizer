#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isPaused(false)
    , currentAlgorithm(0)
{
    ui->setupUi(this);
    // Initialize the QGraphicsScene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    // Apply stylesheet to the QGraphicsView
    QString styleSheet   = "QGraphicsView {"
                           "    background-color: #282c34;"
                           "    border: 2px solid #61afef;"
                           "    border-radius: 10px;"
                           "}"
                           "QGraphicsTextItem {"
                           "    color: #e06c75;"
                           "    font-family: Arial, sans-serif;"
                           "    font-size: 14px;"
                           "}";

    QString styleSheet_2 = "QLineEdit{ "
                           "    background-color: #282c34;"
                           "    border-radius: 5px; "
                           "    font-size: 13px;"
                           "}";

    QString styleSheet_3 = "QComboBox{ "
                           "    border-radius: 5px; "
                           "    font-size: 13px;"
                           "    padding: 5px 5px;"
                           "}";

    ui->graphicsView->setStyleSheet(styleSheet);
    ui->lineEdit->setStyleSheet(styleSheet_2);
    ui->lineEdit->setPlaceholderText("Enter the array elements coma separated.");
    ui->comboBox->setStyleSheet(styleSheet_3);

    // Initialize the QTimer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateVisualization);

    // Connect comboBox selection change to update currentAlgorithm
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){
        currentAlgorithm = index;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::insertionSort(std::vector<int>& array)
{
    int n = array.size();
    steps.clear();
    steps.push_back(array); // Store initial state

    for (int i = 1; i < n; i++) {
        int curr = array[i];
        int j    = i - 1;

        while (j >= 0 && array[j] > curr) {
            array[j + 1] = array[j];
            j--;
            steps.push_back(array);
        }
        array[j + 1] = curr;
        steps.push_back(array);
    }
}

void MainWindow::selectionSort(std::vector<int>& array)
{
    int n = array.size();
    steps.clear();
    steps.push_back(array); // Store initial state

    for (int i = 0; i < n-1; i++){
        for (int j = i+1; j < n; j++){
            if (array[j] <= array[i]){
                // int tmp  = array[i];
                // array[i] = array[j];
                // array[j] = tmp;
                std::swap(array[i],array[j]);
            }
            steps.push_back(array);
        }
        steps.push_back(array);
    }
}

void MainWindow::bubbleSort(std::vector<int>& array)
{
    int n       = array.size();
    int counter = 1;
    steps.clear();
    steps.push_back(array); // Store initial state

    while (counter < n){
        for (int i = 0; i < n - counter; i++){
            if (array[i] > array[i + 1]){
                // int tmp    = array[i];
                // array[i]   = array[i+1];
                // array[i+1] = tmp;
                std::swap(array[i],array[i+1]);
            }
            steps.push_back(array);
        }
        counter = counter + 1;
        steps.push_back(array);
    }
}

void MainWindow::on_pushButton_clicked() // Start button
{
    // Get input from lineEdit
    QString     input     = ui->lineEdit->text();
    QStringList inputList = input.split(',');
    array.clear();
    for (const QString& number : inputList) {
        array.push_back(number.toInt());
    }

    // Start the selected sorting algorithm
    switch (currentAlgorithm) {
    case 0:
        insertionSort(array);
        break;
    case 1:
        selectionSort(array);
        break;
    case 2:
        bubbleSort(array);
        break;
    default:
        QMessageBox::warning(this, "Error", "Invalid sorting algorithm selected.");
        break;
    }

    // Initialize visualization
    currentStep = 0;
    timer->start(500); // Update every 500 ms
}

void MainWindow::on_pushButton_2_clicked() // Pause button
{
    // isPaused = !isPaused;
    if (timer->isActive()) {
        timer->stop();
    } else {
        timer->start(500);
    }
    // Handle pausing the animation (to be implemented)
}

void MainWindow::on_pushButton_3_clicked() // Reset button
{
    // array.clear();
    // ui->lineEdit->clear();

    // scene->clear();
    // rectItems.clear();
    array.clear();
    steps.clear();
    currentStep = 0;
    ui->lineEdit->clear();
    scene->clear();
    rectItems.clear();
    timer->stop();
}

void MainWindow::visualizeArray(const std::vector<int>& array)
{
    // Clear the previous items
    scene->clear();
    rectItems.clear();

    // Calculate the dimensions for each rectangle
    int width       = ui->graphicsView->width() / array.size();
    int heightScale = 5; // Scale factor for height

    for (int i = 0; i < array.size(); i++) {
        int height = array[i] * heightScale;
        QGraphicsRectItem* rect = scene->addRect(i * width, 0, width - 2, height);

        rectItems.push_back(rect);
        // Set the fill color based on the value
        QColor color;
        if (array[i] % 2 == 0) {
            color.setRgb(152, 195, 121); // Green for even numbers
        } else {
            color.setRgb(224, 108, 117); // Red for odd numbers
        }
        rect->setBrush(QBrush(color));

        rectItems.push_back(rect);
        // Add the number label
        QGraphicsTextItem* text = scene->addText(QString::number(array[i]));
        text->setPos(i * width + width / 2 - text->boundingRect().width() / 2, height + 5);
    }
}

void MainWindow::updateVisualization()
{
    if (currentStep < steps.size()) {
        visualizeArray(steps[currentStep]);
        currentStep++;
    } else {
        timer->stop();
    }
}
