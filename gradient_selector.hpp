#ifndef GRADIENT_SELECTOR_HPP
#define GRADIENT_SELECTOR_HPP

#include <QWidget>

class GradientSelector : public QWidget
{
    Q_OBJECT

public:

    GradientSelector(
            const QColor& left = Qt::white, const QColor& right = Qt::black,
            QWidget* parent = Q_NULLPTR );
    virtual ~GradientSelector() override = default;

    qint32 height() const;
    void setHeight(const qint32& height);

    qint32 margin() const;
    void setMargin( const qint32& margin );

protected:

    void addGradientPoint( const qreal pos, const QColor& color );

    QRect getGradientRect() const;
    QRect getGradientRect( const QRect& rect ) const;

    void paintTickers( QPainter& painter, const QRect& gradientRect );

    virtual void paintEvent( QPaintEvent* event ) override;
    virtual void mousePressEvent( QMouseEvent* event ) override;
    virtual void mouseMoveEvent( QMouseEvent* event ) override;

    virtual QSize minimumSizeHint() const override;

private:
    qint32 mHeight;
    qint32 mMargin;
    QVector< QPair< qreal, QColor > >::Iterator mHoveredTicker;
    QVector< QPair< qreal, QColor > > mGradientPositions;
};

#endif // GRADIENT_SELECTOR_HPP
