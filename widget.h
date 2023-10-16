#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mvcamera.h"
#include "QDebug"
#include "grabthread.h"
#include <QMessageBox>
#include "algorithm.h"
#include "string.h"
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

typedef enum
{
    GrabState_OpenGrab,
    GrabState_CloseGrab,
}GrabState;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:
    // 打开，关闭设备槽函数
    void slotBtnOpenDevice();
    void slotBtnCloseDevice();
    // 开始停止抓取图像槽函数
    void slotBtnStartGrab();
    void slotBtnStopGrab();
    // 显示图像槽函数
    void slotDisImg(QImage &img);

    // 保存图像槽函数
    void slotBtnSaveBMP();
    void slotBtnSaveJPG();
    void slotBtnSaveTIFF();
    void slotBtnSavePNG();

    // 相机参数相关槽函数
    void slotBtnGetParam();
    void slotBtnSetParam();


    // g伽马校正相关槽函数
    //void slotBtnGetGamma();
    //void slotBtnSetGamma();


private:
    void updateState(GrabState ret);

private:
    bool m_bOpenDevice;
private:
    double exposure;  // 曝光时间
    double gain;      // 增益
    double frameRate; // 帧率
    double gamma; // 伽马值

    int GetExposureTime(); // ch:设置曝光时间 | en:Set Exposure Time
    int SetExposureTime();
    int GetGain();         // ch:设置增益 | en:Set Gain
    int SetGain();
    int GetFrameRate();    // ch:设置帧率 | en:Set Frame Rate
    int SetFrameRate();
    // 图片格式
    int GetPixelFormat();
    //int GetGamma();    // ch:伽马矫正 | en:Set Gamma
    //int SetGamma();


private:
    bool isSaveImgBMP;
    bool isSaveImgJPG;
    bool isSaveImgTIFF;
    bool isSaveImgPNG;
    bool isUpdateData;

    MvCamera* mvCamera;
    GrabThread* grabThread;
    Ui::Widget *ui;
};
#endif // WIDGET_H
