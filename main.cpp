#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <Eigen/Dense>
#include <QInputDialog>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Kalkulator macierzy kwadratowych");

    QGridLayout* layout = new QGridLayout(&window);

    QLabel* labelMatrix1 = new QLabel("Macierz A:");
    QLabel* labelMatrix2 = new QLabel("Macierz B:");
    QTableWidget* tableMatrix1 = new QTableWidget;
    QTableWidget* tableMatrix2 = new QTableWidget;
    QPushButton* buttonCalculate = new QPushButton("Oblicz");
    QLabel* labelResult = new QLabel("Wynik:");


    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - window.width()) / 2;
    int y = (screenGeometry.height() - window.height()) / 2;
    window.move(x, y);

    QInputDialog inputDialog;
    inputDialog.setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    inputDialog.size(),
                    QApplication::desktop()->availableGeometry()
            )
    );

    int matrixSize = QInputDialog::getInt(&window, "Podaj rozmiar macierzy kwadratowej", "Rozmiar macierzy kwadratowej:");

    tableMatrix1->setRowCount(matrixSize);
    tableMatrix1->setColumnCount(matrixSize);
    tableMatrix2->setRowCount(matrixSize);
    tableMatrix2->setColumnCount(matrixSize);

    layout->addWidget(labelMatrix1, 0, 0);
    layout->addWidget(tableMatrix1, 0, 1);
    layout->addWidget(labelMatrix2, 1, 0);
    layout->addWidget(tableMatrix2, 1, 1);
    layout->addWidget(buttonCalculate, 2, 0, 1, 2);
    layout->addWidget(labelResult, 3, 0, 1, 2);

    window.resize(matrixSize * 100 + 150, matrixSize * 120 + 85);

    QObject::connect(buttonCalculate, &QPushButton::clicked, [&]() {
        Eigen::MatrixXd matrix1(matrixSize, matrixSize);
        Eigen::MatrixXd matrix2(matrixSize, matrixSize);

        for (int row = 0; row < matrixSize; row++) {
            for (int col = 0; col < matrixSize;col++) {
                QTableWidgetItem* itemMatrix1 = tableMatrix1->item(row, col);
                QTableWidgetItem* itemMatrix2 = tableMatrix2->item(row, col);

                if (itemMatrix1 && itemMatrix2) {
                    matrix1(row, col) = itemMatrix1->text().toDouble();
                    matrix2(row, col) = itemMatrix2->text().toDouble();
                } else {
                    std::cout<<"Error"<<std::endl;
                    exit(-1);
                }
            }
        }

        QInputDialog inputDialog;
        inputDialog.setComboBoxEditable(false);
        inputDialog.setWindowTitle("Wybierz operację");
        inputDialog.setLabelText("Operacja:");
        inputDialog.setComboBoxItems(QStringList() << "Dodawanie" << "Odejmowanie A-B" << "Mnożenie A*B"<<"Mnożenie B*A"<<"Wyznacznik A"<<"Wyznacznik B"<<"Transpozycja macierzy A"<<"Transpozycja macierzy B"<<"Macierz odwrotna do A"<<"Macierz odwrotna do B");
        inputDialog.exec();

        QString operation = inputDialog.textValue();

        Eigen::MatrixXd result;
        double result_int;
        int pom =1;

        if (operation == "Dodawanie") {
            result = matrix1 + matrix2;
        } else if (operation == "Odejmowanie A-B") {
            result = matrix1 - matrix2;
        } else if (operation == "Mnożenie A*B") {
            result = matrix1 * matrix2;
        } else if (operation == "Mnożenie B*A") {
            result = matrix2 * matrix1;
        } else if (operation == "Wyznacznik A") {
            result_int = matrix1.determinant();
            pom = 0;
            if(result_int==-0) result_int=0;
        } else if (operation == "Wyznacznik B") {
            result_int = matrix2.determinant();
            pom = 0;
            if(result_int==-0) result_int=0;
        } else if (operation == "Transpozycja macierzy A") {
            result = matrix1.transpose();
        } else if (operation == "Transpozycja macierzy B") {
            result = matrix2.transpose();
        } else if (operation == "Macierz odwrotna do A") {
            if (matrix1.determinant() != 0) {
                result = matrix1.inverse();
            } else {
                pom = -1;
            }
        } else if (operation == "Macierz odwrotna do B") {
            if (matrix2.determinant() != 0) {
                result = matrix2.inverse();
            } else {
                pom = -1;
            }
        }

        std::ostringstream resultStream;
        if (pom == 1) {
            if (result.size() > 0) {
                int rows = result.rows();
                int cols = result.cols();
                QTableWidget* tableResult = new QTableWidget(rows, cols);
                tableResult->setEditTriggers(QAbstractItemView::NoEditTriggers);
                for (int row = 0; row < rows; row++) {
                    for (int col = 0; col < cols; col++) {
                        QTableWidgetItem* item = new QTableWidgetItem(QString::number(result(row, col)));
                        tableResult->setItem(row, col, item);
                    }
                }
                QLayoutItem* previousResult = layout->itemAtPosition(4, 0);
                if (previousResult) {
                    QWidget* previousWidget = previousResult->widget();
                    layout->removeWidget(previousWidget);
                    delete previousWidget;
                }

                layout->addWidget(tableResult, 4, 0, 1, 2);
            }
            resultStream << "Wynik:";
            labelResult->setText(QString::fromStdString(resultStream.str()));
        } else if (pom == 0) {
            QLayoutItem* previousResult = layout->itemAtPosition(4, 0);
            if (previousResult) {
                QWidget* previousWidget = previousResult->widget();
                layout->removeWidget(previousWidget);
                delete previousWidget;
            }

            resultStream << "Wynik:\n\n" << result_int;
            labelResult->setText(QString::fromStdString(resultStream.str()));
        } else if (pom == -1) {
            QLayoutItem* previousResult = layout->itemAtPosition(4, 0);
            if (previousResult) {
                QWidget* previousWidget = previousResult->widget();
                layout->removeWidget(previousWidget);
                delete previousWidget;
            }

            resultStream << "Nie istnieje macierz odwrotna - wyznacznik równy 0\n";
            labelResult->setText(QString::fromStdString(resultStream.str()));
        }
    });

    window.show();
    return app.exec();
}