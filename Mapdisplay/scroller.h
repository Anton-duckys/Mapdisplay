#ifndef SCROLLER_H
#define SCROLLER_H


#include <QtWidgets>
#define KINETIC_PERIOD 0.005f
#define DEFAULT_FRICTION 50000
#define DEFAULT_DAMPING 3
class scroller : public QWidget
{
  Q_OBJECT
public:
  explicit scroller(QGraphicsView* _view, QWidget* parent = 0);
  ~scroller();
  scroller(scroller const&) = delete;
  scroller& operator=(scroller const&) = delete;

private slots:
  void process();

public slots:
  void setFriction(const int& _friction);
  void setDamping(const int& _damping);

public:
  void startScroll();
  void resetScroll();
  void setMousePressed(bool pressed);

signals:
  void scrollOver();
  void sendSpeed(const QString& _speed);

public:
  bool m_mousePressed;

private:
  QGraphicsView* m_view;
  QTimer* m_timer;

  QPoint m_mouseLastPosition;
  qreal m_direction;
  qreal m_speed;
  qreal m_friction;
  qreal m_damping;

  qreal v_direction;
  qreal v_speed;
  qreal v_friction;
  qreal v_damping;

  qreal speed;

};
#endif // SCROLLER_H
