//
// Created by Wsy on 2023/9/25.
//

#ifndef ICEMARK_PAINTERCONFIG_H
#define ICEMARK_PAINTERCONFIG_H

#include <QColor>
#include <QPen>
#include <QObject>
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QGraphicsEffect>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class PainterConfig {
public:
    enum DrawShape {
        Line,           // 直线
        Rectangle,      // 矩形
        RoundRect,      // 圆角矩形
        Ellipse,        // 椭圆
        Circle,         // 圆形
        Polygon,        // 多边形
        Polyline,       // 折线
        Points,         // 点
        Arc,            // 弧形
        Path,           // 路径
        Text,           // 文本
        Pixmap,         // 图像
        Arrow_0,
        Arrow_1,
        Arrow_2,
        Mark,           // 马克笔
        Mosaic,         // 马赛克
        Eraser          // 橡皮擦
    };

    struct LineStyle {
        QVector<QPoint> points;             // 绘制图形点集
        QPen pen;                           // 画笔-轮廓
        QBrush brush;                       // 画刷-填充
        DrawShape shape;                    // 绘制图像样式
        QColor penColor;                    // 画笔颜色
        int penWidth;                       // 画笔线宽
        Qt::PenStyle penStyle;              // 画笔风格
        Qt::PenCapStyle penCapStyle;        // 画笔顶帽风格
        Qt::PenJoinStyle penJoinStyle;      // 画笔连接点风格
        Qt::FillRule fillRule;              // 填充模式
        QGradient::Spread spreadStyle;      // 铺展效果
        QColor brushColor;                  // 画刷颜色
        Qt::BrushStyle brushStyle;          // 画刷风格
        QPixmap brushTexture;               // 画刷纹理-制作马赛克、橡皮擦
        int brushWidth;                     // 画刷宽度
        QString content;                    // 文本内容
        QFont font;                         // 字体样式
    };

public:
    PainterConfig();
    PainterConfig(DrawShape shape);
    ~PainterConfig();

public:
    LineStyle *lineStyle;

public:
    void init();
    void setPen();
    void setPen(QPen pen);
    void setBrush();
    void setBrush(QBrush brush);
    void setShape(DrawShape shape);
    void setPenColor(QColor color);
    void setPenWidth(int width);
    void setPenStyle(Qt::PenStyle style);
    void setPenCapStyle(Qt::PenCapStyle style);
    void setPenJoinStyle(Qt::PenJoinStyle style);
    void setFillRule(Qt::FillRule rule);
    void setSpreadStyle(QGradient::Spread style);
    void setBrushColor(QColor color);
    void setBrushStyle(Qt::BrushStyle style);
    void setBrushTexture(QPixmap pix);
    void setBrushWidth(int width);
    void setContent(QString content);
    void setFont(QFont font);
    void draw(QPixmap *pix);
    void addPoint(QPoint point);
    void addText(QString text);
    PainterConfig* copy();
    void drawPathImage();
    void setMosaic(QPixmap pix);
    void setEraser(QPixmap pix);
    QImage blurImage(QImage img);

    QImage cvMat2QImage(const cv::Mat mat);
    cv::Mat QImage2cvMat(const QImage image);

private:
    QVector<QPoint> getArrowShape(QPoint beginPoint, QPoint endPoint, int type);
    QVector<QPoint> getLineShape(QVector<QPoint> points, double width);
    QPainter painter;
};


#endif //ICEMARK_PAINTERCONFIG_H
