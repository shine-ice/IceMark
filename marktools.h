//
// Created by Wsy on 2023/9/25.
//

#ifndef ICEMARK_MARKTOOLS_H
#define ICEMARK_MARKTOOLS_H

#include <QWidget>
#include <QDebug>
#include <QScreen>
#include "PainterConfig.h"
#include <QMouseEvent>
#include <QVector>
#include <QButtonGroup>
#include <QColorDialog>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MarkTools; }
QT_END_NAMESPACE

class MarkTools : public QWidget {
Q_OBJECT

public:
    enum ToolWidget     // 工具栏用途-截图、贴图
    {
        Capture,
        Paste
    };
    enum ConfigWidget       // 工具配置栏-不同工具显示不同配置
    {
        Rectangle,
        Ellipse,
        Polyline,
        Arrow,
        Pen,
        Mark,
        Mosaic,
        Text,
        Eraser,
        Other
    };
    enum MarkState
    {
        None,
        BeginMark,
        FinishMark
    };

    explicit MarkTools(QWidget *parent = nullptr);
    explicit MarkTools(QWidget *parent = nullptr, QPixmap image = QPixmap(), QPoint topLeftPoint = QPoint());

    ~MarkTools() override;

    void initWindow();

    void setToolType(ToolWidget type);                      // 设置工具栏类型
    void moveTo(QRect rect);                                // 根据参数设置显示位置
    void setShow(bool flag);                                // 是否显示
    void addPainter();                                      // 添加一个画笔配置
    void addPainter(PainterConfig *painter);
    void changePainter(PainterConfig::DrawShape shape);     // 修改当前画笔
    void addPoint(QPoint point);                            // 向当前画笔添加一个绘制点
    void addText(QString text);                             // 向当前画笔添加一个文本
    void draw(QPixmap *pix);                                // 绘制
    void setOpen(bool open);                                // 设置工具栏开启或关闭状态
    void setOriginImage(QPixmap pix);                       // 设置标注的原始图片
    void clearAllPainter();                                 // 清除所有的标注操作
    void addPenWidth(int step);                             // 增大或减少线宽

private:

    bool eventFilter(QObject *watched, QEvent *event);      // 事件过滤
    void paintWidget();                                     // 内部widget上绘制图片


    void showToolWidget(ToolWidget tool);                   // 根据不同用途显示不同内容
    void showConfigWidget(ConfigWidget config);             // 根据工具显示配置栏

    void initDoneTBtnGroupState();                          // 恢复完成按钮组状态
    void initDoTBtnGroupState();                            // 恢复撤销按钮组状态
    void initArrowTBtnGroupState();                         //
    void undo();                                            // 撤销
    void redo();                                            // 重做

    bool isPointInImageWidget(QPoint pos);                  // 点是否在图片区域内
    QPoint getImagePos(QPoint cursorPos);                   // 获取鼠标相对于图片的位置

signals:
    void signalChangePainter(MarkTools::ConfigWidget type);
    void signalCancel();
    void signalPin();
    void signalSave();
    void signalCopy();
    void signalConfirm();
    void signalChangeDoStep();

private slots:

    void onBaseTBtnGroupClicked(QAbstractButton *button);
    void onArrowTBtnGroupClicked(QAbstractButton *button);

    void on_cancelTBtn_clicked(bool checked);

    void on_pinTBtn_clicked(bool checked);

    void on_saveTBtn_clicked(bool checked);

    void on_copyTBtn_clicked(bool checked);

    void on_confirmTBtn_clicked(bool checked);

    void on_undoTBtn_clicked(bool checked);

    void on_redoTBtn_clicked(bool checked);

    void on_fillTBtn_clicked(bool checked);

    void on_colorTBtn_clicked(bool checked);

    void lineEditFinishText();

protected:
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

public:
    bool isOpen;
    ToolWidget toolType;
    ConfigWidget configType;
    QVector<PainterConfig*> painterConfigArr;

private:
    Ui::MarkTools *ui;

    QPainter painter;
    MarkState curMarkState;

    int screenWidth;
    int screenHeight;

    QPixmap originImage;            // 待标注的图片
    QPixmap markedImage;            // 标注完成的图片
    int curDoStep;                  // 当前操作的步数
    QRect lastAlignRect;            // 上次参考的显示位置

    QLineEdit *customLineEdit;
};


#endif //ICEMARK_MARKTOOLS_H
