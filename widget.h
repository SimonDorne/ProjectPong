#ifndef WIDGET_H
#define WIDGET_H

#include<QWidget>
#include<QGraphicsView>
#include<QGraphicsEllipseItem>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsLineItem>
#include<QMouseEvent>
#include<iostream>
#include<cmath>
#include<QGraphicsView>
#include<QGraphicsPolygonItem>
#include<QBrush>
#include<QPen>
#include<QPointF>
#include<QTimer>
#include<QCursor>
#include<QGraphicsLineItem>
#include<chrono>
class Widget : public QGraphicsView
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void updatePosition();
    void setCursorcircle(QPointF point);
    void derivative_X();
    void com_movement();
    double gen_bias(double min, double max);
    ~Widget();

private:    //m_ for memberVariablen von widget.h Widget:: Prefix wird in widget.cpp weggelassen
    double m_deriv=500;
    double rnum=1;
    bool goal=true;
    double myGoal=0;
    double comGoal=0;
    QString  m_ausgabe;
    QGraphicsTextItem * m_text=nullptr;
    QGraphicsEllipseItem * m_circlecursor=nullptr;
    QGraphicsEllipseItem * m_supershot=nullptr;
    QGraphicsEllipseItem * m_com=nullptr;
    QGraphicsRectItem *m_field= nullptr;
    QGraphicsRectItem *m_lowerBoundery= nullptr;
    QGraphicsRectItem *m_upperBoundery= nullptr;
    QGraphicsRectItem *m_leftBoundery= nullptr;
    QGraphicsRectItem *m_rightBoundery= nullptr;
    QGraphicsEllipseItem *m_ball=nullptr;
    QTimer * m_timer =nullptr;
    double m_veloX=-0.00001;
    double m_veloY=0;
    int parity_X=-1;
    int parity_Y=-1;

};
#endif // WIDGET_H
// WIDGET_H
