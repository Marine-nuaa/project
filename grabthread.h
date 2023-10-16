#ifndef GRABTHREAD_H
#define GRABTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "mvcamera.h"
#include <QImage>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "algorithm.h"

class Algorithm;

class GrabThread :public QThread
{
    Q_OBJECT
public:
    GrabThread(MvCamera *pMvCamera);
    ~GrabThread();

    virtual void run();

    void setThreadState(bool ret);

signals:
    void grabImg(QImage& img);

private:
    MvCamera* m_pMvCamera;
    bool m_bThreadState;
};

#endif // GRABTHREAD_H
