#include "scroller.h"

scroller::scroller(QGraphicsView* _view, QWidget *parent) :
  QWidget(parent), m_view(_view)
{
  m_mouseLastPosition = QPoint(0,0);
  m_direction = 0;
  m_speed = 0;
  m_friction = DEFAULT_FRICTION; // implicit int to qreal
  m_damping = DEFAULT_DAMPING; // implicit int to qreal


  v_direction = 0;
  v_speed = 0;
  v_friction = DEFAULT_FRICTION; // implicit int to qreal
  v_damping = DEFAULT_DAMPING; // implicit int to qreal
  m_timer = new QTimer(this);
  m_timer->setSingleShot(true);

  connect(m_timer, SIGNAL(timeout()), this, SLOT(process()));
}

scroller::~scroller()
{}

void scroller::setMousePressed(bool pressed)
{
  m_mousePressed = pressed;
}

void scroller::startScroll()
{
  m_timer->start((int)(KINETIC_PERIOD*1000.f));
}

void scroller::resetScroll()
{
  m_mouseLastPosition = QPoint(0,0);
  m_direction = 0;
  m_speed = 0;

  v_direction = 0;
  v_speed = 0;

  m_timer->stop();
}

void scroller::setFriction(const int& _friction)
{
  m_friction = _friction;
   v_friction = _friction;// implicit int to qreal
}

void scroller::setDamping(const int& _damping)
{
  m_damping = _damping;
  v_damping = _damping;// implicit int to qreal
}

void scroller::process()
{
qreal currentX = m_view->horizontalScrollBar()->value();
qreal newX;
qreal deltaX;

qreal currentY = m_view->verticalScrollBar()->value();
qreal newY;
qreal deltaY;

if(m_mousePressed)
{
        if (m_mouseLastPosition != QPoint(0, 0))
        {
            deltaX = m_mouseLastPosition.x() - QCursor::pos().x();
            m_direction = (deltaX >= 0) ? 1 : -1;
            m_speed = 0.8f*m_speed + 0.2f*fabs(deltaX)/KINETIC_PERIOD; // v = dx/dt => speed in pixels per period (smoothed with a lowpass iir)
            newX = currentX + deltaX; // update position
            m_view->horizontalScrollBar()->setValue(newX);


            deltaY = m_mouseLastPosition.y() - QCursor::pos().y();
            v_direction = (deltaY >= 0) ? 1 : -1;
            v_speed = 0.8f*v_speed + 0.2f*fabs(deltaY)/KINETIC_PERIOD; // v = dx/dt => speed in pixels per period (smoothed with a lowpass iir)
            newY = currentY + deltaY; // update position
            m_view->verticalScrollBar()->setValue(newY);

            speed= sqrt(pow(m_speed,2)+pow(v_speed,2));
        }
        m_mouseLastPosition = QCursor::pos();
        m_timer->start((int)(KINETIC_PERIOD*1000.f));

}
else // mouse released
{
    //newX = currentX + m_direction*m_speed*KINETIC_PERIOD;// dx = v*dt => x(t) = x0 + v*dt => new position based on current speed
    //newY = currentY + v_direction*v_speed*KINETIC_PERIOD;


    newX = currentX + m_direction*m_speed*KINETIC_PERIOD;// dx = v*dt => x(t) = x0 + v*dt => new position based on current speed
    newY = currentY + v_direction*v_speed*KINETIC_PERIOD;

    m_speed = m_speed - m_friction*KINETIC_PERIOD; // v = v0 - k/dt => compute new speed
    v_speed = v_speed - v_friction*KINETIC_PERIOD;
    speed=speed-m_friction*KINETIC_PERIOD;
    if(newX > m_view->horizontalScrollBar()->maximum()) // bounce when end of view is reached
    {
    newX = m_view->horizontalScrollBar()->maximum()*2 - newX;
    m_direction = -1;
    m_speed = m_speed / m_damping; // decrease speed on bounce
    speed = speed / m_damping;
    }
    else if(newX < m_view->horizontalScrollBar()->minimum())
    {
    newX = m_view->horizontalScrollBar()->minimum()*2- newX;
    m_direction = 1;
    m_speed = m_speed / m_damping; // decrease speed on bounce

    speed = speed / m_damping;
    }


    if(newY > m_view->verticalScrollBar()->maximum()) // bounce when end of view is reached
    {
    newY = m_view->verticalScrollBar()->maximum()*2 - newY;
    v_direction = -1;
    v_speed = v_speed / v_damping; // decrease speed on bounce
    speed = speed / m_damping;
    }
    else if(newY < m_view->verticalScrollBar()->minimum())
    {
    newY = m_view->verticalScrollBar()->minimum()*2- newY;
    v_direction = 1;
    v_speed = v_speed / v_damping; // decrease speed on bounce
    speed = speed / m_damping;
    }


    m_view->horizontalScrollBar()->setValue(newX);
    m_view->verticalScrollBar()->setValue(newY);


    /*if(speed > 0)
    m_timer->start((int)(KINETIC_PERIOD*1000.f));
    else
    {
    emit scrollOver();
    m_speed = 0;
    v_speed=0;

    }
*/
    if(m_speed > 0)
    m_timer->start((int)(KINETIC_PERIOD*1000.f));
    else
    {
    emit scrollOver();
    m_speed = 0;

    }

    if(v_speed > 0)
    m_timer->start((int)(KINETIC_PERIOD*1000.f));
    else
    {
    emit scrollOver();
    v_speed = 0;

    }


}
sendSpeed("pixelsPerSecond: " + QString::number(m_speed, 'f', 2));
}
