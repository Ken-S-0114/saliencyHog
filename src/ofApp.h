#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "saliencySpecializedClasses.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include "iostream"

using namespace std;
using namespace cv;
using namespace saliency;

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    Mat saliencyAlgorithm(Mat mat);

    bool saliencyCheck(bool checkUI);
    void algorithmMinPixels(bool checkPixels);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // 画像
    ofImage inputOfImg;
    // UI
    Mat image;
    // 動画
    ofVideoPlayer player;
    ofVideoPlayer player_map;
    // カメラ
    ofVideoGrabber vidGrabber;
    ofPixels videoInverted;
    int camWidth, camHeight;

    // 最小と最大の要素値とそれらの位置
    cv::Point min_loc, max_loc;
    double min_val, max_val;

    // 出力データ（SPECTRAL_RESIDUAL, UI(画像)）
    ofImage outputOfImg_SPECTRAL_RESIDUAL, outputOfImg;
    // 顕著性マップ
    Mat saliencyMap, saliencyMap_norm, saliencyMap_conv, saliencyMap_color;

    // SPECTRAL_RESIDUAL(顕著性マップを求めるアルゴリズム : 画像)
    Ptr<StaticSaliencySpectralResidual> saliencyAlgorithm_SPECTRAL_RESIDUAL = StaticSaliencySpectralResidual::create();

    // 10*10の顕著マップの最小値の場所
    int widthMin, heightMin;
    // 2回目以降の条件を分けるため
    bool firstFrameCheck;
    // UIを出した箇所が次のフレームで一定数値以下であればUIを動かさない
    bool algorithmCheck;

    // どちらを描画するか：画像（Picture）か動画（Map）か
    bool imgDraw, mapDraw;

    // ファイルの拡張子
    enum File {
        png,
        mov,
        mp4,
        none
    };

    // 環境指定
    enum Use {
        release,
        preRelease,
        debug
    };
    enum Use use;

//    struct fileName {
//        string circle = "circle.png";
//        string roadSign_speed = "roadSign_speed.png";
//        string roadSign_stop = "roadSign_stop.png";
//        string icon_mail = "icon_mail.png";
//        string string_picture = "string.png";
//        string map = "movie_map.mov";
//        string driver_daytime = "driver_daytime.mp4";
//        string driver_night = "driver_night.mp4";
//        string driver_daytime_long = "昼のドライブ映像.mp4";
//        string driver_night_long = "夜のドライブ映像.mp4";
//    };
};
