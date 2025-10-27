#pragma once

#include <QVector>

#include <QtDataVisualization/Q3DBars>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QBar3DSeries>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtWidgets>

#include <QWidget>

#include <cmath>
#include <vector>

namespace Widgets {
using namespace QtDataVisualization;

class Matrix3DView : public QWidget
{
    Q_OBJECT

public:
    void clear()
    {
        if (!surface_->seriesList().isEmpty())
            surface_->removeSeries(surface_->seriesList().first());
    }
    //! Перезагружает отображение.
    void setMatrix(const std::vector<std::vector<double>>& matrix)
    {
        if (matrix.empty())
        {
            qDebug() << "Матрица говна";
            return;
        }

        clear();

        QSurfaceDataArray* dataArray = new QSurfaceDataArray;
        dataArray->reserve(matrix.size());
        for (int y = 0; y < (int)matrix.size(); ++y)
        {
            QSurfaceDataRow* row = new QSurfaceDataRow(matrix[y].size());
            for (int x = 0; x < (int)matrix[y].size(); ++x)
                (*row)[x].setPosition(QVector3D(x, matrix[y][x], y));
            *dataArray << row;
        }
        QSurfaceDataProxy* proxy = new QSurfaceDataProxy();
        proxy->resetArray(dataArray);
        QSurface3DSeries* series = new QSurface3DSeries(proxy);
        series->setDrawMode(QSurface3DSeries::DrawSurface);
        series->setFlatShadingEnabled(false);

        QLinearGradient grad;
        grad.setColorAt(0.00, QColor("#313695"));
        grad.setColorAt(0.10, QColor("#4575b4"));
        grad.setColorAt(0.25, QColor("#74add1"));
        grad.setColorAt(0.40, QColor("#abd9e9"));
        grad.setColorAt(0.50, QColor("#e0f3f8"));
        grad.setColorAt(0.60, QColor("#fee090"));
        grad.setColorAt(0.75, QColor("#fdae61"));
        grad.setColorAt(0.90, QColor("#f46d43"));
        grad.setColorAt(1.00, QColor("#a50026"));
        series->setBaseGradient(grad);
        series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
        surface_->addSeries(series);

        update();
    }

    Matrix3DView(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QWidget* stackContainer = new QWidget(this);

        stacked_ = new QStackedLayout(stackContainer);

        //! Надписи

        label = new QLabel("<T> = 0, <S> = <0, 0>");
        label->setAlignment(Qt::AlignLeft);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        // --- Surface ---
        surface_ = new Q3DSurface();
        QWidget* surfContainer = QWidget::createWindowContainer(surface_);
        stacked_->addWidget(surfContainer);

        auto* layout = new QVBoxLayout(this);

        layout->addWidget(label);
        layout->addWidget(stackContainer);
    }

public slots:
    void showSurface()
    {
        stacked_->setCurrentIndex(0);
    }

    void setAverageTime(double value)
    {
        averageTime_ = value;
        updateLabel();
    }

    void setTimeStatistics(std::pair<double, double>& value)
    {
        timeStatistics_ = value;
        updateLabel();
    }

    void updateLabel()
    {
        label->setText(QString("<T> = %1, <S> = <%2, %3>").arg(averageTime_).arg(timeStatistics_.first).arg(timeStatistics_.second));
    }

    void setWait()
    {
        label->setText("-.-");
    }

private:
    QStackedLayout* stacked_;
    Q3DSurface* surface_;

    QLabel* label;

    double averageTime_{0};
    std::pair<double, double> timeStatistics_{0, 0};
};
} // namespace Widgets
