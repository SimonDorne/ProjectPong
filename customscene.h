#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = 0);
    ~CustomScene();

signals:
    void signalTargetCoordinate(QPointF point);
    //-> Koordinaten werden bei mouseMoveEvent übergeben

public slots:

private:
    // The function, which made tracking the mouse
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CUSTOMSCENE_H
