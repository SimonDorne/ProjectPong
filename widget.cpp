#include"widget.h"
#include"customscene.h"
#include<cmath>
#include<thread>

Widget::Widget(QWidget *parent): QGraphicsView(new CustomScene(), parent){
    //m_timer bestimmt, nach wievielen Millisekunden der nächste Frame generiert wird
    m_timer = new QTimer (this);
    std::srand(std::time(nullptr));
    //ColorTheme: catpuccin
    QColor peach("#fab387");
    QColor text("#a6adc8");
    QColor surface2("#5b6078");
    QColor teal("#8bd5ca");
    QColor rosewater("#f4dbd6");
    QColor lavender("#b7bdf8");
    QColor catYellow("#eed49f");
    QColor sky("#91d7e3");
    QColor catred("#ed8796");

    //auch wenn scene() als Fullscreen geöffnet wird (in main), sorgt diese Auswahl der Größe für ein zentriertes Spielfeld
    scene()->setBackgroundBrush(text);
    scene() -> setSceneRect (0 ,0 ,800 ,545);

    //Spielfeld:
    m_field=new QGraphicsRectItem(25,25,750,500);
    m_field->setBrush(QBrush(teal));
    scene()->addItem(m_field);

    m_timer ->start (2); // ergibt einen Frame alle zwei Millisekunden
    //bei jedem Durchlauf der Main wird nach einem Delay von einer Millisekunde die Funktion updatePosition aufgerufen ->Animation des Balls
    connect (m_timer , & QTimer :: timeout , this , & Widget :: updatePosition );

    //Spielstandsanzeige
    m_text = new QGraphicsTextItem;
    m_text->setPos(300,-100);
    m_text->setDefaultTextColor(Qt::darkBlue);
    QString m_ausgabe=QString::number(myGoal)+" zu " +QString::number(comGoal);
    m_text->setScale(5);
    m_text->setPlainText(m_ausgabe);
    scene()->addItem(m_text);


    //m_circlecursor, m_com und m_ball werden bei 0,0 generiert und dann versetzt
    //daduch ist der obere Rand bei ca. 20 und der untere bei ca. 400 Pixeln (wichtig für abfragen)
    m_circlecursor = new QGraphicsEllipseItem (0 ,00 ,100 ,100);
    m_circlecursor->setBrush(QBrush(catred));
    m_circlecursor->setStartAngle(270*16);
    m_circlecursor->setSpanAngle(180*16);
    m_circlecursor->setPos(0,250);
    scene()->addItem(m_circlecursor);

    m_com = new QGraphicsEllipseItem (0 ,00 ,100 ,100);
    m_com->setBrush(QBrush(surface2));
    m_com->setStartAngle(90*16);
    m_com->setSpanAngle(180*16);
    m_com->setPos(700,250);
    scene()->addItem(m_com);

    m_ball=new QGraphicsEllipseItem(0,0,50,50);
    m_ball->setBrush(QBrush(peach));
    m_ball->setPos(400,250);
    scene()->addItem(m_ball);

    m_lowerBoundery = new QGraphicsRectItem (25 ,515 ,750 ,15);
    m_lowerBoundery->setBrush(QBrush(sky));
    scene()->addItem(m_lowerBoundery);

    m_upperBoundery = new QGraphicsRectItem (25 ,10 ,750 ,15);
    m_upperBoundery->setBrush(QBrush(lavender));
    scene()->addItem(m_upperBoundery);

    m_leftBoundery = new QGraphicsRectItem (10 ,10 ,15 ,520);
    m_leftBoundery->setBrush(QBrush(catYellow));
    scene()->addItem(m_leftBoundery);

    m_rightBoundery = new QGraphicsRectItem (775 ,10 ,15 ,520);
    m_rightBoundery->setBrush(QBrush(rosewater));
    scene()->addItem(m_rightBoundery);

    Widget::setMouseTracking(true);
    connect(static_cast<CustomScene *>(scene()), &CustomScene::signalTargetCoordinate, this, &Widget::setCursorcircle);
    //gibt bei Mausbewegung (mouseMoveEvent) Koordinaten an Funktion setCursorcircle weiter


}

