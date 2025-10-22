#pragma once

#include "../../Models/Request.hpp"
#include <QInputDialog>
#include <QStandardItemModel>
#include <QTreeView>
#include <QWidget>

namespace Widgets::Data {
class RequestPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit RequestPropertyEditor(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);
        treeView = new QTreeView(this);
        model = new QStandardItemModel(this);
        treeView->setModel(model);
        treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
        treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        treeView->setSelectionMode(QAbstractItemView::SingleSelection);
        treeView->setContextMenuPolicy(Qt::CustomContextMenu);
        layout->addWidget(treeView);
        setupModel();

        connect(model, &QStandardItemModel::dataChanged, this, &RequestPropertyEditor::updateRequestFromModel);
        connect(treeView, &QTreeView::customContextMenuRequested, this, &RequestPropertyEditor::showContextMenu);
    }

public slots:
    void updateFromRequest(const Models::Request& request)
    {
        updateModelFromRequest(request);
    }

signals:
    void propertyChanged(const Models::Request& request);

private:
    void setupModel()
    {
        model->clear();
        model->setHorizontalHeaderLabels({"Property", "Value"});

        // Request properties
        QList<QStandardItem*> timeItems;
        timeItems << new QStandardItem("Time") << new QStandardItem;
        timeItems[0]->setFlags(timeItems[0]->flags() & ~Qt::ItemIsEditable);
        model->appendRow(timeItems);

        // Target properties
        setupTargetModel();

        // Ship properties
        setupShipModel();

        // Perimeter properties
        setupPerimeterModel();
    }

    void setupPerimeterModel()
    {
        QStandardItem* perimeterRoot = new QStandardItem("Search Region");
        perimeterRoot->setFlags(perimeterRoot->flags() & ~Qt::ItemIsEditable);
        model->appendRow(perimeterRoot);
        // Perimeter - Points
        createPointEditor(perimeterRoot, "Entrance", QPointF());
        createPointEditor(perimeterRoot, "Exit", QPointF());
        // Perimeter - Borders
        QStandardItem* bordersRoot = new QStandardItem("Rings");
        bordersRoot->setFlags(bordersRoot->flags() & ~Qt::ItemIsEditable);
        perimeterRoot->appendRow(bordersRoot);

        // Border Line
        QStandardItem* borderLineRoot = new QStandardItem("Border Line");
        borderLineRoot->setFlags(borderLineRoot->flags() & ~Qt::ItemIsEditable);
        perimeterRoot->appendRow(borderLineRoot);
    }

    void setupShipModel()
    {
        QStandardItem* shipRoot = new QStandardItem("Ship");
        shipRoot->setFlags(shipRoot->flags() & ~Qt::ItemIsEditable);
        model->appendRow(shipRoot);
        QList<QStandardItem*> detectionRangeItems;
        detectionRangeItems << new QStandardItem("Detection Range") << new QStandardItem;
        detectionRangeItems[0]->setFlags(detectionRangeItems[0]->flags() & ~Qt::ItemIsEditable);
        shipRoot->appendRow(detectionRangeItems);

        QList<QStandardItem*> maxVelocityItems;
        maxVelocityItems << new QStandardItem("Max Velocity") << new QStandardItem;
        maxVelocityItems[0]->setFlags(maxVelocityItems[0]->flags() & ~Qt::ItemIsEditable);
        shipRoot->appendRow(maxVelocityItems);

        QList<QStandardItem*> searchVelocityItems;
        searchVelocityItems << new QStandardItem("Search Velocity") << new QStandardItem;
        searchVelocityItems[0]->setFlags(searchVelocityItems[0]->flags() & ~Qt::ItemIsEditable);
        shipRoot->appendRow(searchVelocityItems);

        QList<QStandardItem*> turningRadiusItems;
        turningRadiusItems << new QStandardItem("Turning Radius") << new QStandardItem;
        turningRadiusItems[0]->setFlags(turningRadiusItems[0]->flags() & ~Qt::ItemIsEditable);
        shipRoot->appendRow(turningRadiusItems);

        QList<QStandardItem*> minLengthItems;
        minLengthItems << new QStandardItem("Min Length Section") << new QStandardItem;
        minLengthItems[0]->setFlags(minLengthItems[0]->flags() & ~Qt::ItemIsEditable);
        shipRoot->appendRow(minLengthItems);
    }

    void setupTargetModel()
    {
        QStandardItem* targetRoot = new QStandardItem("Target");
        model->appendRow(targetRoot);
        targetRoot->setFlags(targetRoot->flags() & ~Qt::ItemIsEditable);
        createPointEditor(targetRoot, "Detection Point", QPointF());
        createRangeEditor(targetRoot, "Courses", {0, 0});

        QList<QStandardItem*> rmseItems;
        rmseItems << new QStandardItem("RootMeanSquareError") << new QStandardItem;
        rmseItems[0]->setFlags(rmseItems[0]->flags() & ~Qt::ItemIsEditable);
        targetRoot->appendRow(rmseItems);
        // Target - Velocities
        QStandardItem* velocitiesRoot = new QStandardItem("Velocities");
        velocitiesRoot->setFlags(velocitiesRoot->flags() & ~Qt::ItemIsEditable);
        targetRoot->appendRow(velocitiesRoot);

        QList<QStandardItem*> currentVelItems;
        currentVelItems << new QStandardItem("Current") << new QStandardItem;
        currentVelItems[0]->setFlags(currentVelItems[0]->flags() & ~Qt::ItemIsEditable);
        velocitiesRoot->appendRow(currentVelItems);

        QList<QStandardItem*> maxVelItems;
        maxVelItems << new QStandardItem("Max") << new QStandardItem;
        maxVelItems[0]->setFlags(maxVelItems[0]->flags() & ~Qt::ItemIsEditable);
        velocitiesRoot->appendRow(maxVelItems);

        QList<QStandardItem*> minNoiseItems;
        minNoiseItems << new QStandardItem("Min Noise Reduced") << new QStandardItem;
        minNoiseItems[0]->setFlags(minNoiseItems[0]->flags() & ~Qt::ItemIsEditable);
        velocitiesRoot->appendRow(minNoiseItems);

        QList<QStandardItem*> maxNoiseItems;
        maxNoiseItems << new QStandardItem("Max Noise Reduced") << new QStandardItem;
        maxNoiseItems[0]->setFlags(maxNoiseItems[0]->flags() & ~Qt::ItemIsEditable);
        velocitiesRoot->appendRow(maxNoiseItems);

        // Target - Other parameters
        QList<QStandardItem*> obsolescenceItems;
        obsolescenceItems << new QStandardItem("Obsolescence Time") << new QStandardItem;
        obsolescenceItems[0]->setFlags(obsolescenceItems[0]->flags() & ~Qt::ItemIsEditable);
        targetRoot->appendRow(obsolescenceItems);

        QList<QStandardItem*> avoidanceItems;
        avoidanceItems << new QStandardItem("Avoidance Distance") << new QStandardItem;
        avoidanceItems[0]->setFlags(avoidanceItems[0]->flags() & ~Qt::ItemIsEditable);
        targetRoot->appendRow(avoidanceItems);
    }

    void updateModelFromRequest(const Models::Request& request)
    {
        this->request = request;

        model->blockSignals(true);

        // Time
        model->item(0, 1)->setText(QString::number(this->request.time));

        // Update Target
        updateTargetModel();

        // Update Ship
        updateShipModel();

        // Update Perimeter
        updatePerimeterModel();

        model->blockSignals(false);
    }

    void updateTargetModel()
    {
        QStandardItem* targetRoot = model->item(1);
        // Target - Detection Point
        targetRoot->child(0, 1)->setText(QString("%1, %2")
                                             .arg(request.target.detectionPoint.x())
                                             .arg(request.target.detectionPoint.y()));
        // Target - Courses
        targetRoot->child(1, 1)->setText(QString("%1 - %2")
                                             .arg(request.target.courses.first)
                                             .arg(request.target.courses.second));
        // Target - RMSE
        targetRoot->child(2, 1)->setText(QString::number(request.target.rootMeanSquareError));
        // Target - Velocities
        QStandardItem* velocitiesRoot = targetRoot->child(3);
        velocitiesRoot->child(0, 1)->setText(QString::number(request.target.currentVelocity));
        velocitiesRoot->child(1, 1)->setText(QString::number(request.target.maxVelocity));
        velocitiesRoot->child(2, 1)->setText(QString::number(request.target.minNoiseReduced));
        velocitiesRoot->child(3, 1)->setText(QString::number(request.target.maxNoiseReduced));
        // Target - Other parameters
        targetRoot->child(4, 1)->setText(QString::number(request.target.obsolescenceTime));
        targetRoot->child(5, 1)->setText(QString::number(request.target.avoidanceDistance));
    }

    void updateShipModel()
    {
        QStandardItem* shipRoot = model->item(2);
        shipRoot->child(0, 1)->setText(QString::number(request.ship.detection_range));
        shipRoot->child(1, 1)->setText(QString::number(request.ship.max_velocity));
        shipRoot->child(2, 1)->setText(QString::number(request.ship.search_velocity));
        shipRoot->child(3, 1)->setText(QString::number(request.ship.turning_radius));
        shipRoot->child(4, 1)->setText(QString::number(request.ship.min_lenght_section));
    }

    void updatePerimeterModel()
    {
        QStandardItem* perimeterRoot = model->item(3);

        // Perimeter - Points
        perimeterRoot->child(0, 1)->setText(QString("%1, %2")
                                                .arg(request.perimeter.entrance.x())
                                                .arg(request.perimeter.entrance.y()));
        perimeterRoot->child(1, 1)->setText(QString("%1, %2")
                                                .arg(request.perimeter.exit.x())
                                                .arg(request.perimeter.exit.y()));

        // Perimeter - Borders
        QStandardItem* bordersRoot = perimeterRoot->child(2);
        bordersRoot->removeRows(0, bordersRoot->rowCount());

        for (int i = 0; i < request.perimeter.rings.size(); ++i)
        {
            QStandardItem* borderItem;
            if (i == 0)
                borderItem = new QStandardItem(QString("Outer"));
            else
                borderItem = new QStandardItem(QString("Inner %1").arg(i));

            borderItem->setFlags(borderItem->flags() & ~Qt::ItemIsEditable);
            bordersRoot->appendRow(borderItem);

            for (int j = 0; j < request.perimeter.rings[i].size(); ++j)
            {
                const QPointF& point = request.perimeter.rings[i][j];
                createPointEditor(borderItem, QString("Point (%1)").arg(j + 1), point);
            }
        }

        // Border Line
        QStandardItem* borderLineRoot = perimeterRoot->child(3);
        borderLineRoot->removeRows(0, borderLineRoot->rowCount());

        for (int i = 0; i < request.border.size(); ++i)
        {
            const QPointF& point = request.border[i];
            createPointEditor(borderLineRoot, QString("Point (%1)").arg(i + 1), point);
        }
    }

    void updateRequestFromModel()
    {
        // Time
        request.time = model->item(0, 1)->text().toDouble();

        // Update Target
        updateRequestTarget();

        // Update Ship
        updateRequestShip();

        // Update Perimeter
        updateRequestPerimeter();

        emit propertyChanged(request);
    }
    void updateRequestShip()
    {
        QStandardItem* shipRoot = model->item(2);
        request.ship.detection_range = shipRoot->child(0, 1)->text().toDouble();
        request.ship.max_velocity = shipRoot->child(1, 1)->text().toDouble();
        request.ship.search_velocity = shipRoot->child(2, 1)->text().toDouble();
        request.ship.turning_radius = shipRoot->child(3, 1)->text().toDouble();
        request.ship.min_lenght_section = shipRoot->child(4, 1)->text().toDouble();
    }

    void updateRequestTarget()
    {
        QStandardItem* targetRoot = model->item(1);
        // Target - Detection Point
        QString detectionStr = targetRoot->child(0, 1)->text();
        detectionStr.remove('[').remove(']');
        QStringList coords = detectionStr.split(',');
        if (coords.size() == 2)
        {
            request.target.detectionPoint.setX(coords[0].trimmed().toDouble());
            request.target.detectionPoint.setY(coords[1].trimmed().toDouble());
        }
        // Target - Courses
        QString coursesStr = targetRoot->child(1, 1)->text();
        QStringList courses = coursesStr.split('-');
        if (courses.size() == 2)
        {
            request.target.courses.first = courses[0].trimmed().toDouble();
            request.target.courses.second = courses[1].trimmed().toDouble();
        }

        // Target - rootMeanSquareError
        request.target.rootMeanSquareError = targetRoot->child(2, 1)->text().toDouble();

        // Target - Velocities
        QStandardItem* velocitiesRoot = targetRoot->child(3);
        request.target.currentVelocity = velocitiesRoot->child(0, 1)->text().toDouble();
        request.target.maxVelocity = velocitiesRoot->child(1, 1)->text().toDouble();
        request.target.minNoiseReduced = velocitiesRoot->child(2, 1)->text().toDouble();
        request.target.maxNoiseReduced = velocitiesRoot->child(3, 1)->text().toDouble();

        // Target - Other parameters
        request.target.obsolescenceTime = targetRoot->child(4, 1)->text().toDouble();
        request.target.avoidanceDistance = targetRoot->child(5, 1)->text().toDouble();
    }

    void updateRequestPerimeter()
    {
        QStandardItem* perimeterRoot = model->item(3);
        // Perimeter - Points
        QString entranceStr = perimeterRoot->child(0, 1)->text();
        request.perimeter.entrance = parsePoint(entranceStr);

        QString exitStr = perimeterRoot->child(1, 1)->text();
        request.perimeter.exit = parsePoint(exitStr);

        // Rings
        QStandardItem* ringsRoot = perimeterRoot->child(2);

        request.perimeter.rings.clear();

        for (size_t i = 0; i < ringsRoot->rowCount(); ++i)
        {
            QStandardItem* ringItem = ringsRoot->child(i);
            QPolygonF polygon;

            for (size_t j = 0; j < ringItem->rowCount(); ++j)
            {
                QString pointStr = ringItem->child(j, 1)->text();

                polygon << parsePoint(pointStr);
            }

            request.perimeter.rings.append(polygon);
        }

        // Border Line
        QStandardItem* borderLineRoot = perimeterRoot->child(3);

        request.border.clear();

        for (size_t i = 0; i < borderLineRoot->rowCount(); ++i)
        {
            QString pointStr = borderLineRoot->child(i, 1)->text();

            request.border << parsePoint(pointStr);
        }
    }

    QPointF parsePoint(const QString& pointStr)
    {
        QString cleanStr = pointStr;
        cleanStr /*.remove('[').remove(']')*/.remove(' ');

        QStringList coords = cleanStr.split(',');

        if (coords.size() == 2)
        {
            return QPointF(coords[0].toDouble(), coords[1].toDouble());
        }

        // TODO: Спорно.  Вызывать исключение.

        throw std::runtime_error("Длина pointStr не соответствует.");
        return QPointF{};
    }

    void showContextMenu(const QPoint& pos)
    {
        QModelIndex index = treeView->indexAt(pos);
        if (!index.isValid())
            return;

        QStandardItem* item = model->itemFromIndex(index);
        QString itemText = item->text();

        QMenu contextMenu(this);

        // For border points
        if (itemText.startsWith("Point"))
        {
            QStandardItem* parentItem = item->parent();
            if (parentItem && (parentItem->text().contains("Outer") || parentItem->text().contains("Search Region") || parentItem->text().contains("Inner") || parentItem->text() == "Border Line"))
            {
                QAction* editAction = contextMenu.addAction("Edit Point");
                connect(editAction, &QAction::triggered, [this, item]() {
                    editPoint(item);
                });
            }
        }

        contextMenu.exec(treeView->viewport()->mapToGlobal(pos));
    }

    void editPoint(QStandardItem* pointItem)
    {
        QString pointStr = pointItem->text();
        QString currentValue = pointItem->parent()->child(pointItem->row(), 1)->text();
        bool ok;
        QString newValue = QInputDialog::getText(this, "Edit Point", QString("Edit %1:").arg(pointStr), QLineEdit::Normal, currentValue, &ok);
        if (ok && !newValue.isEmpty())
        {
            pointItem->parent()->child(pointItem->row(), 1)->setText(newValue);
            updateRequestFromModel();
        }
    }

    void createPointEditor(QStandardItem* parent, const QString& name, const QPointF& point)
    {
        QList<QStandardItem*> pointItems;
        pointItems << new QStandardItem(name) << new QStandardItem(QString("%1, %2").arg(point.x()).arg(point.y()));
        pointItems[0]->setFlags(pointItems[0]->flags() & ~Qt::ItemIsEditable);

        parent->appendRow(pointItems);
    }

    void createRangeEditor(QStandardItem* parent, const QString& name, const std::pair<double, double>& range)
    {
        QList<QStandardItem*> rangeItems;
        rangeItems << new QStandardItem(name) << new QStandardItem(QString("%1 - %2").arg(range.first).arg(range.second));
        rangeItems[0]->setFlags(rangeItems[0]->flags() & ~Qt::ItemIsEditable);
        parent->appendRow(rangeItems);
    }

    Models::Request request;
    QStandardItemModel* model;
    QTreeView* treeView;
};
} // namespace Widgets::Data
