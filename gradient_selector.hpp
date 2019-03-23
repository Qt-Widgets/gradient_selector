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

    qint32 magrin() const;
    void setMagrin( const qint32& magrin );

    qint32 height() const;
    void setHeight(const qint32& height);

protected:

    void addGradientPoint( const qreal pos, const QColor& color );

    virtual void paintEvent( QPaintEvent* event ) override;
    virtual void mousePressEvent( QMouseEvent* event ) override;

    virtual QSize minimumSizeHint() const override;

private:
    qint32 mHeight;
    qint32 mMagrin;
    QVector< QPair< qreal, QColor > > mGradientPositions;
};

#endif // GRADIENT_SELECTOR_HPP