//updatePosition ist für alles zuständig was irgendwie animiert sein soll
//ist mit timer connected um Animationen mit Delay durchzuführen (Aktualisierung der Frames, alle 2 ms)
void Widget::updatePosition(){
    derivative_X();
    //derivative_X erhöht die Geschwindigkeit in X-Richtung (m_veloX)
    double position_X=m_ball->pos().x()+m_veloX;
    double position_Y=m_ball->pos().y()+m_veloY;
    QGraphicsEllipseItem * checkTheMove=m_ball;
    //übertrage die zukünftigen Koordinaten an checkTheMove
    checkTheMove->setPos(position_X,position_Y);

    //Aus debug Gründen war es praktikabler oben und unten seperat zu überprüfen
    //Check für obere Barriere, falls wahr:
    //invertiere m_veloY, gehe den schritt (+10 um von Rand etwas wegzukommen), ändere parität
    if(checkTheMove->collidesWithItem(m_upperBoundery)==true){
        m_veloY=m_veloY*parity_Y;
        position_Y=m_ball->y()+m_veloY+10;
        parity_Y=-1*parity_Y;
        //std::cout<<"reset up"<<std::endl;
    }
    //analog zu upperboundery
    if(checkTheMove->collidesWithItem(m_lowerBoundery)==true){
        m_veloY=m_veloY*parity_Y;
        position_Y=m_ball->y()+m_veloY-10;
        parity_Y=-1*parity_Y;
        //std::cout<<"reset lower"<<std::endl;
    }
    //Falls check rechts auslöst: ändere goal, sodass im weiteren verlauf variablen resettet werden + änder den text
    if(checkTheMove->collidesWithItem(m_rightBoundery)==true){
        goal=false;
        std::cout<<"Tooooor"<<std::endl;
        myGoal++;
        QString m_ausgabe=QString::number(myGoal)+" zu " +QString::number(comGoal);
        m_text->setPlainText(m_ausgabe);
    }
    //analog zu check rechts
    if(checkTheMove->collidesWithItem(m_leftBoundery)==true){
        goal=false;
        std::cout<<"Tor :("<<std::endl;
        comGoal++;
        QString m_ausgabe=QString::number(myGoal)+" zu " +QString::number(comGoal);
        m_text->setPlainText(m_ausgabe);
    }
    //check für player Cursor: m_veloX wird invertiert + leicht versetzt + parität geändert
    // für Abprallwinkel, bestimme den x und y unterschied zwischen ball und center von cursor
    // arcustan(y,x) aus cmath, *1,5 für höheren Abprallwinkel
    //generiere zufallszahl um den Bias für die m_com bewegung zu bestimmen
    if(checkTheMove->collidesWithItem(m_circlecursor)==true){
        m_veloX=m_veloX*parity_X;
        position_X=m_ball->pos().x()+m_veloX+15;
        parity_X=-1*parity_X;
        double x_to_center=m_ball->x()-m_circlecursor->x();
        double y_to_center=m_ball->y()-m_circlecursor->y();
        m_veloY=atan2(y_to_center,x_to_center)*1.5;
        position_Y=m_ball->pos().y()+m_veloY;
        gen_bias(0.65,1.35);
        //std::cout<<"reset player"<<std::endl;
    }
    //Analoger check wie bei m_circlecursor
    checkTheMove->setPos(position_X,position_Y);
    if(checkTheMove->collidesWithItem(m_com)==true){
        m_veloX=m_veloX*parity_X;
        position_X=m_ball->pos().x()+m_veloX-15;
        parity_X=-1*parity_X;
        double x_to_center=m_ball->x()-m_circlecursor->x();
        double y_to_center=m_ball->y()-m_circlecursor->y();
        m_veloY=atan2(y_to_center,x_to_center)*1.5;
        position_Y=m_ball->pos().y()+m_veloY;
    }
    //Wenn alle checks durchlaufen sind, dann kann m_ball den schritt gehen
    m_ball->setPos(position_X,position_Y);
    //com_movement() bewegt m_com in idealerweise + den bias aus check circlecursor
    com_movement();

    //Resett durch ein Tor:
    if(goal==false){
        m_veloX=-0.00001*parity_X;
        m_deriv=500;
        scene()->removeItem(m_ball);
        m_ball->setPos(400,250);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        m_veloY=0;
        scene()->addItem(m_ball);
        m_com->setPos(700,250);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        goal=true;
    }
    //Bei 5 Toren gewinnt der entsprechende Spieler
    if(myGoal>4||comGoal>4){
        exit(0);
    }
}

//generiere den Bias für com_movement
double Widget::gen_bias(double min, double max) {
    // Setze den Zufallsgenerator mit der aktuellen Zeit als Seed
    // Generiere eine Zufallszahl zwischen 0 und 1
    double number = (double)std::rand() / RAND_MAX;

    // Skaliere und verschiebe die Zufallszahl, um sie im gewünschten Bereich zu erhalten
    rnum = min + number * (max - min);

    return rnum;
}
//Falls zu weit oben oder unten wird m_com auf den zur Kante passenden Punkt gesetzt
//Falls dies nicht der Fall ist: addiere zu y den m_veloY*bias Wert
void Widget::com_movement(){

    if(m_com->y()+m_veloY<26){
        m_com->setY(26);
    }
    else if(m_com->y()+m_veloY>414){
        m_com->setY(414);
    }
    else{
        m_com->setY(m_com->y()+m_veloY*rnum);
    }
}
//m_veloX wird um 1.3/x erhöht -> beschränktes Wachstum
void Widget::derivative_X(){
    m_veloX=parity_X*(std::abs(m_veloX)+1.3/(m_deriv));
    m_deriv=m_deriv+2;
}

//Wenn maus sich bewegt, wird m_circlecursor auf die neue Koordinate der Maus gesetzt
void Widget::setCursorcircle(QPointF point){
    m_circlecursor->setX(0);
    m_circlecursor->setY(point.y());
    //Werte für Kontakt mit Bande oben und unten: 414 26
    if(point.y()<26){m_circlecursor->setY(26);}
    if(point.y()>414){m_circlecursor->setY(414);}
    //Cursor ist da wo maus zuletzt ist (mouse move event)
}


//Destruktor
Widget::~Widget(){
    delete(m_circlecursor);
    delete(m_timer);
    delete(m_ball);
    delete(m_field);
    delete(m_upperBoundery);
    delete(m_lowerBoundery);
    delete(m_leftBoundery);
    delete(m_rightBoundery);
    delete(m_com);
    delete(m_text);
    delete(m_supershot);
}
