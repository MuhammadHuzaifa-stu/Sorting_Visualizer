#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QString>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void insertionSort(std::vector<int>& array);
    void selectionSort(std::vector<int>& array);
    void bubbleSort(std::vector<int>& array);


private slots:
    void on_pushButton_clicked();    // Start button
    void on_pushButton_2_clicked();  // Pause button
    void on_pushButton_3_clicked();  // Reset button
    void updateVisualization();      // Update the visualization step

private:
    Ui::MainWindow *ui;
    std::vector<int> array;         // The array to be sorted
    std::vector<std::vector<int>> steps; // Steps of sorting

    bool isPaused;                  // Flag for pausing

    int currentStep;                 // Current step in visualization
    int currentAlgorithm;           // Current sorting algorithm

    QGraphicsScene *scene;          // Graphics scene for visualization
    std::vector<QGraphicsRectItem*> rectItems; // Rect items to represent array elements

    QTimer *timer;                   // Timer for visualization

    void visualizeArray(const std::vector<int>& array);
};
#endif // MAINWINDOW_H
